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
 
#include "alt_graphics_module.h"
#include "alt_graphics_module_local.h"

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
    if (y0 == y1 && drawLastPixel != 0) {                                           // Horizontal line.        
        if (!inDrawingVert) {                                                       // Out of window vertically, nothing to draw
            GFXRawMove(x1,y1);                                                      // so position and exit
            return;    
        }                            
        if (_GFXInClipWindow(x0) && _GFXInClipWindow(x1)) {                         // Entirely in clipped area ?
                if (abs(x0-x1) >= 10) {                                             // If there is sufficient to make it worth using.
                    SORT_PAIR(x0,x1);                                               // Sort x coordinates into order.
                    GFXOptimisedHorizontalLine(x0,x1,y1,true);
                    GFXRawMove(x1,y1);
                    return;
                }
        }
    }

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

