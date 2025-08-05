/**
 * @file       dvi_common.h
 *
 * @brief      Header file, DVI interface
 *
 * @author     Paul Robson
 *
 * @date       07/01/2025
 *
 */

#pragma once
//
//      Structure for mode information.
//
#define DVI_MODE_640_240_8      (0)                                                 // Modes
#define DVI_MODE_320_240_8      (1)
#define DVI_MODE_640_480_2      (2)
#define DVI_MODE_320_240_64     (3)            
#define DVI_MODE_320_256_8      (4)                                                 // Trick mode to get BBC Basic compatible 32 text rows.
#define DVI_MODE_640_480_8      (5)

#define DVI_MODE_COUNT          (6)
//
//      DVI Mode information structure
//
#define DVI_MAX_BITPLANES   (8)                                                     // Not very likely, but you never know.

struct DVIModeInformation {
    int mode;                                                                       // Current Mode.
    int width,height;                                                               // Size of screen
    int bitPlaneCount;                                                              // Number of bitPlanes
    int bitPlaneDepth;                                                              // Bitplane depth in bits.
    int bytesPerLine;                                                               // Bytes per line of display.
    uint8_t *bitPlane[DVI_MAX_BITPLANES];                                           // Up to 8 bitplanes    
    int bitPlaneSize;                                                               // Byte size of each bitplane.
};

void DVISetMonoColour(int fg, int bg);
bool DVISetMode(int mode);
struct DVIModeInformation *DVIGetModeInformation(void);
int  DVIGetScreenExtent(int *pWidth,int *pHeight);

