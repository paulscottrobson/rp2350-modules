// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      raw.c
//      Purpose :   Graphics module raw drawers
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

//
//      Check if the current coordinates are visible - one for each axis.
//
#define INDRAWHORIZ() draw->inDrawingHoriz = (draw->x >= draw->clip.xLeft && draw->x <= draw->clip.xRight)
#define INDRAWVERT()  draw->inDrawingVert  = (draw->y >= draw->clip.yTop && draw->y <= draw->clip.yBottom)

static uint8_t bitDraw;                                                             // Bitmask to set current colour e.g.   ....cc..
static uint8_t bitMask;                                                             // Bitmask to clear current colour e.g. 11110011
static uint8_t *pl0,*pl1,*pl2;                                                      // Bitplane pointers.

/**
 * @brief      Move the cursor position to x,y.
 *
 * @param[in]  x     new x position
 * @param[in]  y     new y position
 */
void GFXRawMove(int32_t x,int32_t y) {
    CHECKUPDATE();                                                                  // Possible mode change.
    // draw->x = x;draw->y = y;                                                        // Save position
    // INDRAWHORIZ();INDRAWVERT();                                                     // Update in H/V flags
    // draw->currentByte = vi.drawSurface + (x / vi.pixelsPerByte) + y*vi.bytesPerLine;// The byte we are currently in.
    // draw->shiftsPerPixel = 8 / vi.pixelsPerByte;                                    // Shifts for each pixel, so 2 pixels per byte would be 4,8 would be 1.
    // draw->pixelIndex = (x % vi.pixelsPerByte) * draw->shiftsPerPixel;               // The index position in that byte this is counting overall shifts.
    // draw->pixelMask = pixelMasks[vi.pixelsPerByte];                                 // Mask for a pixel in the left most pixel (e.g. pixelIndex = 0)
}

/**
 * @brief      Draw a pixel in the current position in the current foreground colour.
 */
void GFXRawPlot(bool useFgr) {
    // if (draw->inDrawingHoriz && draw->inDrawingVert) {                              // Are we in the drawing area, e.g. the clip window
    //     uint8_t b = useFgr ? draw->foreground:draw->background;                     // Colour to use.
    //     uint8_t m = 0;                                                              // Mask the whole thing.

    //     if (vi.pixelsPerByte == 1 && draw->drawMode == 0) {                         // This optimises writes for 256 colour modes.
    //         *draw->currentByte = b;
    //         return;
    //     }

    //     if (vi.pixelsPerByte > 1) {                                                 // Multi-pixels per byte have a different mask.
    //         b = (b & draw->pixelMask) << draw->pixelIndex;     
    //         m = ~(draw->pixelMask << draw->pixelIndex);
    //     }

    //     switch(draw->drawMode) {
    //         case 0:                                                                 // Draw mode 0 : Mask out old pixels and xor it.
    //             *draw->currentByte = (*draw->currentByte) & m ^ b;                          
    //             break;
    //         case 1:                                                                 // Draw mode 1 : and with current pixels
    //             *draw->currentByte &= b;
    //             break;
    //         case 2:                                                                 // Draw mode 2 : or with current pixels
    //             *draw->currentByte |= b;
    //             break;
    //         case 3:                                                                 // Draw mode 3 : xor with current pixels
    //             *draw->currentByte ^= b;
    //             break;
    //     }
    // } 
}

/**
 * @brief      Move up. 
 */
void GFXRawUp(void) {
    // draw->currentByte -= vi.bytesPerLine;
    // draw->y--;
    // INDRAWVERT();
}

/**
 * @brief      Move down
 */
void GFXRawDown(void) {    
    // draw->currentByte += vi.bytesPerLine;
    // draw->y++;
    // INDRAWVERT();
}

/**
 * @brief      Move left
 */
void GFXRawLeft(void) {    
    // draw->x--;                                                                      // Track horizontal position. 
    // draw->pixelIndex -= draw->shiftsPerPixel;                                       // Shift left, which is moving right.
    // if (draw->pixelIndex < 0) {                                                     // Done the whole byte.
    //     draw->pixelIndex += 8;                                                      // Back to rh pixel
    //     draw->currentByte--;                                                        // In the next byte left.
    // }
    // INDRAWHORIZ();
}

/**
 * @brief      Move right
 */
void GFXRawRight(void) {   
    // draw->x++;                                                                      // Track horizontal position. 
    // draw->pixelIndex += draw->shiftsPerPixel;                                       // Shift left, which is moving right.
    // if (draw->pixelIndex == 8) {                                                    // Done the whole byte.
    //     draw->pixelIndex = 0;                                                       // Back to lh pixel
    //     draw->currentByte++;                                                        // In the next byte right.
    // }
    // INDRAWHORIZ();
}

/**
 * @brief      Draws a desktop pattern, either black and white speckles or grey if the mode supports it.
 */
void GFXDrawDesktop(void) {
    uint8_t pattern = 0xAA;
    for (int y = 0;y < modeInfo.height;y++) {
        for (int plane = 0;plane < modeInfo.bitPlaneCount;plane++) {
            memset(modeInfo.bitPlane[plane]+y*modeInfo.bytesPerLine,pattern,modeInfo.bytesPerLine);
        }
        pattern = pattern ^ 0xFF;
    }
}
