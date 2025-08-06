// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      rect.c
//      Purpose :   Rectangles
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

/**
 * @brief      Draw a rectangle
 *
 * @param[in]  x0    The x0 coordinate
 * @param[in]  y0    The y0 coordinate
 * @param[in]  x1    The x1 coordinate
 * @param[in]  y1    The y1 coordinate
 * @param[in]  fill  true to fill
 */
void GFXDrawRectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1,bool fill) {
    SORT_PAIR(x0,x1);SORT_PAIR(y0,y1);
    GFXDrawLine(x0,y0,x1,y0,true);                                                  // Top line.
    if (y0 == y1) return;                                                           // 1 pixel high
    GFXRawMove(x0,y1);GFXDrawLine(x0,y1,x1,y1,true);                                // Bottom line.
    if (fill) {                                                                     // Fill code
        for (int32_t y = y0+1;y < y1;y++) {
            GFXRawMove(x0,y);GFXDrawLine(x0,y,x1,y,true);
        }
    } else {                                                                        // Outline code
        GFXRawMove(x0,y0+1);GFXDrawLine(x0,y0+1,x0,y1-1,true);                      // Draw the sides.
        GFXRawMove(x1,y1+1);GFXDrawLine(x1,y0+1,x1,y1-1,true);
    }

}
