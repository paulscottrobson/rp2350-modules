// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      atomic.c
//      Purpose :   Low level drawing on the bitplanes
//      Date :      13th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************

#include "graphics_module.h"
#include "graphics_module_local.h"

static inline void _VDUDrawBitmap3(void);
static inline void _VDUDrawBitmap6(void);
static inline void _VDUDrawBitmap(void);
static int _VDUAReadPixelDirect(void);
static void _VDUAValidate(bool isValid);

static DVIMODEINFO *_dmi = NULL;                                                    // Current mode information.

static int xPixel,yPixel;                                                           // Pixel position in current window
static bool dataValid;                                                              // True if data is valid.
static uint8_t bitMask;                                                             // Bitmask (when data is valid)
static uint8_t *pl0,*pl1,*pl2;                                                      // Bitplane pointers.
static uint8_t colour = 7;                                                          // Drawing colour
static uint8_t action = 0;                                                          // What to do.
static int controlBits = 0;                                                         // Controls various aspects of atomic drawing

#define OFFWINDOWH(x)   ((x) < vc.gw.xLeft || (x) > vc.gw.xRight)
#define OFFWINDOWV(y)   ((y) < vc.gw.yBottom || (y) > vc.gw.yTop)
#define OFFWINDOW(x,y)  (OFFWINDOWH(x) || OFFWINDOWV(y))

/**
 * @brief      Set Action and Colour (from GCOL)
 *
 * @param[in]  act   Action to use (0-5 draw, or, and, xor, invert)
 * @param[in]  col   The colour to use (not used for invert)
 */
void VDUASetActionColour(int act,int col) {
    action = act;colour = col;
}

/**
 * @brief      Set the drawing control bits
 *
 * @param[in]  c     Control bits.
 */
void VDUASetControlBits(int c) {
    controlBits = c;
}

/**
 * @brief      Validate it (e.g. check on screen and in viewport)
 *
 * @param[in]  isValid  If true, assume it is valid automatically.
 */
static void _VDUAValidate(bool isValid) {
    dataValid = false;
    if (!isValid && OFFWINDOW(xPixel,yPixel)) return;                               // No, we can't do anything.

    int offset;
    if (_dmi->bitPlaneDepth == 2) {
            offset = (xPixel >> 2) + ((_dmi->height-1-yPixel) * _dmi->bytesPerLine);// Work out the offset on the bitmap planes.
            bitMask = (0xC0 >> (2*(xPixel & 3)));                                   // Work out the bitmask for the current pixel.
    } else {
            offset = (xPixel >> 3) + ((_dmi->height-1-yPixel) * _dmi->bytesPerLine);// Work out the offset on the bitmap planes.
            bitMask = (0x80 >> (xPixel & 7));                                       // Work out the bitmask for the current pixel.
    }
    pl0 = _dmi->bitPlane[0]+offset;                                                 // Set up bitmap plane pointers.
    pl1 = _dmi->bitPlane[1]+offset;
    pl2 = _dmi->bitPlane[2]+offset;

    dataValid = true;                                                               // We have valid data
}

/**
 * @brief      Plot pixel in current viewport
 *
 * @param[in]  x       x coordinate
 * @param[in]  y       y coordinate
 */
void VDUAPlot(int x,int y) {
    _dmi = DVIGetModeInformation();                                                 // Get mode information
    xPixel = x;yPixel = y;                                                          // Update the pixel positions.
    _VDUAValidate(false);                                                           // Validate the position.
    if (dataValid) _VDUDrawBitmap();                                                // Draw pixel if valid.
}

/**
 * @brief      Draw horizontal line
 *
 * @param[in]  x1      The x1 coordinate
 * @param[in]  x2      The x2 coordinate
 * @param[in]  y       Y coordinate
 */
void VDUAHorizLine(int x1,int x2,int y) {
    _dmi = DVIGetModeInformation();                                                 // Get mode information
    int ppb = _dmi->bitPlaneDepth==2 ? 4 : 8;
    if (OFFWINDOWV(y)) return;                                                      // Vertically out of range => no line.
    if (x1 >= x2) { int n = x1;x1 = x2;x2 = n; }                                    // Sort the x coordinates into order.
    if (x2 < vc.gw.xLeft || x1 >= vc.gw.xRight) return;                             // On screen area (e.g. lower off right, higher off left)
    x1 = max(x1,vc.gw.xLeft);x2 = min(x2,vc.gw.xRight);                             // Trim horizontal line to port.
    xPixel = x1;yPixel = y;dataValid = false;                                       // First pixel.
    _VDUAValidate(false);
    int pixelCount = x2-x1+1;                                                       // Pixels to draw

    //
    //      First, we go to a byte boundary, if there are enough pixels.
    //
    while (pixelCount > 0 && (bitMask & 0x80) == 0) {                               // Shift until reached byte boundary
        _VDUDrawBitmap();
        VDUARight();
        pixelCount--;
    }
    //
    //      While on a byte boundary, if there are enough pixels, do whole bytes. I did consider doing it in longs at this point.
    //
    while (pixelCount >= ppb) {                                                     // Now do it byte chunks.
        bitMask = 0xFF;_VDUDrawBitmap();                                            // This does the line in whole bytes.
        pl0++;pl1++;pl2++;                                                          // Advance pointer
        pixelCount -= ppb;                                                          // 8 fewer pixels
        xPixel += ppb;                                                              // Keep the position up to date, doesn't really matter.
    }
    //
    //      Do any remaining single pixels
    //
    bitMask = _dmi->bitPlaneDepth == 2? 0xC0 : 0x80;                                // We know we are on a byte boundary
    while (pixelCount-- > 0) {                                                      // Draw any remaining pixels.
        _VDUDrawBitmap();
        VDUARight();
    }
}

/**
 * @brief      Vertical line
 *
 * @param[in]  x       X coordinate
 * @param[in]  y1      y1 coordinate
 * @param[in]  y2      y2 coordinate
 * @param[in]  colour  colour
 */
void VDUAVertLine(int x,int y1,int y2) {
    _dmi = DVIGetModeInformation();                                                 // Get mode information
    if (OFFWINDOWH(x)) return;                                                      // Off screen.
    if (y1 > y2) { int n = y1;y1 = y2;y2 = n; }                                     // Sort y coordinates
    if (y2 < vc.gw.yBottom || y1 >= vc.gw.yTop) return;                             // Wholly off top or bottom.
    y1 = max(y1,vc.gw.yBottom);y2 = min(y2,vc.gw.yTop);                             // Clip into region.
    xPixel = x;yPixel = y1;dataValid = false;                                       // Set start and validate
    _VDUAValidate(false);
    int pixelCount = y2-y1+1;                                                       // Pixels to draw
    while (pixelCount-- > 0) {                                                      // Shift until reached byte boundary
        _VDUDrawBitmap();VDUAUp();
    }
}

/**
 * @brief      Move current up
 */
void VDUAUp(void) {
    yPixel++;                                                                       // Pixel up
    pl0 -= _dmi->bytesPerLine;                                                      // Shift pointers to next line up.
    pl1 -= _dmi->bytesPerLine;
    pl2 -= _dmi->bytesPerLine;
    if (dataValid) dataValid = (yPixel <= vc.gw.yTop);                              // Still in window
}

/**
 * @brief      Move current down
 */
void VDUADown(void) {
    yPixel--;                                                                       // Pixel down
    pl0 += _dmi->bytesPerLine;                                                      // Shift pointers to next line down
    pl1 += _dmi->bytesPerLine;
    pl2 += _dmi->bytesPerLine;
    if (dataValid) dataValid = (yPixel >= vc.gw.yBottom);                           // Still in window
}

/**
 * @brief      Move current left
 */
void VDUALeft(void) {
    xPixel--;                                                                       // Pixel left
    if (_dmi->bitPlaneDepth == 2) {
        bitMask = (bitMask << 2) & 0xFF;                                            // Shift bitmap left
        if (bitMask == 0) {                                                         // Off the left side.
            bitMask = 0x03;                                                         // Reset bitmap
            pl0--;pl1--;pl2--;                                                      // Bump plane pointers
        }
    } else {
        bitMask = (bitMask << 1) & 0xFF;                                            // Shift bitmap left
        if (bitMask == 0) {                                                         // Off the left side.
            bitMask = 0x01;                                                         // Reset bitmap
            pl0--;pl1--;pl2--;                                                      // Bump plane pointers
        }
    }
    if (dataValid) dataValid = (xPixel >= vc.gw.xLeft);                             // Still in window
}

/**
 * @brief      Move current right
 */
void VDUARight(void) {
    xPixel++;                                                                       // Pixel right
    if (_dmi->bitPlaneDepth == 2) {
        bitMask >>= 2;                                                              // Shift bitmap right
        if (bitMask == 0) {                                                         // Off the right side.
            bitMask = 0xC0;                                                         // Reset bitmap
            pl0++;pl1++;pl2++;                                                      // Bump plane pointers
        }
    } else {
        bitMask >>= 1;                                                              // Shift bitmap right
        if (bitMask == 0) {                                                         // Off the right side.
            bitMask = 0x80;                                                         // Reset bitmap
            pl0++;pl1++;pl2++;                                                      // Bump plane pointers
        }
    }
    if (dataValid) dataValid = (xPixel < vc.gw.xRight);                             // Still in window
}

/**
 * @brief      Line drawing (simple Bresenham)
 *
 * @param[in]  x0      The x0 coordinate
 * @param[in]  y0      The y0 coordinate
 * @param[in]  x1      The x1 coordinate
 * @param[in]  y1      The y1 coordinate
 * @param[in]  colour  The colour
 */

void VDUALine(int x0, int y0, int x1, int y1) {
    bool drawDot = true;                                                            // Used for dotted lines. If GFXC_DOTTED clear this never changes.

    if (controlBits == 0 || action != 0) {                                          // If control bits set or not simply drawing, use the Bresenham
        if (y0 == y1) {                                                             // Use the horizontal one.
            VDUAHorizLine(x0,x1,y1);
            return;
        }
        if (x0 == x1) {                                                             // Use the vertical one.
             VDUAVertLine(x0,y0,y1);
             return;
        }
    }

    _dmi = DVIGetModeInformation();                                                 // Get mode information

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;

    xPixel = x0;yPixel = y0;                                                        // Start at x0,y0

    _VDUAValidate(false);                                                           // Validate the current

    while(xPixel != x1 || yPixel != y1) {

        if (!dataValid) _VDUAValidate(false);                                       // Try to validate if invalid
        if (dataValid && drawDot) _VDUDrawBitmap();                                 // If valid, then draw line.

        if (controlBits & GFXC_DOTTED) drawDot = !drawDot;                          // Toggle draw flag if dotted.

        int e2 = 2 * error;
        if (e2 >= dy) {
            error = error + dy;
            if (sx < 0) { VDUALeft(); } else { VDUARight(); }
        }
        if (e2 <= dx) {
            error = error + dx;
            if (sy < 0) { VDUADown(); } else { VDUAUp();}
        }
    }
    if ((controlBits & GFXC_NOENDPOINT) == 0) {                                     // Draw the last point
        if (!dataValid) _VDUAValidate(false);
        if (dataValid & drawDot) _VDUDrawBitmap();
    }
}

/**
 * @brief      Draw bitmap dispatched
 */
static inline void _VDUDrawBitmap(void) {
    if (!dataValid) return;                                                         // Not valid drawing.
    if (_dmi->bitPlaneDepth == 2) {
        _VDUDrawBitmap6();
    } else {
        _VDUDrawBitmap3();
    }
}

/**
 * @brief      Draw bitmap for 3 plane
 */
static inline void _VDUDrawBitmap3(void) {

    switch(action) {

        case 0:                                                                     // Standard draw
            *pl0 = ((*pl0) & (~bitMask)) | ((colour & 1) ? bitMask:0);
            *pl1 = ((*pl1) & (~bitMask)) | ((colour & 2) ? bitMask:0);
            *pl2 = ((*pl2) & (~bitMask)) | ((colour & 4) ? bitMask:0);
            break;
        case 1:                                                                     // OR Draw
            *pl0 |= ((colour & 1) ? bitMask:0);
            *pl1 |= ((colour & 2) ? bitMask:0);
            *pl2 |= ((colour & 4) ? bitMask:0);
            break;
        case 2:                                                                     // AND Draw
            *pl0 &= ((colour & 1) ? bitMask:0);
            *pl1 &= ((colour & 2) ? bitMask:0);
            *pl2 &= ((colour & 4) ? bitMask:0);
            break;
        case 3:                                                                     // XOR Draw
            *pl0 ^= ((colour & 1) ? bitMask:0);
            *pl1 ^= ((colour & 2) ? bitMask:0);
            *pl2 ^= ((colour & 4) ? bitMask:0);
            break;
        case 4:                                                                     // Invert Draw
            *pl0 ^= bitMask;
            *pl1 ^= bitMask;
            *pl2 ^= bitMask;
            break;
    }
}


/**
 * @brief      Draw bitmap for a 1 plane 6 bit colour display
 */
static inline void _VDUDrawBitmap6(void) {


    switch(action) {

        case 0:                                                                     // Standard draw
            *pl0 =  ((*pl0) & (~bitMask)) | ((colour & 1) ? bitMask & 0xAA : 0) |
                    ((colour & 8)  ? bitMask & 0x55:0);
            *pl1 =  ((*pl1) & (~bitMask)) | ((colour & 2) ? bitMask & 0xAA : 0) |
                    ((colour & 16) ? bitMask & 0x55 :0);
            *pl2 =  ((*pl2) & (~bitMask)) | ((colour & 4) ? bitMask & 0xAA : 0) |
                    ((colour & 32) ? bitMask & 0x55 :0);
             break;
        case 1:                                                                     // OR Draw
            *pl0 |= (((colour & 1) ? bitMask & 0xAA:0) | ((colour & 8) ? bitMask & 0x55:0));
            *pl1 |= (((colour & 2) ? bitMask & 0xAA:0) | ((colour & 16) ? bitMask & 0x55:0));
            *pl2 |= (((colour & 4) ? bitMask & 0xAA:0) | ((colour & 32) ? bitMask & 0x55:0));
            break;
        case 2:                                                                     // AND Draw
            *pl0 &= (((colour & 1) ? bitMask & 0xAA:0) | ((colour & 8) ? bitMask & 0x55:0));
            *pl1 &= (((colour & 2) ? bitMask & 0xAA:0) | ((colour & 16) ? bitMask & 0x55:0));
            *pl2 &= (((colour & 4) ? bitMask & 0xAA:0) | ((colour & 32) ? bitMask & 0x55:0));
            break;
        case 3:                                                                     // XOR Draw
            *pl0 ^= (((colour & 1) ? bitMask & 0xAA:0) | ((colour & 8) ? bitMask & 0x55:0));
            *pl1 ^= (((colour & 2) ? bitMask & 0xAA:0) | ((colour & 16) ? bitMask & 0x55:0));
            *pl2 ^= (((colour & 4) ? bitMask & 0xAA:0) | ((colour & 32) ? bitMask & 0x55:0));
            break;
        case 4:                                                                     // Invert Draw
            *pl0 ^= bitMask;
            *pl1 ^= bitMask;
            *pl2 ^= bitMask;
            break;
    }
}

/**
 * @brief      Atomic pixel read
 *
 * @param[in]  x        Physical x position
 * @param[in]  y        Physical y position
 * @param[in]  isValid  True if assume validity so can read outside gfx window.
 *
 * @return     pixel colour or -1 if not readable.
 */
int  VDUAReadPixel(int x,int y,bool isValid) {
    _dmi = DVIGetModeInformation();                                                 // Get mode information
    xPixel = x;yPixel = y;                                                          // Update the pixel positions.
    _VDUAValidate(isValid);                                                         // Validate the position.
    return _VDUAReadPixelDirect();
}

/**
 * @brief      Helper function, extracts a colour in l00h format out of a plane in 64 colour mode.
 *
 * @param      p     { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
static int _VDURPDPlane64(uint8_t *p) {
    int c = ((*p) & bitMask & 0xAA) ? 1 : 0;
    return ((*p) & bitMask & 0x55) ? c+8 : c;
}

/**
 * @brief      Lowest level read, after everything has been set up and validated
 *
 * @return     pixel colour of -1 if not readable.
 */
static int _VDUAReadPixelDirect(void) {    
    int colour = 0;
    if (!dataValid) return -1;                                                      // Off window

    if (_dmi->bitPlaneDepth == 2) {                                                 // 64 colour mode.
        colour = _VDURPDPlane64(pl0) + (_VDURPDPlane64(pl1) << 1)                   // Extract colour bits from each plane.
                                                + (_VDURPDPlane64(pl2) << 2); 
    } else {                                                                        // 1 bit per pixel planes (8, 2 colour modes)
        if ((*pl0) & bitMask) colour += 1;                                          // 2 colours (1 plane)
        if (_dmi->bitPlaneCount > 1) {                                              // 8 colours (3 planes)
            if ((*pl1) & bitMask) colour += 2;
            if ((*pl2) & bitMask) colour += 4;
        }
    }
    return colour;
}

/**
 * @brief      Output a row of pixels, not drawing background (for graphic text)
 *
 * @param[in]  x          Physical X
 * @param[in]  y          Physical Y
 * @param[in]  pixelData  Pixel pattern, left = MSB
 */
void VDUAOutputByte(int x,int y,uint8_t pixelData) {
    while (pixelData != 0) {
        if (pixelData & 0x80) VDUAPlot(x,y);
        x++;
        pixelData <<= 1;
    }
}

