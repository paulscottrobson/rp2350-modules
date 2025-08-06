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
#define INDRAWHORIZ() inDrawingHoriz = (draw->x >= draw->clip.xLeft && draw->x <= draw->clip.xRight)
#define INDRAWVERT()  inDrawingVert  = (draw->y >= draw->clip.yTop && draw->y <= draw->clip.yBottom)

static uint8_t bitDraw;                                                             // Bitmask to set current colour e.g.   ....cc..
static uint8_t bitMask;                                                             // Bitmask to clear current colour e.g. 11110011
static uint8_t *pl[DVI_MAX_BITPLANES];                                              // Bitplane pointers.
bool    inDrawingVert,inDrawingHoriz;                                               // True if currently drawing (e.g. as move, drawing occurs)

/**
 * @brief      Move the cursor position to x,y.
 *
 * @param[in]  x     new x position
 * @param[in]  y     new y position
 */
void GFXRawMove(int32_t x,int32_t y) {
    uint32_t offset;
    CHECKUPDATE();                                                                  // Possible mode change.
    draw->x = x;draw->y = y;                                                        // Save position
    INDRAWHORIZ();INDRAWVERT();                                                     // Update in H/V flags
    if (modeInfo.bitPlaneDepth == 1) {
        offset = (x >> 3) + y * modeInfo.bytesPerLine;
        bitDraw = 0x80 >> (x & 7);
        bitMask = bitDraw ^ 0xFF;
    } else {
        // 64 bit goes here.
    }
    for (int i = 0;i < modeInfo.bitPlaneCount;i++) {                                // Initialise the bitplane position pointers.
        pl[i] = modeInfo.bitPlane[i] + offset;
    }
}    

/**
 * @brief      Draw a pixel in the current position in the current foreground colour.
 */
void GFXRawPlot(bool useFgr) {
    if (!inDrawingVert || !inDrawingHoriz) return;                                  // Cannot plot as we are off the screen.
    uint8_t colour = useFgr ? draw->foreground:draw->background;                    // Drawing colour.    
    for (int plane = 0;plane < modeInfo.bitPlaneCount;plane++) {
        uint8_t *p = pl[plane];                                                     // Byte we are working on.
        bool isSet = (colour & (1 << plane)) != 0;                                  // Is the pixel set on this plane, or not ?
        switch(draw->drawMode) {
            case 0:                                                                 // Draw mode 0 : direct plot.
                if (isSet) {
                    *p |= bitDraw;
                } else {
                    *p &= bitMask;
                }
                break;
        }
    }
    //         case 1:                                                                 // Draw mode 1 : and with current pixels
    //         case 2:                                                                 // Draw mode 2 : or with current pixels
    //         case 3:                                                                 // Draw mode 3 : xor with current pixels
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
