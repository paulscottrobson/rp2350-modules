// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      systemfont.c
//      Purpose :   System Text Font Wrapper
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include <graphics_module.h>
#include <graphics_module_local.h>

/**
 * @brief      Get the character definition for a character for the system font
 *             (8x8)
 *
 * @param[in]  code  ASCII code
 *
 * @return     Character or NULL if not a legal character.
 */
GFXFONTSOURCE *GFXGetSystemCharacter(uint16_t code) {
    static GFXFONTSOURCE ch;                                                        // Information structure
    if (code < ' ' || code >= 0x100) return NULL;                                   // Bad character
    ch.pixelData = DVIGetSystemFont()+(code - ' ') * 8;                             // Address in above byte array
    ch.bytesPerLine = 1;                                                            // 8x8 pixels, so 1 byte per line 
    ch.height = ch.width = 8;                                                       // 8x8 pixels
    ch.xOffset = ch.yOffset = 0;                                                    // No H/V offset.
    return &ch;
}
