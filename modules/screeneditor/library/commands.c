// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      commands.c
//      Purpose :   Extended keys.
//      Date :      15th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "screeneditor_module.h"
#include "screeneditor_module_local.h"

/**
 * @brief      Insert a space in the extended line.
 */
void SEDInsert(void) {
    uint8_t lastY = SEDGetEndCurrentLine();
    LOG("Last %d",lastY);
}

/**
 * @brief      Delete a character in the extended line.
 */
void SEDDelete(void) {
    uint8_t lastY = SEDGetEndCurrentLine();
    uint8_t x,y;
    VDUGetTextCursor(&x,&y);
    VDUWINDOW *tw = VDUGetTextWindow();
    uint8_t width = tw->xRight-tw->xLeft;                                           // From current position, till end of line (inc extensions)
    while (x != width || y != lastY) {
        uint8_t xNext = x+1,yNext = y;                                              // Next character
        if (xNext > width) xNext = 0,yNext++;   
        VDUCopyChar(xNext+tw->xLeft,yNext+tw->yTop,x+tw->xLeft,y+tw->yTop);         // Copy next character here
        x = xNext;y = yNext;
    }
    VDURenderCharacter(width+tw->xLeft,lastY+tw->yTop,' ');                         // And blank the last character slot.
}

/**
 * @brief      Get the last line of the current extended line
 *
 * @return     Line number of the last line.
 */
uint8_t SEDGetEndCurrentLine(void) {
    VDUWINDOW *tw = VDUGetTextWindow();
    uint8_t x,y;
    VDUGetTextCursor(&x,&y);
    while (y+tw->yTop < tw->yBottom &&                                              // While more screen and ext line marker, go down.
           VDUHasLineEndMarker(y+tw->yTop)) y++;
    return y;
}