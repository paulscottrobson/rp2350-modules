// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Text module test program.
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "text_module.h"
#include "input_module.h"

/**
 * @brief      A simple state manager. This tracks the actual contents of the
 *             text window, which is optional, for things like a screen editor.
 *             This can be used for both writing to the screen state and reading
 *             it.
 *
 * @param      w     Window structure
 * @param[in]  x     x position
 * @param[in]  y     y position
 * @param[in]  ch    character to write if non zero
 *
 * @return     character at that point.
 */
int16_t DemoStateManager(TXTWINDOW *w,uint16_t x,uint16_t y,uint16_t ch) {
    static uint8_t stateMem[80*50];
    uint16_t offset = x + y * 80;                                                   // Convert coords to offset.
    if (offset >= 4000) return 0;                                                   // Out of range - should not happen !
    if (ch != 0) stateMem[offset] = ch & 0xFF;                                      // Store ch if non-zero
    return stateMem[offset];
}

/**
 * @brief      Main Progrm
 *
 * @param[in]  argc  Arguments count
 * @param      argv  Arguments
 *
 * @return     Error code
 */
int MAINPROGRAM(int argc,char *argv[]) {
    INPInitialise();                                                                // Initialise input
    TXTInitialise();
    GFXDraw(Mode,MODE_640_480_8,0);       
    GFXDraw(Desktop,0,0);

    TXTWINDOW *df = TXTDefault();
    df->xPos = 64;df->yPos = 32;df->width = 26;df->height = 12;
    df->stateManager = DemoStateManager;
    TXTWrite(CTL_CLEAR);

    while (COMAppRunning()) {                                                       // Our "main program"
        int16_t k = INPGetKey();                                                    // Get key, log to serial and list if F or D pressed
        if (k != 0) {
            LOG("Key %d",k);
            TXTWrite(k);
        }
        COMUpdate();                                                                // Update stuff.
    }
	return 0;
}
