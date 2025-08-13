// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      ellipse.c
//      Purpose :   Ellipses
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "alt_graphics_module.h"
#include "alt_graphics_module_local.h"

static void _GFXFramePart(int x,int y);
static void _GFXLinePart(int x,int y);
static void _GFXDrawEllipse(bool fill);

static int32_t rx,ry,xc,yc;

/**
 * @brief      Draw an ellipse in a bounding box
 *
 * @param[in]  x0    The x0 coordinate
 * @param[in]  y0    The y0 coordinate
 * @param[in]  x1    The x1 coordinate
 * @param[in]  y1    The y1 coordinate
 * @param[in]  fill  true to fill
 */
void GFXDrawEllipse(int32_t x0, int32_t y0, int32_t x1, int32_t y1,bool fill) {
    rx = abs(x0-x1)/2;ry = abs(y0-y1)/2;
    xc = (x0+x1)/2;yc = (y0+y1)/2;
    _GFXDrawEllipse(fill);
}

/**
 * @brief      Draw the frame part of the ellipse
 *
 * @param[in]  x       x Coordinate
 * @param[in]  y       y Coordinate
 */
static void _GFXFramePart(int x,int y) {
    GFXRawMove(xc+x,yc+y);GFXRawPlot(true);
    if (x != 0) {                                                               // If at 0 horizontal only do once
        GFXRawMove(xc-x,yc+y);GFXRawPlot(true);
    }
    if (y != 0) {                                                               // If at 0 vertical only do once.
        GFXRawMove(xc+x,yc-y);GFXRawPlot(true);
        GFXRawMove(xc-x,yc-y);GFXRawPlot(true);
    }
}

/**
 * @brief      Draw the line for the filled ellipse
 *
 * @param[in]  x       x Coordinate
 * @param[in]  y       y Coordinate
 */
static void _GFXLinePart(int x,int y) {
    GFXRawMove(xc-x,yc+y);GFXDrawLine(xc-x,yc+y,xc+x,yc+y,true);
    if (yc != 0) {                                                              // Don't redraw the middle.
        GFXRawMove(xc-x,yc-y);GFXDrawLine(xc-x,yc-y,xc+x,yc-y,true);
    }
}

/**
 * @brief      Midpoint Ellipse Algorithm
 *
 * @param[in]  fill    True if solid fill
 */
static void _GFXDrawEllipse(bool fill) {
    float dx, dy, d1, d2, x, y;
    x = 0;y = ry;
    d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;

    while (dx < dy)
    {
        if (fill) {
            _GFXLinePart(x,y);
        } else {
            _GFXFramePart(x,y);
        }
        if (d1 < 0)
        {
            x++;
            dx = dx + (2 * ry * ry);
            d1 = d1 + dx + (ry * ry);
        } else {
            x++;
            y--;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
        }
    }

    d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) +
         ((rx * rx) * ((y - 1) * (y - 1))) -
          (rx * rx * ry * ry);

    while (y >= 0)
    {
        if (fill) {
            _GFXLinePart(x,y);
        } else {
            _GFXFramePart(x,y);
        }
        if (d2 > 0)
        {
            y--;
            dy = dy - (2 * rx * rx);
            d2 = d2 + (rx * rx) - dy;
        } else {
            y--;
            x++;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d2 = d2 + dx - dy + (rx * rx);
        }
    }
}

