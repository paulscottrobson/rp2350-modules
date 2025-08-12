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

#include "screen_module.h"
#include "screen_module_local.h"

/**
 * @brief      Clear the screen.
 */
void SCRClear(void) {
    memset(scrInfo.charMem,' ',SCR_MAXWIDTH*SCR_MAXHEIGHT);                         // Clear the screen
    memset(scrInfo.extendLine,0,SCR_MAXHEIGHT);                                     // Reset the extend line flags.
    SCRRepaint();
}

/**
 * @brief      Repaint the screen.
 */
void SCRRepaint(void) {
    for (uint32_t x = 0;x < scrInfo.width;x++) {
        for (uint32_t y = 0;y < scrInfo.height;y++) {
            SCRDraw(x,y,scrInfo.colour,*SCRCharAccess(x,y),false);
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
uint8_t *SCRCharAccess(uint32_t x,uint32_t y) {
    return scrInfo.charMem + x + y * SCR_MAXWIDTH;
}

/**
 * @brief      Draw character in a particular cell.
 *
 * @param[in]  x       x position (offset from left)
 * @param[in]  y       y position (offset from top)
 * @param[in]  colour  colour to write in (raw)
 * @param[in]  ch      Character to draw.
 * @param[in]  bHighlight  if true, highlight it by drawing the 'shaded' graphic.
 */
void SCRDraw(uint32_t x,uint32_t y,uint8_t colour,uint32_t ch,bool bHighlight) {

    static uint8_t _pixelMapper[16] = { 0x00,0x03,0x0C,0x0F, 0x30,0x33,0x3C,0x3F,   // Map 4 bit pixels -> byte for 64 bit mode.
                                        0xC0,0xC3,0xCC,0xCF, 0xF0,0xF3,0xFC,0xFF  };

    static uint8_t _colourMapper[4] = { 0x00,0x55,0xAA,0xFF };                      // Colour masks.

    static uint8_t _cursor[8] = { 0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55 };

    if (ch < 32 || ch > 127) ch = 32;                                               // Force into range.

    DVIMODEINFO *m = DVIGetModeInformation();                                       // Get current mode information.
    if (x >= m->width/8 || y >= m->height/8) return;                                // Mode could have changed, this stops out of bounds access.

    uint8_t *fontData = DVIGetSystemFont() + (ch - ' ') * 8;                        // Access font data.
    if (bHighlight) fontData = _cursor;
    uint32_t offset;

    if (m->bitPlaneDepth == 1) {                                                    // 8 colours
        offset = (x + scrInfo.x) + (y + scrInfo.y) * 8 * m->bytesPerLine;           // Offset in each line.
        for (uint32_t p = 0;p < m->bitPlaneCount;p++) {                             // Each bit plane
            uint8_t *pt = m->bitPlane[p] + offset;
            for (uint32_t y = 0;y < 8;y++) {                                        // Each line of character.
                *(pt+y*m->bytesPerLine) = (colour & (1 << p)) ? fontData[y] : 0x00;
            }
        }
    } else {                                                                        // 64 colours.
        uint8_t colourMask[3];
        for (uint32_t i = 0;i < 3;i++) {
            colourMask[i] = _colourMapper[(colour >> (2 * i)) & 3];
        }
        offset = (x + scrInfo.x)*2 + (y + scrInfo.y) * 8 * m->bytesPerLine;         // Offset in each line.
        for (uint32_t p = 0;p < m->bitPlaneCount;p++) {                             // Each bit plane
            uint8_t *pt = m->bitPlane[p] + offset;
            for (uint32_t y = 0;y < 8;y++) {                                        // Each line of character.
                *(pt+y*m->bytesPerLine) = _pixelMapper[fontData[y] >> 4] & colourMask[p];
                *(pt+y*m->bytesPerLine+1) = _pixelMapper[fontData[y] & 0x0F] & colourMask[p];
            }
        }   
    }
}

