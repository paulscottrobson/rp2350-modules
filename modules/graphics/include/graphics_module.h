// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      graphics_module.h
//      Purpose :   Graphics module header.
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "common_module.h"
#include "dvi_module.h"

enum GFXCommand :uint32_t {
    Mode = 0,                                                                       // Set Graphics Mode to x
    RawColour = 1,                                                                  // Set raw colours to x,y
    Scaling = 3,                                                                    // Set font scaling to x,y
    SetClip = 4,                                                                    // Set current clip (pointer)
    NoClip = 5,                                                                     // Remove clipping.
    SetMapper = 6,                                                                  // Set mapper function (pointer)
    DrawMode = 7,                                                                   // Set drawing mode.

    Move = 20,                                                                      // Move cursor
    Plot = 21,                                                                      // Plot pixel
    Line = 22,                                                                      // Line from last position
    Rect = 23,                                                                      // Frame 
    FillRect = 24,                                                                  // Solid rectangle
    Ellipse = 25,                                                                   // Ellipse outline
    FillEllipse = 26,                                                               // Filled Ellipse
    Triangle = 27,                                                                  // Triangle outline.
    FillTriangle = 28,                                                              // Filled triangle
    Character = 29,                                                                 // Character.
    CharExtent = 30,                                                                // Character extent.
    Clear = 31,                                                                     // Clear screen to background
    ClearWindow = 32,                                                               // Clear the clip window to background
    Desktop = 33                                                                    // Clear screen to desktop
};

void GFXInitialise(void);
//
//      At present the commands that require a pointer should use GFXDrawP(). This will always be available
//      but I'm looking up same identifier/different signature calls in C. I really didn't want the old
//      Windows thing of "casting pointers to LONG" and the runtime is 64bit.
//
uint32_t GFXDraw(enum GFXCommand cmd,uint32_t x,uint32_t y);
uint32_t GFXDrawP(enum GFXCommand cmd,void *p,uint32_t y);

bool GFXOpenContext(void);              
bool GFXCloseContext(void);

typedef void *(*GFXMAPPER)(uint32_t *x,uint32_t *y);                                // Coordinate Mapper type

//
//      The rendering information for a single monochrome character. initially we just support simple 8x8 fonts, but this
//      should be expandable to any monochrome font.
//
typedef struct FontSource {
    uint8_t     *pixelData;                                                         // Pointer to the pixel data for this data.
    uint8_t     bytesPerLine;                                                       // How many 8 bit bytes per horizontal line of character
    uint8_t     height;                                                             // Height of character in pixels
    uint8_t     width;                                                              // Width of character in pixels (e.g. spacing)
    int8_t      xOffset,yOffset;                                                    // The offset of this character from the draw point (descenders etc)
} GFXFONTSOURCE;

typedef GFXFONTSOURCE *(*GFXFONTSOURCEFUNCTION)(uint16_t code);                     // Function that gets font data

#define COL_BLACK       (0)                                                         // RGGB colours supported by 16 and 256 colour modes.
#define COL_DARKBLUE    (1)
#define COL_DARKGREEN   (2)
#define COL_BLUE        (3)
#define COL_GREEN       (4)
#define COL_TEAL        (5)
#define COL_LIGHTGREEN  (6)
#define COL_CYAN        (7)
#define COL_RED         (8)
#define COL_MAGENTA     (9)
#define COL_ORANGE      (10)
#define COL_PINK        (11)
#define COL_GOLD        (12)
#define COL_PLUM        (13)
#define COL_YELLOW      (14)
#define COL_WHITE       (15)