/**
 * @file drawing.c
 *
 * @brief      Extended drawing functions built from the atomic functions
 *             Rectangles and Ellipses
 *
 * @author     Paul Robson
 *
 * @date       29/01/2025
 */

#include "graphics_module.h"
#include "graphics_module_local.h"

/**
 * @brief      Draw a framed rectangle
 *
 * @param[in]  x0      The x0 coordinate
 * @param[in]  y0      The y0 coordinate
 * @param[in]  x1      The x1 coordinate
 * @param[in]  y1      The y1 coordinate
 */
void VDUAFrameRect(int x0,int y0,int x1,int y1) {
    if (y0 > y1) { int n = y0;y0 = y1;y1 = n; }                                     // Sort vertically.
    VDUAHorizLine(x0,x1,y0);                                                 		// Top line.
    if (y1 != y0) VDUAHorizLine(x0,x1,y1);                                   		// Bottom line, if not a one line frame.
    for (int y = y0+1;y < y1;y++) {
        VDUAPlot(x0,y);
        if (x0 != x1) VDUAPlot(x1,y);
    }
}


/**
 * @brief      Draw a solid rectangle
 *
 * @param[in]  x0      The x0 coordinate
 * @param[in]  y0      The y0 coordinate
 * @param[in]  x1      The x1 coordinate
 * @param[in]  y1      The y1 coordinate
 */
void VDUAFillRect(int x0,int y0,int x1,int y1) {
    if (y0 > y1) { int n = y0;y0 = y1;y1 = n; }                                     // Sort vertically.
    for (int y = y0;y <= y1;y++) {
        VDUAHorizLine(x0,x1,y);
    }
}

static int rx,ry,xc,yc;

static void _GFXDrawEllipse(bool fill);
static void _GFXDrawEllipseMain(int x0,int y0,int x1,int y1,bool fill);


/**
 * @brief      Draw an outline ellipse
 *
 * @param[in]  x0      The x0 coordinate
 * @param[in]  y0      The y0 coordinate
 * @param[in]  x1      The x1 coordinate
 * @param[in]  y1      The y1 coordinate
 */
void VDUAFrameEllipse(int x0,int y0,int x1,int y1) {
    _GFXDrawEllipseMain(x0,y0,x1,y1,false);
}

/**
 * @brief      Draw a filled in ellipse.
 *
 * @param[in]  x0      The x0 coordinate
 * @param[in]  y0      The y0 coordinate
 * @param[in]  x1      The x1 coordinate
 * @param[in]  y1      The y1 coordinate
 */
void VDUAFillEllipse(int x0,int y0,int x1,int y1) {
    _GFXDrawEllipseMain(x0,y0,x1,y1,true);
}


/**
 * @brief      Run main draw ellipse
 *
 * @param[in]  x0      The x0 coordinate
 * @param[in]  y0      The y0 coordinate
 * @param[in]  x1      The x1 coordinate
 * @param[in]  y1      The y1 coordinate
 * @param[in]  fill    In fill ?
 */
static void _GFXDrawEllipseMain(int x0,int y0,int x1,int y1,bool fill) {
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
    VDUAPlot(xc+x,yc+y);
    if (x != 0) {                                                               // If at 0 horizontal only do once
        VDUAPlot(xc-x,yc+y);
    }
    if (y != 0) {                                                               // If at 0 vertical only do once.
        VDUAPlot(xc+x,yc-y);
        VDUAPlot(xc-x,yc-y);
    }
}

/**
 * @brief      Draw the line for the filled ellipse
 *
 * @param[in]  x       x Coordinate
 * @param[in]  y       y Coordinate
 */
static void _GFXLinePart(int x,int y) {
    VDUAHorizLine(xc-x,xc+x,yc+y);
    if (yc != 0) {                                                              // Don't redraw the middle.
        VDUAHorizLine(xc-x,xc+x,yc-y);
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

