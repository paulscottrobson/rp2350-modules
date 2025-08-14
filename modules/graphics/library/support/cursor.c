// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      cursor.c
//      Purpose :   Cursor functions.
//      Date :      14th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

static bool cursorIsVisible = false;                                                // Cursor visible
static bool cursorIsEnabled = true;

/**
 * @brief      Draw or Erase the cursor
 *
 * @param[in]  isVisible  true if cursor to be drawn.
 */
static void _VDUDrawCursor(bool isVisible) {
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


/**
 * @brief      Disable showing of cursor.
 */
void VDUDisableCursor(void) {
    VDUHideCursor();
    cursorIsEnabled = false;
}


/**
 * @brief      Enable showing of cursor.
 */
void VDUEnableCursor(void) {
    cursorIsEnabled = true;
}


/**
 * @brief      Hide cursor if displayed.
 */
void VDUHideCursor(void) {
    if (cursorIsVisible) {
        _VDUDrawCursor(false);
        cursorIsVisible = false;
    }
}
/**
 * @brief      Show cursor if not already visible
 */
void VDUShowCursor(void) {
    if (!cursorIsVisible && cursorIsEnabled) {
        _VDUDrawCursor(true);
        cursorIsVisible = true;
    }
}

/**
 * @brief      Home cursor to top left of current window
 */
void VDUHomeCursor(void) {
    VDUSetTextCursor(0,0);
}

/**
 * @brief      Set the cursor position in the text window
 *
 * @param[in]  x     Horizontal character position
 * @param[in]  y     Vertical character position
 */
void VDUSetTextCursor(uint8_t x,uint8_t y) {
    if (x >= 0 && y >= 0 && x <= vc.tw.xRight-vc.tw.xLeft && y <= vc.tw.yBottom-vc.tw.yTop) {
        vc.xCursor = x;vc.yCursor = y;
    }
}


/**
 * @brief      Read the text cursor position
 *
 * @param      x     pointer to x store or NULL
 * @param      y     pointer to y store or NULL
 */
void VDUGetTextCursor(uint8_t *x, uint8_t *y) {
    if (x != NULL) *x = vc.xCursor;
    if (y != NULL) *y = vc.yCursor;
}
