// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      read.c
//      Purpose :   Read characters from display
//      Date :      14th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

/**
 * @brief      Read the character at text position (x,y)
 *
 * @param[in]  x     x character position
 * @param[in]  y     y character position
 *
 * @return     character code, of 0 if cannot be recognised.
 */
uint8_t VDURead(uint8_t x,uint8_t y) {
    int16_t c1,c2;                                                                  // The two colours found while scanning,c1 is background,c2 is foreground.
    DVIMODEINFO *dmi = DVIGetModeInformation();
    uint8_t charDef[8];                                                             // Character bitmap.
    x = x * vc.textWidth;                                                           // These are now pixel positions.
    y = y * vc.textHeight; 
    c1 = VDUAReadPixel(x,dmi->height-1-(y+7),true);                                 // The background pixel is *probably* the first on the bottom row.
    c2 = -1;                                                                        // Foreground is unknown.
    for (int y1 = 0;y1 < 8;y1++) {
        charDef[y1] = 0;
        uint32_t yPixel = dmi->height-1-y-(vc.textHeight == 16 ? y1*2 : y1);        // Work out the line allowing for double height text.
        for (int x1 = 0;x1 < 8;x1++) {
            int16_t c = VDUAReadPixel(x+x1,yPixel,true);                            // Read the pixel.
            if (c != c1 && c != c2) {                                               // Not backgound/foreground (or undefined)
                if (c2 >= 0) return 0;                                              // If foreground defined 3 colours so cannot be a text character, graphics.
                c2 = c;                                                             // Use it as foreground
            }
            if (c == c2) charDef[y1] |= (0x80 >> x1);                               // Set bit if foreground.
        }
        //LOG("%08b",charDef[y1]);
    }
    // Compare
    // If doesn't match, flip and compare.
    return 1;
}
