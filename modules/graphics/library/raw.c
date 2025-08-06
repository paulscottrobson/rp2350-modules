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
static uint8_t *pl[DVI_MAX_BITPLANES];                                              // Bitplane pointers.
bool    inDrawingVert,inDrawingHoriz;                                               // True if currently drawing (e.g. as move, drawing occurs)

static void GFXRawPlot8(bool useFgr);
static void GFXRawPlot64(bool useFgr);

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
    } else {
        offset = (x >> 2) + y * modeInfo.bytesPerLine;
        bitDraw = (0xC0 >> (2*(x & 3)));
    }
    for (int i = 0;i < modeInfo.bitPlaneCount;i++) {                                // Initialise the bitplane position pointers.
        pl[i] = modeInfo.bitPlane[i] + offset;
    }
}    

/**
 * @brief      Draw a pixel in the current position
 *
 * @param[in]  useFgr  true foreground colour, false, background colour.
 */
void GFXRawPlot(bool useFgr) {
    if (!inDrawingVert || !inDrawingHoriz) return;                                  // Cannot plot as we are off the screen.
    if (modeInfo.bitPlaneDepth == 1) {
        GFXRawPlot8(useFgr);
    } else {
        GFXRawPlot64(useFgr);        
    }
}

/**
 * @brief      Raw plotting for 8 colour mode
 *
 * @param[in]  useFgr  true foreground colour, false, background colour.
 */
static void GFXRawPlot8(bool useFgr) {
    uint8_t colour = useFgr ? draw->foreground:draw->background;                    // Drawing colour.    
    for (int plane = 0;plane < modeInfo.bitPlaneCount;plane++) {
        uint8_t *p = pl[plane];                                                     // Byte we are working on.
        bool isSet = (colour & (1 << plane)) != 0;                                  // Is the pixel set on this plane, or not ?
        switch(draw->drawMode) {
            case 0:                                                                 // Draw mode 0 : direct plot.
                if (isSet) {
                    *p |= bitDraw;
                } else {
                    *p &= (~bitDraw);
                }
                break;
            case 1:                                                                 // Draw mode 1, AND with current pixel
                if (!isSet) {                                                       // Clear bitplane if pixel not set.
                    *p &= (~bitDraw);
                }
                break;
            case 2:                                                                 // Draw mode 2, OR with current pixel
                if (isSet) {                                                        // Set bitplane if pixel set.
                    *p |= bitDraw;
                }
                break;
            case 3:                                                                 // Draw mode 3, XOR with current pixel
                if (isSet) {                                                        // Toggle bitplane if pixel set.
                    *p ^= bitDraw;
                }
                break;
        }
    }
}

/**
 * @brief      Raw plotting for 64 colour mode
 *
 * @param[in]  useFgr  true foreground colour, false, background colour.
 */
static void GFXRawPlot64(bool useFgr) {
    uint8_t colour = useFgr ? draw->foreground:draw->background;                    // Drawing colour.    
    for (int plane = 0;plane < modeInfo.bitPlaneCount;plane++) {
        uint8_t *p = pl[plane];                                                     // Byte we are working on.
        uint8_t drawBits = (colour >> (plane * 2)) & 3;                               // These are the two colour pixels.
        drawBits = drawBits << (6-2*(draw->x & 3));                                     // Shift into correct place.
        switch(draw->drawMode) {
            case 0:                                                                 // Draw mode 0 : direct plot.
                *p = (*p & (~bitDraw)) | drawBits;
                break;
            case 1:                                                                 // Draw mode 1, AND with current pixel
                *p &= drawBits;
                break;
            case 2:                                                                 // Draw mode 2, OR with current pixel
                *p |= drawBits;
                break;
            case 3:                                                                 // Draw mode 3, XOR with current pixel
                *p ^= drawBits;
                break;
        }
    }
}
/**
 * @brief      Move up. 
 */
void GFXRawUp(void) {
    for (int i = 0;i < modeInfo.bitPlaneCount;i++) {
        pl[i] -= modeInfo.bytesPerLine;
    }
    draw->y--;
    INDRAWVERT();
}

/**
 * @brief      Move down
 */
void GFXRawDown(void) {    
    for (int i = 0;i < modeInfo.bitPlaneCount;i++) {
        pl[i] += modeInfo.bytesPerLine;
    }
    draw->y++;
    INDRAWVERT();
}

/**
 * @brief      Move left
 */
void GFXRawLeft(void) {    
    draw->x--;                                                                   
    if (modeInfo.bitPlaneDepth == 1) {
        bitDraw <<= 1;
        if ((draw->x & 7) == 7) {
            bitDraw = 0x01;
            for (int i = 0;i < modeInfo.bitPlaneCount;i++) pl[i]--;
        }
    } else {
        bitDraw <<= 2;
        if ((draw->x & 3) == 3) {
            bitDraw = 0x03;
            for (int i = 0;i < modeInfo.bitPlaneCount;i++) pl[i]--;
        }
    }
    INDRAWHORIZ();
}

/**
 * @brief      Move right
 */
void GFXRawRight(void) {   
    draw->x++;                                                                   
    if (modeInfo.bitPlaneDepth == 1) {
        bitDraw >>= 1;
        if ((draw->x & 7) == 0) {
            bitDraw = 0x80;
            for (int i = 0;i < modeInfo.bitPlaneCount;i++) pl[i]++;
        }
    } else {
        bitDraw >>= 2;
        if ((draw->x & 3) == 0) {
            bitDraw = 0xC0;
            for (int i = 0;i < modeInfo.bitPlaneCount;i++) pl[i]++;
        }
    }
    INDRAWHORIZ();
}

/**
 * @brief      Draws a desktop pattern, either black and white speckles or grey if the mode supports it.
 */
void GFXDrawDesktop(void) {
    uint8_t pattern = 0xAA;
    for (int y = 0;y < modeInfo.height;y++) {
        if (modeInfo.bitPlaneDepth == 2) pattern = 0x55;
        for (int plane = 0;plane < modeInfo.bitPlaneCount;plane++) {
            memset(modeInfo.bitPlane[plane]+y*modeInfo.bytesPerLine,pattern,modeInfo.bytesPerLine);
        }
        pattern = pattern ^ 0xFF;
    }
}
