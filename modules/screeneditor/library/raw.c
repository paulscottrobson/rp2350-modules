// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      raw.c
//      Purpose :   Screen Editor Raw I/O
//      Date :      9th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "screeneditor_module.h"
#include "screeneditor_module_local.h"

/**
 * @brief      Clear the screen.
 */
void SEDClear(void) {
    memset(sedInfo.charMem,0,SED_MAXWIDTH*SED_MAXHEIGHT);                           // Clear the screen
    memset(sedInfo.extendLine,0,SED_MAXHEIGHT);                                     // Reset the extend line flags.
    SEDRepaint();
}

/**
 * @brief      Repaint the screen.
 */
void SEDRepaint(void) {
    for (uint32_t x = 0;x < sedInfo.width;x++) {
        for (uint32_t y = 0;y < sedInfo.height;y++) {
            SEDDraw(x,y,*SEDCharAccess(x,y));
        }
    }
}

/**
 * @brief      Access character memory for a particular cell.
 *
 * @param[in]  x     x offset
 * @param[in]  y     y offset
 *
 * @return     Pointer to byte holding character
 */
uint8_t *SEDCharAccess(uint32_t x,uint32_t y) {
    return sedInfo.charMem + x + y * SED_MAXWIDTH;
}

/**
 * @brief      Draw character in a particular cell.
 *
 * @param[in]  x     x position (offset from left)
 * @param[in]  y     y position (offset from top)
 * @param[in]  ch    character code
 */
void SEDDraw(uint32_t x,uint32_t y,uint8_t ch) {
    if (ch < 32 || ch > 127) ch = 32;                                               // Force into range.

    ch = (x + y * 16) % 96 + 32;

    DVIMODEINFO *m = DVIGetModeInformation();                                       // Get current mode information.
    uint8_t *fontData = DVIGetSystemFont() + (ch - ' ') * 8;                        // Access font data.
    
    if (m->bitPlaneDepth == 1) {                                                    // 8 colours

    } else {                                                                        // 64 colours.

    }
}