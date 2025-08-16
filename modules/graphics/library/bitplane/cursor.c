// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      cursor.c
//      Purpose :   Cursor drawing.
//      Date :      16th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

/**
 * @brief      Draw or Erase the cursor
 *
 * @param[in]  isVisible  true if cursor to be drawn.
 */
void VDUDrawCursor(bool isVisible) {
    DVIMODEINFO *dmi = DVIGetModeInformation();            
    bool is64Bit = (dmi->bitPlaneDepth == 2);
    for (int plane = 0;plane < dmi->bitPlaneCount;plane++) {        
        uint8_t *p = dmi->bitPlane[plane] + (dmi->bytesPerLine * vc.textHeight * (vc.yCursor+vc.tw.yTop)) + ((vc.xCursor+vc.tw.xLeft) * (is64Bit ? 2 : 1));
        for (int y = 0;y < vc.textHeight;y++) {
            *p ^= 0xFF;if (is64Bit) *(p+1) ^= 0xFF;
            p += dmi->bytesPerLine;
        }
    }
}


