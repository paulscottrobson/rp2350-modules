// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      process.c
//      Purpose :   Main processing
//      Date :      15th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "screeneditor_module.h"
#include "screeneditor_module_local.h"

/**
 * @brief      Process a single key.
 *
 * @param[in]  key   key code.
 */
void SEDProcess(uint8_t key) {
    int8_t x,y,h;
    VDUWINDOW *tw = VDUGetTextWindow();

    switch(key) {
        case CTL_CR:                                                                // CR key does CR/LF.
            VDUWrite(13);VDUWrite(10);                                              // Actually we won't process this but useful for testing.
            break;

        case CTL_HOME:                                                              // Home (start of line)
            VDUGetTextCursor(&x,&y);
            VDUWrite(31);VDUWrite(0);VDUWrite(y);
            break; 

        case CTL_END:                                                               // End (end of line)
            VDUGetTextCursor(&x,&y);
            VDUWrite(31);VDUWrite(tw->xRight-tw->xLeft);VDUWrite(y);
            break;

        case CTL_PAGEUP:                                                            // Page up (up half a screen, no scrolling)
            VDUGetTextCursor(&x,&y);
            y = y - (tw->yBottom-tw->yTop)/2+1;if (y < 0) y = 0;
            VDUWrite(31);VDUWrite(0);VDUWrite(y);
            break;

        case CTL_PAGEDOWN:                                                          // Page down (down half a screen, no scrolling)
            VDUGetTextCursor(&x,&y);
            h = tw->yBottom-tw->yTop;
            y = y + (tw->yBottom-tw->yTop)/2+1;if (y > h) y = h;
            VDUWrite(31);VDUWrite(0);VDUWrite(y);
            break;

        case CTL_INSERT:                                                            // Insert, make space in extended line.
            SEDInsert();
            break;

        case CTL_DELETE:                                                            // Delete, delete current character in extended line.
            SEDDelete();
            break;

        case CTL_TAB:                                                               // TAB across to next 8 char stop.
            do {
                VDUWrite(9);
                VDUGetTextCursor(&x,&y);
            } while (x % 8 != 0);
            break;

        case CTL_UP:                                                                // These just do what they do.
        case CTL_DOWN:
        case CTL_LEFT:
        case CTL_RIGHT:
        case CTL_CLEAR:
        case CTL_BACKSPACE:                                                         // Backspace does not delete rest of line..
            VDUWrite(key);
            break;
        default:                                                                    // Anything else ...
            if (key >= ' ') VDUWrite(key);                                          // Ignore any other control keys which might do wierd things.
            break;
    }
}