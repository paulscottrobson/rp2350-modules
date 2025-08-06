// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Input Manager main program
//      Date :      2nd July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include <stdlib.h>
#include <math.h>
#include "dvi_module.h"
#include "input_module.h"

static uint8_t framebuffer[640*480];
static void drawGamepadButton(int x,int y,bool on);

/**
 * @brief      Display line accessor
 *
 * @param[in]  scanLine  Scan line
 *
 * @return     Address of line data.
 */
static uint8_t *KEEPINRAM(_DVIGetDisplayLine)(uint16_t scanLine) {
    return framebuffer + scanLine * 640;                                            // Simple 640x480 screen
}

/**
 * @brief      Simple test program
 *
 * @param[in]  argc  Count of arguments
 * @param      argv  Arguments array
 *
 * @return     Error code.
 */
int MAINPROGRAM(int argc,char *argv[]) {
    int next = 0;
    INPInitialise();                                                                // Initialise input module.
    DVIInitialise();                                                                // Set up display - this is for mouse testing, not 
    DVISetLineAccessorFunction(_DVIGetDisplayLine);                                 // required for this module as a dependency.
    DVISetMode(1);                                                                  // 640 x 480 x 256 colours
    memset(framebuffer,0x03,640*480);
    while (COMAppRunning()) {                                                       // This is for the run time library.
        int16_t x,y,s,b;
        INPGetMouseStatus(&x,&y,&b);                                                // Read the mouse
        x = x >> 1;y = y >> 1;s = b * 2 + 2;                                        // Mouse coordinate to pixels, buttons define the size.
        for (int16_t xi = x-s;xi < x+s;xi++) {                                      // Draw the 'blob'
            for (int16_t yi = y-s;yi < y+s;yi++) {
                if (xi >= 0 && xi < 640 && yi >= 0 && yi < 480) {
                    framebuffer[xi+yi*640] = random();
                }
            }
        }
        //
        //      Display the gamepad.
        //
        INPGAMEPAD *pad = INPReadGamepad(0);
        if (pad != NULL) {
            drawGamepadButton(4,1,pad->a);
            drawGamepadButton(5,1,pad->b);
            drawGamepadButton(6,1,pad->x);
            drawGamepadButton(7,1,pad->y);
            drawGamepadButton(0,1,pad->dx < 0);
            drawGamepadButton(2,1,pad->dx > 0);
            drawGamepadButton(1,0,pad->dy < 0);
            drawGamepadButton(1,2,pad->dy > 0);
        } 
        //
        //      In early tests, if this was run as is, the DVI Library crashed (the main core kept going). I'm not quite sure why
        //      USBUpdate() is now coded in its own library so that this only calls at 25Hz irrespective of how fast you actually
        //      run this.
        //
        COMUpdate();    
    }	
    return 0;
}

/**
 * @brief      Draw a square button
 *
 * @param[in]  x     x position
 * @param[in]  y     y position
 * @param[in]  on    draw green or red
 */
static void drawGamepadButton(int x,int y,bool on) {
    x = x * 32 + 16;y = y * 32 + 16;
    for (int x1 = 0;x1 < 20;x1++) {
        for (int y1 = 0;y1 < 20;y1++) {
            framebuffer[x+x1+(y+y1)*640] = on ? 0x1C : 0xE0;
        }
    }
}

