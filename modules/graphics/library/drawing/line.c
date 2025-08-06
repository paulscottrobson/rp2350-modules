// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      line.c
//      Purpose :   Simple Bresenham Line drawer.
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

static bool _GFXInClipWindow(uint32_t x);

/**
 * @brief      Line drawing (simple Bresenham)
 *
 * @param[in]  x0             The x0 coordinate
 * @param[in]  y0             The y0 coordinate
 * @param[in]  x1             The x1 coordinate
 * @param[in]  y1             The y1 coordinate
 * @param[in]  drawLastPixel  Draw the last pixel.
 */
void GFXDrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1,bool drawLastPixel) {

    GFXRawMove(x0,y0);                                                              // To start, so we can check clipping
    // if (y0 == y1 && !drawLastPixel) {                                               // Horizontal line.
    //     if (!draw->inDrawingVert) {                                                 // Out of window vertically, nothing to draw->
    //         GFXRawMove(x1,y1);return;                                               // Position and exit
    //     }                            
    //     if (_GFXInClipWindow(x0) && _GFXInClipWindow(x1)) {                         // Entirely in clipped area ?
    //             if (abs(x0-x1)/(4*vi.pixelsPerByte) >= 3) {                         // If there is sufficient to make it worth using.
    //                 GFXOptimisedHorizontalLine(x0,x1,y1,true);
    //                 GFXRawMove(x1,y1);
    //                 return;
    //             }
    //     }
    // }

    int32_t dx = abs(x1 - x0);
    int32_t sx = x0 < x1 ? 1 : -1;
    int32_t dy = -abs(y1 - y0);
    int32_t sy = y0 < y1 ? 1 : -1;
    int32_t error = dx + dy;

    GFXRawMove(x0,y0);                                                              // Move drawing cursor

    while(x0 != x1 || y0 != y1) {

        GFXRawPlot(true);                                                           // Draw a pixel.
        int32_t e2 = 2 * error;
        if (e2 >= dy) {
            error = error + dy;
            if (sx < 0) {                 
                GFXRawLeft(); 
            } else { 
                GFXRawRight(); 
            }
            x0 += sx;
        }
        if (e2 <= dx) {
            error = error + dx;
            if (sy < 0) { 
                GFXRawUp(); 
            } else { 
                GFXRawDown();
            }
            y0 += sy;
        }
    }
    if (drawLastPixel) GFXRawPlot(true);                                            // Last pixel
}

/**
 * @brief      Check if an x coordinate is in the clip window
 *
 * @param[in]  x     x coordinate
 *
 * @return     true if in the clip window
 */
static bool _GFXInClipWindow(uint32_t x) {
    return (x >= draw->clip.xLeft && x <= draw->clip.xRight);
}


/**
 * @brief      Draw a horizontal line fast by writing in words where possible.
 *
 * @param[in]  x0             one end of line
 * @param[in]  x1             other end of line
 * @param[in]  y              vertical position
 * @param[in]  useForeground  If true use fgr colour, if false, use bgr colour.
 */
void GFXOptimisedHorizontalLine(int32_t x0, int32_t x1, int32_t y,bool useForeground) {

    // SORT_PAIR(x0,x1);                                                               // Sort x coordinates into order.
    // GFXRawMove(x0,y);
    // int32_t modReqd = 4 * vi.pixelsPerByte;                                         // Modulus of x0 that needs to be zero to be on a word boundary.
    // while (x0 % modReqd != 0) {
    //     x0++;GFXRawPlot(true);GFXRawRight();
    // }
    // int wordsToDo = (x1 - x0) / modReqd;                                            // This is the number of words we can write.
    // if (wordsToDo != 0) {
    //     uint32_t colour = useForeground ? draw->foreground : draw->background;      // Colour to draw in fgr or bgr
    //     if (vi.pixelsPerByte == 2) colour = (colour & 0x0F) * 0x11;                 // Expand to a byte,
    //     if (vi.pixelsPerByte == 4) colour = (colour & 0x03) * 0x55;
    //     if (vi.pixelsPerByte == 8) colour = (colour & 0x01) ? 0xFF:0x00;

    //     colour = colour | (colour << 8) | (colour << 16) | (colour << 24);          // Replicate each byte through the 32 bit word.
    //     for (int i = 0;i < wordsToDo;i++) {
    //         x0 += modReqd;   
    //         GFXRawWordRight(colour);
    //     }
    // }

    // while (x0 < x1) {                                                               // Complete any leftover pixels.
    //     x0++;GFXRawPlot(true);GFXRawRight();        
    // }
}

