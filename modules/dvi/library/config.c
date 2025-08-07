// *******************************************************************************************
// *******************************************************************************************
//
//      Name :       config.c
//      Purpose :    Pico DVI Configuration
//      Date :       7th August 2025
//      Author :     Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "dvi_module.h"

DVIMODEINFO dvi_modeInfo;                                                           // Mode information structure.

/**
 * @brief      Get mode information
 *
 * @return     Pointer to a mode information structure
 */
DVIMODEINFO *DVIGetModeInformation(void) {
    return &dvi_modeInfo;
}

/**
 * @brief      Set current mode
 *
 * @param[in]  mode  The mode to set.
 *
 * @return     0 if switched ok.
 */
bool DVISetMode(DVIMODE mode) {
    bool supported = true;
    dvi_modeInfo.mode = mode;                                                       // Record mode

    switch(mode) {

        case MODE_640_480_8:                                                        // 640x480x8 information if enabled.
            dvi_modeInfo.width = 640;dvi_modeInfo.height = 480;
            dvi_modeInfo.bitPlaneCount = 3;
            dvi_modeInfo.bitPlaneSize = PLANE_SIZE(640,480);
            dvi_modeInfo.bitPlaneDepth = 1;
            for (int i = 0;i <dvi_modeInfo.bitPlaneCount;i++)
                dvi_modeInfo.bitPlane[i] = framebuf + dvi_modeInfo.bitPlaneSize * i;
            dvi_modeInfo.bytesPerLine = dvi_modeInfo.width / 8;
        break;

        case MODE_640_240_8:                                                        // 640x240x8 information.
            dvi_modeInfo.width = 640;dvi_modeInfo.height = 240;
            dvi_modeInfo.bitPlaneCount = 3;
            dvi_modeInfo.bitPlaneSize = PLANE_SIZE(640,240);
            dvi_modeInfo.bitPlaneDepth = 1;
            for (int i = 0;i <dvi_modeInfo.bitPlaneCount;i++)
                dvi_modeInfo.bitPlane[i] = framebuf + dvi_modeInfo.bitPlaneSize * i;
            dvi_modeInfo.bytesPerLine = dvi_modeInfo.width / 8;
        break;

        case MODE_320_240_8:                                                        // 320x240x8 information.
            dvi_modeInfo.width = 320;dvi_modeInfo.height = 240;
            dvi_modeInfo.bitPlaneCount = 3;
            dvi_modeInfo.bitPlaneSize = PLANE_SIZE(320,240);
            dvi_modeInfo.bitPlaneDepth = 1;
            for (int i = 0;i <dvi_modeInfo.bitPlaneCount;i++)
                dvi_modeInfo.bitPlane[i] = framebuf + dvi_modeInfo.bitPlaneSize * i;
            dvi_modeInfo.bytesPerLine = dvi_modeInfo.width / 8;
        break;

        case MODE_320_256_8:                                                        // 320x256x8 information.
            dvi_modeInfo.width = 320;dvi_modeInfo.height = 256;
            dvi_modeInfo.bitPlaneCount = 3;
            dvi_modeInfo.bitPlaneSize = PLANE_SIZE(320,256);
            dvi_modeInfo.bitPlaneDepth = 1;
            for (int i = 0;i <dvi_modeInfo.bitPlaneCount;i++)
            dvi_modeInfo.bitPlane[i] = framebuf + dvi_modeInfo.bitPlaneSize * i;
            dvi_modeInfo.bytesPerLine = dvi_modeInfo.width / 8;
        break;

        case MODE_320_240_64:                                                       // 320x240x64 information.
            dvi_modeInfo.width = 320;dvi_modeInfo.height = 240;
            dvi_modeInfo.bitPlaneCount = 3;
            dvi_modeInfo.bitPlaneSize = PLANE_SIZE(320,240)*2;
            dvi_modeInfo.bitPlaneDepth = 2;
            for (int i = 0;i <dvi_modeInfo.bitPlaneCount;i++)
            dvi_modeInfo.bitPlane[i] = framebuf + dvi_modeInfo.bitPlaneSize * i;
            dvi_modeInfo.bytesPerLine = dvi_modeInfo.width / 4;        
            break;
        default:
            supported = false;
            dvi_modeInfo.mode = -1;                                                 // Failed.
            break;
        }
    return supported;
}
