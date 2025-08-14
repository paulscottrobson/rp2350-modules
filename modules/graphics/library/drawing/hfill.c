// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      hfill.c
//      Purpose :   Horizontal fill background code.
//      Date :      13th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************

#include "graphics_module.h"
#include "graphics_module_local.h"

static bool _VDUHIsBackground(int px,int py);
static void _VDUHorizontalFill(int x,int y,int dx);

static int background;

/**
 * @brief      Horizontal flood fill to non-background colour
 *
 * @param[in]  x          x physical coordinate
 * @param[in]  y          y physical coordinate
 * @param[in]  rightOnly  do right only.
 */
void VDUHorizontallFill(int x,int y,bool rightOnly) {
    background = VDUGetBackgroundColour();                                          // Get the background colour.
    if (!_VDUHIsBackground(x,y)) return;                                            // the pixel has to be non background.
    _VDUHorizontalFill(x,y,1);                                                      // Fill right
    if (!rightOnly) _VDUHorizontalFill(x,y,-1);                                     // Fill left.
    VDUAPlot(x,y);                                                                  // Draw the middle pixel.
}

/**
 * @brief      Check if this position contains a background pixel.
 *
 * @param[in]  px    physical x
 * @param[in]  py    physical y
 *
 * @return     true if background
 */
static bool _VDUHIsBackground(int px,int py) {
    int c = VDUAReadPixel(px,py,false);                                             // Read the pixel at that location
    if (c < 0) return false;                                                        // Off the edge.
     return (c == background);
}

/**
 * @brief      Horizontal fill in current foreground colour
 *
 * @param[in]  x     x physical
 * @param[in]  y     y physical
 * @param[in]  dx    direction
 */
static void _VDUHorizontalFill(int x,int y,int dx) {
    x += dx;                                                                        // Skip the start.
    while (_VDUHIsBackground(x,y)) {                                                // While still background
        VDUAPlot(x,y);
        x += dx;  
    }
}

