// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      markers.c
//      Purpose :   Markers required for the screen editor
//      Date :      14th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

/**
 * @brief      Reset all the end of line overflow markers.
 */
void VDUResetTextEndMarkers(void) {
    LOG("M:Reset");
    for (int i = 0;i < MAX_HEIGHT;i++) {
        vc.isExtendedLine[i] = false;
    }
}
/**
 * @brief      Scroll the end of line overflow marker array
 *
 * @param[in]  dir   1 down, -1 up.
 */
void VDUScrollTextEndMarkers(int dir) {
    uint8_t y;
    if (dir > 0) {                                                                  // Scroll down, move all down one and top false.
        y = vc.tw.yBottom;
        while (y != vc.tw.yTop) {
            vc.isExtendedLine[y-1] = vc.isExtendedLine[y];
            y--;
        }
        vc.isExtendedLine[vc.tw.yTop] = false;
    } else {                                                                        // Scroll up, move all up one and bottom false
        y = vc.tw.yTop;
        while (y != vc.tw.yBottom) {
            vc.isExtendedLine[y] = vc.isExtendedLine[y+1];
            y++;
        }
        vc.isExtendedLine[vc.tw.yBottom] = false;
    }
    LOG("M:Scroll %d",dir);
}

/**
 * @brief      Set the overflow marker for the given line
 *
 * @param[in]  y     line number
 */
void VDUSetTextEndMarker(int y) {
    LOG("M:Mark %d",y);
    if (y >= 0 && y < MAX_HEIGHT) {
        vc.isExtendedLine[y] = true;
    }
}

/**
 * @brief      Check the overflow marker for the given line
 *
 * @param[in]  y     line number
 *
 * @return     True if overflow marker here.
 */
bool VDUHasLineEndMarker(int y) {
    return (y >= 0 && y < MAX_HEIGHT) ? vc.isExtendedLine[y] : false;
}
