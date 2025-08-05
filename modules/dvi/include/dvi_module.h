// *******************************************************************************************
// *******************************************************************************************
//
//      Name :       dvi_module.h
//      Purpose :    DVI Module external
//      Date :       5th August 2025
//      Author :     Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

//
//      DVI Mode information structure
//
#define DVI_MAX_BITPLANES   (8)                                                     // Not very likely, but you never know.

typedef struct _DVIModeInformation {
    uint32_t mode;                                                                  // Current Mode.
    uint32_t width,height;                                                          // Size of screen
    uint32_t bitPlaneCount;                                                         // Number of bitPlanes
    uint32_t bitPlaneDepth;                                                         // Bitplane depth in bits.
    uint32_t bytesPerLine;                                                          // Bytes per line of display.
    uint8_t *bitPlane[DVI_MAX_BITPLANES];                                           // Up to 8 bitplanes    
    uint32_t bitPlaneSize;                                                          // Byte size of each bitplane.
} DVIMODEINFO;

//
//      Structure for mode information.
//
typedef enum _DVIMode {
    MODE_640_240_8 = 0,
    MODE_320_240_8 = 1,
    MODE_320_240_64 = 2,
    MODE_320_256_8 = 3,
    MODE_640_480_8 = 4
} DVIMODE;

void DVIInitialise(void);
bool DVISetMode(DVIMODE mode);
DVIMODEINFO *DVIGetModeInformation(void);

