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

#include "screen_module.h"
#include "screen_module_local.h"

static void SCRAdjust(int32_t xi,int32_t yi);
static void SCRBackSpace(void);
static void SCRCarriageReturn(void);
static void SCRScrollWindow(void);
static uint32_t SCRExtract(uint32_t y,uint8_t *buffer,uint32_t size);
static void SCRInsert(void);
static void SCRDelete(void);
static uint32_t SCRFindEndCurrent(uint32_t y);
static uint32_t SCRFindEndCurrent(uint32_t y);

/**
 * @brief      Get one line into the buffer from the current cursor position.
 *
 * @param      buffer  Buffer for line.
 * @param[in]  size    Buffer size in bytes
 */

void SCRGetLine(uint8_t *buffer,uint32_t size) {
    while (SCRProcess(buffer,size,INPGetKey())) {
        COMUpdate();
    }
}

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
bool SCRProcess(uint8_t *buffer,uint32_t size,uint32_t key) {
    *buffer = '\0';                                                                 // Empty buffer in case we exit runtime.
    if (key == 0) {                                                                 // No key, flash the cursor.
        bool showCursor = (COMClock() & 512) != 0;        
        SCRDraw(scrInfo.xCursor,scrInfo.yCursor,
                scrInfo.ink, 
                *SCRCharAccess(scrInfo.xCursor,scrInfo.yCursor),showCursor);
        return COMAppRunning();
    }
    SCRDraw(scrInfo.xCursor,scrInfo.yCursor,                                        // Hide the cursor
            scrInfo.ink,
            *SCRCharAccess(scrInfo.xCursor,scrInfo.yCursor),false);                  

    if (key == CTL_CRLF) {                                                          // If CR load the current line in.
        scrInfo.xCursor = 0;
        scrInfo.yCursor = SCRExtract(scrInfo.yCursor,buffer,size);                  // Extract text
        SCRWriteChar(0);                                                            // Forces checking of y position
    } else {
        SCRWriteChar(key); 
    }
    return COMAppRunning() && key != CTL_CRLF;
}

/**
 * @brief      Process a key stroke
 *
 * @param[in]  key   key to process
 */
void SCRWriteChar(uint32_t key) {
    switch(key) {
        case CTL_TAB:
            scrInfo.xCursor = ((scrInfo.xCursor + 8) & 0xFFF8);
            SCRAdjust(0,0);
            break;
        case CTL_LEFT:
            SCRAdjust(-1,0);break;
        case CTL_RIGHT:
            SCRAdjust(1,0);break;
        case CTL_UP:
            SCRAdjust(0,-1);break;
        case CTL_DOWN:
            SCRAdjust(0,1);break;
        case CTL_PAGEUP:
            SCRAdjust(0,-scrInfo.height*2/3);break;
        case CTL_PAGEDOWN:
            SCRAdjust(0,scrInfo.height*2/3);break;
        case CTL_HOME:
            scrInfo.xCursor = 0;break;
        case CTL_END:
            scrInfo.xCursor = scrInfo.width-1;break;
        case CTL_BACKSPACE:
            SCRBackSpace();break;
        case CTL_CRLF:
            SCRCarriageReturn();break;
        case CTL_CLEAR:
            SCRClear();
            scrInfo.xCursor = scrInfo.yCursor = 0;            
            break;
        case CTL_INSERT:
            SCRInsert();
            break;
        case CTL_DELETE:
            SCRDelete();
            break;
        default:
            if (key >= 0x20 && key <= 0x7F) {
                SCRWrite(scrInfo.xCursor,scrInfo.yCursor,key);                      // Write to display.
                if (scrInfo.xCursor == scrInfo.width-1) {                           // Have we done a multiple line ?
                    scrInfo.extendLine[scrInfo.yCursor] = 1;                        // Set the 'extended line' flag.
                }
                scrInfo.xCursor++;                                                  // Right, CR if right edge.
                if (scrInfo.xCursor == scrInfo.width) {
                    scrInfo.yCursor++;scrInfo.xCursor = 0;
                }
            }
            break;
    }
    if (scrInfo.yCursor == scrInfo.height) {
        SCRScrollWindow();
        scrInfo.yCursor = scrInfo.height-1;
    }
}

/**
 * @brief      Adjust position and fix up.
 *
 * @param[in]  xi    x offset
 * @param[in]  yi    y offset
 */
static void SCRAdjust(int32_t xi,int32_t yi) {
    scrInfo.xCursor += xi;
    scrInfo.yCursor += yi;
    if (scrInfo.xCursor < 0) {
        scrInfo.xCursor = scrInfo.width-1;
        scrInfo.yCursor--;
    }
    if (scrInfo.xCursor >= scrInfo.width) {
        scrInfo.xCursor = 0;
        scrInfo.yCursor++;
    }
    if (scrInfo.yCursor < 0) scrInfo.yCursor = 0;
    if (scrInfo.yCursor >= scrInfo.height) scrInfo.yCursor = scrInfo.height-1;
}
    
/**
 * @brief      Do a backspace.
 */
static void SCRBackSpace(void) {
    SCRAdjust(-1,0);
    SCRWriteChar(' ');
    SCRAdjust(-1,0);
}

/**
 * @brief      Do a carriage return.
 */
static void SCRCarriageReturn(void) {
    scrInfo.xCursor = 0;
    scrInfo.yCursor++;
}

/**
 * @brief      Scroll the window up.
 */
static void SCRScrollWindow(void) {
    for (int x = 0;x < scrInfo.width;x++) {                                         // Scroll the characters up.
        for (int y = 0;y < scrInfo.height-1;y++) {
            SCRCopy(x,y+1,x,y);
        }
        SCRWrite(x,scrInfo.height-1,' ');
    }
    memcpy(scrInfo.extendLine,scrInfo.extendLine+1,SCR_MAXHEIGHT-1);                // Scroll the extended line array up to keep synced.
    scrInfo.extendLine[SCR_MAXHEIGHT-1] = 0;
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
uint32_t SCRExtract(uint32_t y,uint8_t *buffer,uint32_t size) {
    while (y > 0 && scrInfo.extendLine[y-1] != 0) {                                 // While not at top and previous line is an extension.
        y--;                                                                        // Go backwards.
    }
    bool completed = false;
    uint8_t *start = buffer;  

    while (!completed) {                                                            // Now extract it down.
        if (scrInfo.width >= size-1) ERROR("Cannot fit in screen editor buffer");   // Can we extract text into the buffer ?
        for (uint32_t x = 0;x < scrInfo.width;x++) {                                // Extract the line.
            *buffer ++ = *SCRCharAccess(x,y);
        }
        size = size - scrInfo.width;                                                // Adjust the size remaining accordingly.
        completed = (scrInfo.extendLine[y] == 0) || (y == scrInfo.height-1);        // Completed if the line is not extended or at bottom.
        y++;                                                                        // Go to next line.
    }
    while (buffer > start && *(buffer-1) == ' ') buffer--;                          // Strip trailing spaces.
    *buffer = '\0';                                                                 // Mark buffer end.    
    return y;                                                                       // Line after completed line.
}

/**
 * @brief      Insert a space at the current position, moving things to make space.
 */
static void SCRInsert(void) {
    int32_t x,y,x1,y1;
    x = scrInfo.width-1;y = SCRFindEndCurrent(scrInfo.yCursor);
    while (x != scrInfo.xCursor || y != scrInfo.yCursor) {
        x1 = x;y1 = y;
        x--;if (x < 0) x = scrInfo.width-1,y--;
        SCRCopy(x,y,x1,y1);
    }
    SCRWrite(x,y,' ');
}

/**
 * @brief      Delete character at the current position, shifting everything back one.
 */
static void SCRDelete(void) {
    uint32_t x,y,y1;
    x = scrInfo.xCursor;y = scrInfo.yCursor;
    y1 = SCRFindEndCurrent(y);
    while (x != scrInfo.width-1 || y != y1) {
        SCRCopy(x+1,y,x,y);
        x++;if (x == scrInfo.width) x = 0,y++;
    }
    SCRWrite(x,y,' ');
}

/**
 * @brief      Find the end of the line at the cursor by tracking extensions.
 *
 * @param[in]  y     Start line
 *
 * @return     End line.
 */
static uint32_t SCRFindEndCurrent(uint32_t y) {
    while (scrInfo.extendLine[y] != 0 && y != scrInfo.height-1) y++;
    return y;
}

