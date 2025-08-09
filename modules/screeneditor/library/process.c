// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      process.c
//      Purpose :   Process keys.
//      Date :      9th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "screeneditor_module.h"
#include "screeneditor_module_local.h"

/**
 * @brief      Process a key. If ENTER then fill the buffer with the current
 *             line, otherwise handle it accordingly.
 *
 * @param      buffer  Buffer for line.
 * @param[in]  size    Buffer size in bytes
 * @param[in]  key     Any key to process, or zero if no key available.
 *
 * @return     continue : false if either the Runtime App has stopped *or* ENTER
 *             was pressed.
 */
bool SEDProcess(uint8_t *buffer,uint32_t size,uint32_t key) {
    *buffer = '\0';                                                                 // Empty buffer in case we exit runtime.
    if (key == 0) {                                                                 // No key, flash the cursor.
        bool showCursor = (COMClock() & 512) != 0;
        SEDDraw(sedInfo.xCursor,sedInfo.yCursor,showCursor ? sedInfo.cursorColour : sedInfo.colour ,showCursor);
        return COMAppRunning();
    }
    SEDDraw(sedInfo.xCursor,sedInfo.yCursor,sedInfo.colour,false);                  // Hide the cursor

    return COMAppRunning() && key != CTL_CRLF;
}

