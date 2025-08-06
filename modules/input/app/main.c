// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Input Manager main program
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include <stdlib.h>
#include <math.h>
#include "dvi_module.h"
#include "input_module.h"

static void drawGamepadButton(int x,int y,bool on);
static void plotPixel(int x,int y,int colour);
static DVIMODEINFO info;


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
    DVIInitialise();                                                                // Set up display - this is for mouse testing, not 
    DVISetMode(MODE_640_480_8);
    info = *DVIGetModeInformation();                                                // Get mode information.    
    for (int i = 0;i < info.bitPlaneCount;i++) {                                    // Blue background.
        memset(info.bitPlane[i],(i == 2) ? 0xFF:0x00,info.bitPlaneSize);
    }
    INPInitialise();                                                                // Initialise input module.
    while (COMAppRunning()) {                                                       // This is for the run time library.
        int16_t x,y,s,b;
        INPGetMouseStatus(&x,&y,&b);                                                // Read the mouse
        x = x >> 1;y = y >> 1;s = b * 2 + 2;                                        // Mouse coordinate to pixels, buttons define the size.
        for (int16_t xi = x-s;xi < x+s;xi++) {                                      // Draw the 'blob'
            for (int16_t yi = y-s;yi < y+s;yi++) {
                if (xi >= 0 && xi < 640 && yi >= 0 && yi < 480) {
                    plotPixel(xi,yi,random() & 7);
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
        //      Track key presses.
        //
        uint32_t k = INPGetKey();
        if (k != 0) LOG("%d pressed.",k);
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
            plotPixel(x+x1,y+y1,on ? 3 : 1);
        }
    }
    plotPixel(x,y,6);
}

/**
 * @brief      Simple pixel plotter, 8 colour mode
 *
 * @param[in]  x       x position
 * @param[in]  y       y position
 * @param[in]  colour  colour BGR
 */
static void plotPixel(int x,int y,int colour) {
    int pos = (x >> 3) + y * info.bytesPerLine;
    int mask = (0x80 >> (x & 7));
    for (int plane = 0;plane < info.bitPlaneCount;plane++) {
        info.bitPlane[plane][pos] &= ~mask;        
        if (colour & (1 << plane)) info.bitPlane[plane][pos] |= mask;
    }
}
