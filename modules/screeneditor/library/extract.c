// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      extract.c
//      Purpose :   Extract line at current position.
//      Date :      15th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "screeneditor_module.h"
#include "screeneditor_module_local.h"

/**
 * @brief      Extract text from line at cursor into buffer, position cursor on
 *             line after end
 *
 * @param      buffer      Buffer
 * @param[in]  bufferSize  Total Buffer size
 *
 * @return     true if successful, false failed (bad character/too long.)
 */
bool SEDExtract(uint8_t *buffer,uint16_t bufferSize) {
    uint8_t x,y;
    uint8_t *bufferStart = buffer;
    VDUWINDOW *tw = VDUGetTextWindow();
    uint8_t lastY = SEDGetEndCurrentLine();                                         // Last Y
    VDUGetTextCursor(&x,&y);                                                        // First Y
    uint8_t firstY = y;
    while (firstY > 0 && VDUHasLineEndMarker(firstY+tw->yTop-1)) firstY--;          // Work backwards until not preceded by extension
    *buffer = '\0';                                                                 // Empty buffer.
    //LOG("%d %d",firstY,lastY);

    for (y = firstY;y <= lastY;y++) {                                               // For Y in order
        for (x = 0;x <= tw->xRight-tw->xLeft;x++) {                                 // For X in order.
            uint8_t ch = VDURead(x,y);                                              // Read character
            if (ch == 0) return false;                                              // Corrupted line.
            if (bufferSize > 1) {                                                   // Space for char and EOS
                *buffer++ = ch;*buffer = '\0';                                      // Copy character in.
            }
        }
    }

    while (buffer != bufferStart && *(buffer-1) == ' ') {                           // Strip ending spaces.
        *(--buffer) = '\0';
    }

    VDUWrite(31);VDUWrite(0);VDUWrite(lastY);VDUWrite(10);                          // Move to last line start and do down.
    return true;
}
