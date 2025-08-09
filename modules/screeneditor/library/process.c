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
static void SEDScrollWindow(void);
static uint32_t SEDExtract(uint32_t y,uint8_t *buffer,uint32_t size);
static void SEDInsert(void);
static void SEDDelete(void);

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
        sedInfo.xCursor = 0;
        sedInfo.yCursor = SEDExtract(sedInfo.yCursor,buffer,size);                  // Extract text
        SEDProcessKey(0);                                                           // Forces checking of y position
    } else {
        SEDProcessKey(key); 
    }
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
        case CTL_INSERT:
            SEDInsert();
            break;
        case CTL_DELETE:
            SEDDelete();
            break;
        default:
            if (key >= 0x20 && key <= 0x7F) {
                *SEDCharAccess(sedInfo.xCursor,sedInfo.yCursor) = key;
                SEDDraw(sedInfo.xCursor,sedInfo.yCursor,sedInfo.colour,false);
                if (sedInfo.xCursor == sedInfo.width-1) {                           // Have we done a multiple line ?
                    sedInfo.extendLine[sedInfo.yCursor] = 1;                        // Set the 'extended line' flag.
                }
                sedInfo.xCursor++;                                                  // Right, CR if right edge.
                if (sedInfo.xCursor == sedInfo.width) {
                    sedInfo.yCursor++;sedInfo.xCursor = 0;
                }
            }
            break;
    }
    if (sedInfo.yCursor == sedInfo.height) {
        SEDScrollWindow();
        sedInfo.yCursor = sedInfo.height-1;
    }
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

/**
 * @brief      Scroll the window up.
 */
static void SEDScrollWindow(void) {
    for (int x = 0;x < sedInfo.width;x++) {                                         // Scroll the characters up.
        for (int y = 0;y < sedInfo.height-1;y++) {
            *SEDCharAccess(x,y) = *SEDCharAccess(x,y+1);

        }
        *SEDCharAccess(x,sedInfo.height-1) = ' ';
    }
    memcpy(sedInfo.extendLine,sedInfo.extendLine+1,SED_MAXHEIGHT-1);                // Scroll the extended line array up to keep synced.
    sedInfo.extendLine[SED_MAXHEIGHT-1] = 0;
    SEDRepaint();
}

/**
 * @brief      Extract text from screen editor
 *
 * @param[in]  y       Line where return was pressed
 * @param      buffer  Character buffer
 * @param[in]  size    Size of buffer
 *
 * @return     Line where cursor should now go.
 */
uint32_t SEDExtract(uint32_t y,uint8_t *buffer,uint32_t size) {
    while (y > 0 && sedInfo.extendLine[y-1] != 0) {                                 // While not at top and previous line is an extension.
        y--;                                                                        // Go backwards.
    }
    bool completed = false;
    uint8_t *start = buffer;  

    while (!completed) {                                                            // Now extract it down.
        if (sedInfo.width >= size-1) ERROR("Cannot fit in screen editor buffer");   // Can we extract text into the buffer ?
        for (uint32_t x = 0;x < sedInfo.width;x++) {                                // Extract the line.
            *buffer ++ = *SEDCharAccess(x,y);
        }
        size = size - sedInfo.width;                                                // Adjust the size remaining accordingly.
        completed = (sedInfo.extendLine[y] == 0);                                   // Completed if the line is not extended.
        y++;                                                                        // Go to next line.
    }
    while (buffer > start && *(buffer-1) == ' ') buffer--;                          // Strip trailing spaces.
    *buffer = '\0';                                                                 // Mark buffer end.    
    return y;                                                                       // Line after completed line.
}

/**
 * @brief      Insert a space at the current position, moving things to make space.
 */
static void SEDInsert(void) {
    LOG("Insert");
}

/**
 * @brief      Delete character at the current position, shifting everything back one.
 */
static void SEDDelete(void) {
    LOG("Delete");
}