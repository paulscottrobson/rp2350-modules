// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      text.c
//      Purpose :   Text Atomic Functions
//      Date :      16th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"


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
 * @brief      Copy screen lines, blank the bottom or top line.
 *
 * @param[in]  yFrom    Copy from here
 * @param[in]  yTo      To here
 * @param[in]  yTarget  Until this is reached.
 * @param[in]  yClear   where to clear?
 * @param[in]  xLeft    left edge of window to scroll.
 * @param[in]  xRight   right edge of window to scroll.
 */
void VDUScrollV(int yFrom,int yTo,int yTarget,int yClear,int xLeft, int xRight) {
    DVIMODEINFO *dmi = DVIGetModeInformation();                                     // Get information.
    yFrom *= vc.textHeight;yTo *= vc.textHeight;yTarget *= vc.textHeight;           // Scale from characters to lines.
    int dir = (yFrom > yTarget) ? -1 : 1;                                           // How From and to are adjusted.
    int bytesPerCharacter = (dmi->bitPlaneDepth == 1) ? 1 : 2;                      // Bytes per character. 
    int copySize = (vc.tw.xRight-vc.tw.xLeft+1) * bytesPerCharacter;                // Amount to copy.
    bool isComplete = false;
    while (!isComplete) {
        for (int i = 0;i < dmi->bitPlaneCount;i++) {                                // For each bitplane
            if (yFrom >= 0 && yTo >= 0 && yFrom < dmi->height && yTo < dmi->height) {
                uint8_t *f = dmi->bitPlane[i] + dmi->bytesPerLine * yFrom;          // Start Line from
                uint8_t *t = dmi->bitPlane[i] + dmi->bytesPerLine * yTo;            // Start Line to.
                f = f + xLeft * bytesPerCharacter;                                  // Start of the copy block
                t = t + xLeft * bytesPerCharacter;
                memcpy(t,f,copySize);                                               // Copy it
            }   
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
void VDUScrollH(int xLeft,int xRight,int dir,int yTop, int yBottom)
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
            VDUScrollH(Left,Right,1,Top,Bottom);
            break;
        case 1: /* left */
            VDUScrollH(Left,Right,-1,Top,Bottom);
            break;
        case 2: /* down */
            VDUScrollV(Bottom,Bottom+1,Top-1,Top,Left,Right);
            break;
        case 3: /* up */
            VDUScrollV(Top+1,Top,Bottom+1,Bottom,Left,Right);
        break;
    }
}

/**
 * @brief      Copy a character on screen
 *
 * @param[in]  xFrom  source x
 * @param[in]  yFrom  source y
 * @param[in]  xTo    dest x
 * @param[in]  yTo    dest y
 */
void VDUCopyChar(int xFrom,int yFrom,int xTo,int yTo) {
    DVIMODEINFO *dmi = DVIGetModeInformation();            
    uint32_t bytesPerCharacter = (dmi->bitPlaneDepth == 1) ? 1 : 2;                 // 1 or 2 bytes per character line.
    for (int plane = 0;plane < dmi->bitPlaneCount;plane++) {                        // Each plane, calculate from and to.
        uint8_t *f = dmi->bitPlane[plane]+xFrom*bytesPerCharacter+yFrom*vc.textHeight*dmi->bytesPerLine;
        uint8_t *t = dmi->bitPlane[plane]+xTo  *bytesPerCharacter+yTo  *vc.textHeight*dmi->bytesPerLine;
        for (int y = 0;y < vc.textHeight;y++) {                                     // Copy each line, 1 or 2 bytes
            *t = *f;
            if (bytesPerCharacter == 2) *(t+1) = *(f+1);
            t += dmi->bytesPerLine;                                                 // Next line.
            f += dmi->bytesPerLine;            
        }
    }
}