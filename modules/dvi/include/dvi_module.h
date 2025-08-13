// *******************************************************************************************
// *******************************************************************************************
//
//      Name :       dvi_module.h
//      Purpose :    DVI Module external
//      Date :       7th August 2025
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
#include <common_module.h>
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

#define DVI_MODE_COUNT      (5)                                                     // Supported DVI modes.

void DVIInitialise(void);
bool DVISetMode(DVIMODE mode);
DVIMODEINFO *DVIGetModeInformation(void);
uint32_t  DVIGetScreenExtent(uint32_t *pWidth,uint32_t *pHeight);
uint8_t * DVIGetSystemFont(void);

#define FRAME_WIDTH 640                                                             // Not the *pixels*, it's the display setting.
#define FRAME_HEIGHT 480
#define PLANE_SIZE(x,y) ((x) * (y) / 8)                                             // Memory usage one bitplane x by y
#define VIDEO_BYTES (PLANE_SIZE(FRAME_WIDTH,FRAME_HEIGHT) * 3)

extern uint8_t framebuf[VIDEO_BYTES];
