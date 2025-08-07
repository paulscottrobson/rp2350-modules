// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      write.c
//      Purpose :   Writing to window.
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "text_module.h"
#include "text_module_local.h"

static void _TXTClearScreen(TXTWINDOW *window);
static void _TXTDrawCursor(TXTWINDOW *window);
static void _TXTCarriageReturn(TXTWINDOW *window);
static void _TXTBackSpace(TXTWINDOW *window);
static void _TXTAdjust(TXTWINDOW *w,int16_t xo,int16_t yo);

/**
 * @brief      Write character to arbitrary window
 *
 * @param      window  Address of structure defining text window.
 * @param[in]  ch      Character to write.
 */
void TXTWriteWindow(TXTWINDOW *window,uint32_t ch) {
    TXTCheckStatus(window);
    GFXOpenContext();
    if (window->isCursorDrawn) {
        _TXTDrawCursor(window);
        window->isCursorDrawn = false;
    }
    GFXDraw(RawColour,window->textColour,window->backColour);
    switch(ch) {
        case CTL_TAB:
            window->xCursor = ((window->xCursor + 8) & 0xFFF8);
            if (window ->xCursor >= window->width) _TXTCarriageReturn(window);
            break;
        case CTL_LEFT:
            _TXTAdjust(window,-1,0);break;
        case CTL_RIGHT:
            _TXTAdjust(window,1,0);break;
        case CTL_UP:
            _TXTAdjust(window,0,-1);break;
        case CTL_DOWN:
            _TXTAdjust(window,0,1);break;
        case CTL_PAGEUP:
            _TXTAdjust(window,0,-window->height*2/3);break;
        case CTL_PAGEDOWN:
            _TXTAdjust(window,0,window->height*2/3);break;
        case CTL_HOME:
            window->xCursor = 0;break;
        case CTL_END:
            window->xCursor = window->width-1;break;
        case CTL_BACKSPACE:
            _TXTBackSpace(window);break;
        case CTL_CRLF:
            _TXTCarriageReturn(window);break;
        case CTL_CLEAR:
            _TXTClearScreen(window);break;
        default:
            if (ch >= 0x20 && ch <= 0x7F) {
                TXTWriteChar(window,window->xCursor,window->yCursor,ch,true);
                window->xCursor++;
                if (window->xCursor == window->width) {
                    _TXTCarriageReturn(window);
                }
            }
            break;
    }
    _TXTDrawCursor(window);
    window->isCursorDrawn = true;
    GFXCloseContext();
}

/**
 * @brief      Read a character from the state manager
 *
 * @param      window  Window Structure
 * @param[in]  x       x coordinate
 * @param[in]  y       y coordinate
 *
 * @return     0 if not available, or character at that coordinate
 */
uint16_t TXTReadChar(TXTWINDOW *window,uint16_t x,uint16_t y) {
    if (window->stateManager == NULL) return 0;                                     // No state manager.
    return (*(TXTSTATEMANAGER)window->stateManager)(window,x,y,0);                  // Call state manager with character 0.
}

/**
 * @brief      Write a character, notifying the state manager optionally
 *
 * @param      window       Window structure
 * @param[in]  x            x coordinate
 * @param[in]  y            y coordinate
 * @param[in]  ch           character
 * @param[in]  updateState  true to update
 */
void TXTWriteChar(TXTWINDOW *window,uint16_t x,uint16_t y,uint16_t ch,bool updateState) {
    GFXDraw(Move,window->xPos+x*window->xSize,window->yPos+y*window->ySize);        // Draw the character
    GFXDraw(Character,ch,0);   
    if (window->stateManager != NULL && updateState) {                              // If updating and state manager in use, update it.
        (*(TXTSTATEMANAGER)window->stateManager)(window,x,y,ch);
    }
}

/**
 * @brief      Adjust text position and wrap
 *
 * @param      window  Window structure
 * @param[in]  xo      x offset
 * @param[in]  yo      y offset
 */
static void _TXTAdjust(TXTWINDOW *window,int16_t xo,int16_t yo) {
    window->xCursor = (window->xCursor + xo + window->width) % window->width;
    window->yCursor = (window->yCursor + yo + window->height) % window->height;
}
/**
 * @brief      Clear the screen
 *
 * @param      window  Window structure
 */
static void _TXTClearScreen(TXTWINDOW *window) {
    for (int x = 0;x < window->width;x++) {
        for (int y = 0;y < window->height;y++) {
            TXTWriteChar(window,x,y,' ',true);
        }
    }
}

/**
 * @brief      Backspace functionality
 *
 * @param      window  Window structure.
 */
static void _TXTBackSpace(TXTWINDOW *window) {
    if (window->xCursor != 0 || window -> yCursor != 0) {                
        if (window->xCursor != 0) {
            window->xCursor--;
        } else {
            window->yCursor--;window->xCursor = window->width-1;
        }
        TXTWriteChar(window,window->xCursor,window->yCursor,' ',true);
    }
}

/**
 * @brief      Carriage return
 *
 * @param      window  Window structure
 */
static void _TXTCarriageReturn(TXTWINDOW *window) {
    window->xCursor = 0;                                                            // Start next line down
    window->yCursor++;
    if (window->yCursor >= window->height) {                                        // Bottom of screen ?
        if (window->stateManager == NULL) {                                         // Not recording state, so we do it Spectrum style, clear/home.
            _TXTClearScreen(window);window->xCursor = 0;window->yCursor = 0;
        } else {                                                                    // Recording state
            for (int y = 0;y < window->height-1;y++) {                              // Scroll the screen by reading state & copying up.
                for (int x = 0;x < window->width;x++) {
                    uint16_t c = TXTReadChar(window,x,y+1);
                    if (c != 0) TXTWriteChar(window,x,y,c,true);
                }
            }
            for (int x = 0;x < window->width;x++) {                                 // Blank bottom line.
                TXTWriteChar(window,x,window->height-1,' ',true);
            }
            window->yCursor = window->height-1;                                     // Start of bottom line.
        }
    }
}

/**
 * @brief      Draw cursor
 *
 * @param      window  window structure
 */
static void _TXTDrawCursor(TXTWINDOW *window) {
    int x = window->xCursor;
    int y = window->yCursor;
    if (x >= window->width) { x = 0; y++; }
    if (y < window->height) {
        x = window->xPos + x * window->xSize;
        y = window->yPos + y * window->ySize;
        GFXDraw(DrawMode,3,0);
        GFXDraw(RawColour,255,0);
        GFXDraw(Move,x,y);
        GFXDraw(FillRect,x+7,y+7);
        GFXDraw(DrawMode,0,0);
    }
}

