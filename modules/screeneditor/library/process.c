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

static void SEDProcessKey(uint32_t key);
static void SEDAdjust(int32_t xi,int32_t yi);
static void SEDBackSpace(void);
static void SEDCarriageReturn(void);

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

    if (key == CTL_CRLF) {                                                          // If CR load the current line in.

    }
    SEDProcessKey(key); 
    return COMAppRunning() && key != CTL_CRLF;
}

/**
 * @brief      Process a key stroke
 *
 * @param[in]  key   key to process
 */
static void SEDProcessKey(uint32_t key) {
    switch(key) {
        case CTL_TAB:
            sedInfo.xCursor = ((sedInfo.xCursor + 8) & 0xFFF8);
            SEDAdjust(0,0);
            break;
        case CTL_LEFT:
            SEDAdjust(-1,0);break;
        case CTL_RIGHT:
            SEDAdjust(1,0);break;
        case CTL_UP:
            SEDAdjust(0,-1);break;
        case CTL_DOWN:
            SEDAdjust(0,1);break;
        case CTL_PAGEUP:
            SEDAdjust(0,-sedInfo.height*2/3);break;
        case CTL_PAGEDOWN:
            SEDAdjust(0,sedInfo.height*2/3);break;
        case CTL_HOME:
            sedInfo.xCursor = 0;break;
        case CTL_END:
            sedInfo.xCursor = sedInfo.width-1;break;
        case CTL_BACKSPACE:
            SEDBackSpace();break;
        case CTL_CRLF:
            SEDCarriageReturn();break;
        case CTL_CLEAR:
            SEDClear();
            sedInfo.xCursor = sedInfo.yCursor = 0;            
            break;
        default:
            if (key >= 0x20 && key <= 0x7F) {
                *SEDCharAccess(sedInfo.xCursor,sedInfo.yCursor) = key;
                SEDDraw(sedInfo.xCursor,sedInfo.yCursor,sedInfo.colour,false);
                if (sedInfo.xCursor == sedInfo.width-1) {                           // Have we done a multiple line ?
                    sedInfo.extendLine[sedInfo.yCursor] = 1;                        // Set the 'extended line' flag.
                }
                SEDAdjust(1,0);
            }
            break;
    }
    // TODO: Check for scroll.
}

/**
 * @brief      Adjust position and fix up.
 *
 * @param[in]  xi    x offset
 * @param[in]  yi    y offset
 */
static void SEDAdjust(int32_t xi,int32_t yi) {
    sedInfo.xCursor += xi;
    sedInfo.yCursor += yi;
    if (sedInfo.xCursor < 0) {
        sedInfo.xCursor = sedInfo.width-1;
        sedInfo.yCursor--;
    }
    if (sedInfo.xCursor >= sedInfo.width) {
        sedInfo.xCursor = 0;
        sedInfo.yCursor++;
    }
    if (sedInfo.yCursor < 0) sedInfo.yCursor = 0;
    if (sedInfo.yCursor >= sedInfo.height) sedInfo.yCursor = sedInfo.height-1;
}
    
/**
 * @brief      Do a backspace.
 */
static void SEDBackSpace(void) {
    SEDAdjust(-1,0);
    *SEDCharAccess(sedInfo.xCursor,sedInfo.yCursor) = ' ';
    SEDDraw(sedInfo.xCursor,sedInfo.yCursor,sedInfo.colour,false);
}

/**
 * @brief      Do a carriage return.
 */
static void SEDCarriageReturn(void) {
    sedInfo.xCursor = 0;
    sedInfo.yCursor++;
}
