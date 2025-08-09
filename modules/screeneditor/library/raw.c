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
            SEDDraw(x,y,*SEDCharAccess(x,y),sedInfo.colour,y == sedInfo.yCursor && x == sedInfo.xCursor);
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
 * @param[in]  x           x position (offset from left)
 * @param[in]  y           y position (offset from top)
 * @param[in]  ch          character code
 * @param[in]  colour      colour to write in (raw)
 * @param[in]  bHighlight  if true, highlight it by drawing the 'shaded' graphic.
 */
void SEDDraw(uint32_t x,uint32_t y,uint8_t ch,uint8_t colour,bool bHighlight) {

    static uint8_t _pixelMapper[16] = { 0x00,0x03,0x0C,0x0F, 0x30,0x33,0x3C,0x3F,   // Map 4 bit pixels -> byte for 64 bit mode.
                                        0xC0,0xC3,0xCC,0xCF, 0xF0,0xF3,0xFC,0xFF  };

    static uint8_t _colourMapper[4] = { 0x00,0x55,0xAA,0xFF };                      // Colour masks.

    static uint8_t _cursor[8] = { 0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55 };

    if (ch < 32 || ch > 127) ch = 32;                                               // Force into range.

    ch = (x + y * 16 + 3) % 96 + 32;

    DVIMODEINFO *m = DVIGetModeInformation();                                       // Get current mode information.
    uint8_t *fontData = DVIGetSystemFont() + (ch - ' ') * 8;                        // Access font data.
    if (bHighlight) fontData = _cursor;
    uint32_t offset;

    if (m->bitPlaneDepth == 1) {                                                    // 8 colours
        offset = (x + sedInfo.x) + (y + sedInfo.y) * 8 * m->bytesPerLine;           // Offset in each line.
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
        offset = (x + sedInfo.x)*2 + (y + sedInfo.y) * 8 * m->bytesPerLine;         // Offset in each line.
        for (uint32_t p = 0;p < m->bitPlaneCount;p++) {                             // Each bit plane
            uint8_t *pt = m->bitPlane[p] + offset;
            for (uint32_t y = 0;y < 8;y++) {                                        // Each line of character.
                *(pt+y*m->bytesPerLine) = _pixelMapper[fontData[y] >> 4] & colourMask[p];
                *(pt+y*m->bytesPerLine+1) = _pixelMapper[fontData[y] & 0x0F] & colourMask[p];
            }
        }   
    }
}