// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      font.c
//      Purpose :   Font functions.
//      Date :      14th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"


/**
 * @brief      Initialise fonts.
 */
void VDUFontInitialise(void) {
    uint8_t *font = DVIGetSystemFont()+96*8;                                        // Preload font 128-255 into UDG Memory.
    memcpy(vc.udgMemory,font,sizeof(vc.udgMemory));
    VDUSetTextSize(1,1);
    VDUResetTextWindow();
}

/**
 * @brief      Get the line data for line y of character c, in l-r bit format
 *
 *             (e.g. 0x80 is the leftmost bit)
 *
 * @param[in]  c     Character 0-255
 * @param[in]  y     Vertical position within character 0-7
 *
 * @return     { description_of_the_return_value }
 */

uint8_t VDUGetCharacterLineData(int c,int y,bool largeFont) {
    c &= 0xFF;
    if (c < ' ' || c == 0x7F) return 0;                                             // Control $00-$1F and $7F
    if (largeFont) {
        return DVIGetSystemFont16()[(c - ' ') * 16+y];                              // ASCII $20-$FF, cannot redefine 8x16
    } else {
        if (c >= 0x80) return vc.udgMemory[(c-0x80)*8+y];                            // UDG $80-$FF
        return DVIGetSystemFont()[(c - ' ') * 8+y];                                  // ASCII $20-$7E ($7F is a control character)
    }
}


/**
 * @brief      Change a UDG definition
 *
 * @param[in]  c      Character ID ($80-$FF)
 * @param      gData  8 bytes of graphic data.
 */
void VDUDefineCharacter(int c,uint8_t *gData) {
    if (c >= 0x80 && c <= 0xFF) {                                                   // Legal UDG
        for (int i = 0;i < 8;i++) {                                                 // Copy into UDG memory
            vc.udgMemory[(c-0x80)*8+i] = gData[i];
        }
    } 
}

