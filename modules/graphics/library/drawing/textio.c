// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      textio.c
//      Purpose :   Text I/O functions.
//      Date :      13th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"


static void _VDUScroll(int yFrom,int yTo,int yTarget,int yClear,int xLeft, int xRight);
static void _VDUScrollH(int xLeft,int xRight,int dir,int yTop, int yBottom);

/**
 * @brief      Convert a pixel pattern to the byte to write to the plane
 *             adjusting for foreground and background colours.
 *
 * @param[in]  line   Pixel Data
 * @param[in]  plane  Bitplane.
 *
 * @return     Modified pixel data.
 */
static uint8_t _VDUMapToBitplaneByte(uint8_t line,int plane) {
    uint8_t mask = 1 << plane;
    if ((vc.fgCol & mask) == (vc.bgCol & mask)) {                                   // Same bit pattern, e.g. it's all the same colour.
        return (vc.fgCol & mask) ? 0xFF:0x00;
    }
    return (vc.fgCol & mask) ? line : line ^ 0xFF;                                  // Colour is different, either value or inverse.
}

/**
 * @brief      Convert the colour in ..r..R format to RrRrRrRr e.g. GCOL format
 *             to Bitplane format
 *
 * @param[in]  colour  The colour to convert in bits 0 and 3.
 *
 * @return     The full colour byte
 */
static uint8_t _VDUMapToBitplaneByte64ToColourMask(int colour) {
    colour = ((colour & 1) << 1) | ((colour & 8) >> 3);                             // Convert from x..y to yx
    return colour | (colour << 2) | (colour << 4) | (colour << 6);                  // Expand it out int yxyxyxyx
}

/**
 * @brief      Convert a pixel pattern to the byte to write to the plane
 *             adjusting for foreground and background colours. This is the 2
 *             bits per plane 64 colour version
 *
 * @param[in]  line   Pixel data in lower 4 bits
 * @param[in]  plane  Bitplane in use.
 *
 * @return     Byte to output equivalent to those 4 pixels in the current colour.
 */
static uint8_t _VDUMapToBitplaneByte64(uint8_t line,int plane) {
    uint8_t mask = 9 << plane;                                                      // 9 is 001001 because of rgbRGB pattern
    if ((vc.fgCol & mask) == (vc.bgCol & mask)) {                                   // Same bit pattern, e.g. it's all the same colour.
        return (vc.fgCol & mask) ? 0xFF:0x00;
    }
    uint8_t fgBits = _VDUMapToBitplaneByte64ToColourMask(vc.fgCol >> plane);        // Convert colours to RrRrRrRr e.g. the bitplane format.
    uint8_t bgBits = _VDUMapToBitplaneByte64ToColourMask(vc.bgCol >> plane);
    return (line & fgBits) | ((~line) & bgBits);
}

/**
 * @brief      Output a character onto the display, text mode, current fgr/bgr
 *
 * @param[in]  x     x Coordinate
 * @param[in]  y     y Coordinate
 * @param[in]  c     Character to write
 */
void VDURenderCharacter(int x,int y,int c) {

    static const uint8_t _pixelMap[16] = {                                          // Convert 4 bit pixel to extended 8 bit byte.
        0x00,0x03,0x0C,0x0F,0x30,0x33,0x3C,0x3F,
        0xC0,0xC3,0xCC,0xCF,0xF0,0xF3,0xFC,0xFF
    };
    
    if (c < 32 || c == 127) return;                                                 // Not a displayable character
    if (x < vc.tw.xLeft || x > vc.tw.xRight ||                                      // Out of the text window
                            y < vc.tw.yTop || y > vc.tw.yBottom) return;                


    DVIMODEINFO *dmi = DVIGetModeInformation();            
    for (int plane = 0;plane < dmi->bitPlaneCount;plane++) {                        // Do all three planes.
        for (int yChar = 0;yChar < vc.textHeight;yChar++) {                         // Each line in a bit plane.
            uint8_t pixels = (vc.textHeight == 8) ?                                 // Get the character line data.
                                    VDUGetCharacterLineData(c,yChar,false):
                                    VDUGetCharacterLineData(c,yChar,true);   
            uint8_t *p = dmi->bitPlane[plane]+                                      // Position in bitmap.
                                (y*vc.textHeight+yChar)*dmi->bytesPerLine;      
            if (dmi->bitPlaneDepth == 1) {                                          // Handle 8 bits per bitmap (8 colours)
                *(p+x) = _VDUMapToBitplaneByte(pixels,plane);
            } else {                                                                // Handle 4 bits per bitmap (64 colours)
                *(p+x*2) = _VDUMapToBitplaneByte64(_pixelMap[pixels >> 4],plane);
                *(p+x*2+1) = _VDUMapToBitplaneByte64(_pixelMap[pixels & 0x0F],plane);
            }
        }
    }
}


/**
 * @brief      Set the text foreground/background colour
 *
 * @param[in]  colour  Colour in bits 0..6, foreground bit 7 = 0, background bit 7 = 1
 */
void VDUSetTextColour(int colour) {
    if (colour & 0x80) {
        vc.bgCol = colour & 0x7F;
    } else {
        vc.fgCol = colour & 0x7F;
    }
}

/**
 * @brief      Set the text size. Currently only supports double height.
 *
 * @param[in]  xSize  x Scale
 * @param[in]  ySize  y Scale.
 */
void VDUSetTextSize(uint8_t xSize,uint8_t ySize) {
    vc.textWidth = 8;
    vc.textHeight = (ySize == 2) ? 16:8;
}

/**
 * @brief      VDU Cursor command
 *
 * @param[in]  c     Command (8,9,10,11 or 13)
 */
void VDUCursor(int c) {
    switch(c) {
        case 8:                                                                     // VDU 8 left
            vc.xCursor--;
            if (vc.xCursor < 0) {                                                   // Off the left
                vc.xCursor = vc.tw.xRight-vc.tw.xLeft;                              // Go to EOL 
                VDUCursor(11);                                                      // And up
            }
            break;
        case 9:                                                                     // VDU 9 right
            vc.xCursor++; 
            if (vc.xCursor > vc.tw.xRight-vc.tw.xLeft) {                            // Off the write
                vc.xCursor = 0;                                                     // Go to SOL
                VDUCursor(10);                                                      // And down.
            }
            break;
        case 10:                                                                    // VDU 10 down
            vc.yCursor++;                                                              
            if (vc.yCursor > vc.tw.yBottom-vc.tw.yTop) {                            // Vertical scroll up
   	            _VDUScroll(vc.tw.yTop+1,vc.tw.yTop,vc.tw.yBottom+1,vc.tw.yBottom,vc.tw.xLeft,vc.tw.xRight); 
                vc.yCursor--;
                VDUScrollTextEndMarkers(-1);                                        // Scroll text end markers as well.
            }
            break;
        case 11:                                                                    // VDU 11 up.
            vc.yCursor--;                                                              
            if (vc.yCursor < 0) {
      	        _VDUScroll(vc.tw.yBottom,vc.tw.yBottom+1,vc.tw.yTop,                // Vertical scroll down.
                                    vc.tw.yTop,vc.tw.xLeft,vc.tw.xRight);                   
                vc.yCursor = 0;
                VDUScrollTextEndMarkers(1);                                         // Scroll text end markers as well.
            }
            break;
        case 13:                                                                    // VDU 13 does not go down a line.
            vc.xCursor = 0; 
            break;
    }
}

/**
 * @brief      Copy screen lines, blank the bottom or top line.
 *
 * @param[in]  yFrom    Copy from here
 * @param[in]  yTo      To here
 * @param[in]  yTarget  Until this is reached.
 * @param[in]  yClear   where to clear?
 * @param[in]  xLeft    left edge of window to scroll.
 * @param[in]  xRight   right edge of window to scroll.
 */
static void _VDUScroll(int yFrom,int yTo,int yTarget,int yClear,int xLeft, int xRight) {
    DVIMODEINFO *dmi = DVIGetModeInformation();                                     // Get information.
    yFrom *= vc.textHeight;yTo *= vc.textHeight;yTarget *= vc.textHeight;           // Scale from characters to lines.
    int dir = (yFrom > yTarget) ? -1 : 1;                                           // How From and to are adjusted.
    int bytesPerCharacter = (dmi->bitPlaneDepth == 1) ? 1 : 2;                      // Bytes per character. 
    int copySize = (vc.tw.xRight-vc.tw.xLeft+1) * bytesPerCharacter;                // Amount to copy.
    bool isComplete = false;
    while (!isComplete) {
        for (int i = 0;i < dmi->bitPlaneCount;i++) {                                // For each bitplane
            uint8_t *f = dmi->bitPlane[i] + dmi->bytesPerLine * yFrom;              // Start Line from
            uint8_t *t = dmi->bitPlane[i] + dmi->bytesPerLine * yTo;                // Start Line to.
            f = f + xLeft * bytesPerCharacter;                                      // Start of the copy block
            t = t + xLeft * bytesPerCharacter;
            memcpy(t,f,copySize);                                                   // Copy it
        }        
        if (yFrom == yTarget) isComplete = true;                                    // Done the last one ?
        yFrom += dir;yTo += dir;                                                    // Scroll the line down.
    }
    for (int x = vc.tw.xLeft;x <= vc.tw.xRight;x++) {                               // Blank the new line.
        VDURenderCharacter(x,yClear,' ');
    }
}

/**
 *@brief      Scroll screen lines horizontally, blank left or right end
 *
 * @param[in]  xLeft    Left edge of window to scroll
 * @param[in]  xRight   Right edge of window to scroll
 * @param[in]  xDir     Direction -1 = left, +1 = right.
 * @param[in]  yTop     Top edge of window to scroll.
 * @param[in]  yBottom  Bottom edge of window to scroll.
 */
static void _VDUScrollH(int xLeft,int xRight,int dir,int yTop, int yBottom)
{
    DVIMODEINFO *dmi = DVIGetModeInformation();                                     // Get information.
    int bytesPerCharacter = (dmi->bitPlaneDepth == 1) ? 1 : 2;                      // Bytes per character.
    int xFrom, xTo;
    if (dir < 0) {
        xTo = xLeft*bytesPerCharacter;
        xFrom = xTo + bytesPerCharacter;
    } else {
        xFrom = xLeft*bytesPerCharacter;
        xTo = xFrom + bytesPerCharacter;
    } // Byte offsets to copy from and to.
    int copySize = (vc.tw.xRight-vc.tw.xLeft)*bytesPerCharacter;                    // Amount to copy.
    for (int y=vc.tw.yTop*vc.textHeight; y<(vc.tw.yBottom+1)*vc.textHeight;y++) {
        for (int i = 0;i < dmi->bitPlaneCount;i++) {                                // For each bitplane
            uint8_t *la = dmi->bitPlane[i] + dmi->bytesPerLine * y;                 // Start Line from
            memmove(la+xTo,la+xFrom,copySize);                                      // Copy it
        }        
    // Scroll the line left/right.
    }
    for (int y = vc.tw.yTop;y <= vc.tw.yBottom;y++) {                                // Blank the new column.
        VDURenderCharacter(dir<0?xRight:xLeft,y,' ');
    }
}

/**
 * @brief     Scroll a rectangle in any direction.
 */
void VDUScrollRect(int ext, int direction)
{
    DVIMODEINFO *dmi = DVIGetModeInformation();            
    int Top,Bottom,Left,Right;
    if (ext) {
        Top = 0;
        Bottom = (dmi->height >> 3)-1;
        Left = 0;
        Right = (dmi->width >> 3)-1;
    } else {
        Top = vc.tw.yTop;
        Bottom = vc.tw.yBottom;
        Left = vc.tw.xLeft;
        Right = vc.tw.xRight;
    }
    switch (direction) {
        case 0: /* right */
            _VDUScrollH(Left,Right,1,Top,Bottom);
            break;
        case 1: /* left */
            _VDUScrollH(Left,Right,-1,Top,Bottom);
            break;
        case 2: /* down */
            _VDUScroll(Bottom,Bottom+1,Top-1,Top,Left,Right);
            break;
        case 3: /* up */
            _VDUScroll(Top+1,Top,Bottom+1,Bottom,Left,Right);
        break;
    }
}

/**
 * @brief      Character output to text display
 *
 * @param[in]  c     Character to output (non control)
 */
void VDUWriteText(uint8_t c) {
    VDURenderCharacter(vc.xCursor+vc.tw.xLeft,vc.yCursor+vc.tw.yTop,c);            // Write character
    if (vc.xCursor == vc.tw.xRight-vc.tw.xLeft) {                                  // Written to the right hand column, mark extended.
        VDUSetTextEndMarker(vc.yCursor+vc.tw.yTop);                     
    }
    VDUWrite(9);                                                                   // Move forward.
}

/**
 * @brief      Reset the default text colours.
 */
void VDUSetDefaultTextColour(void) {
    VDUSetTextColour(0x07);
    VDUSetTextColour(0x80);
}

