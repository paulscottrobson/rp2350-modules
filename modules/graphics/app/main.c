// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Graphics module test program.
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"

static void generalTest(void);

/**
 * @brief      A simple demo mapper. All it does is halve all the coordinates,
 *             shifting the drawing into the upper left quarter of the screen.
 *             The clipping rect still works, but only top/left sides because
 *             that's still on the physical system.
 *
 * @param      x     ptr to x to map
 * @param      y     ptr to y to map.
 */
void _DemoMapper(uint32_t *x,uint32_t *y) {
    *x = (*x) >> 1;
    *y = (*y) >> 1;
}

/**
 * @brief      Main program
 *
 * @return     0 if no error.
 */
int MAINPROGRAM() {

    DVIInitialise();                                                                // Initialise Graphics
    GFXDraw(Mode,MODE_640_480_8,0);                                                 // Mode 640x480 8 colours.
    GFXDraw(Desktop,0,0);
    
    // GFXDraw(Desktop,0,0);                                                           // Clear to desktop
    // GFXDraw(Move,20,30);                                                            // Clip not whole screen.
    // GFXDraw(SetClip,610,440);

    // uint32_t size = GFXDraw(CharExtent,'W',0);                                      // Tells us the extent of 'W' in pixels
    // LOG("%x\n",size);                                                               // Should be 80008 e.g. 8 high 8 wide.

    //
    //      To see the mapper working uncomment this.
    //
    // GFXDraw(SetMapper,(GFXDRAWPARAM)_DemoMapper,0);                               // Set coordinate mapper.

    //
    //      This is a tester for the RGB -> Colour mapping
    //
    //showColours();return 0;

    //
    //      Available for testing.
    //
    generalTest();return 0;    

    //
    //      Demo code. Cycles through lots of drawing with the various drawing commands.
    //
    static uint32_t commands[] = { Plot, Line, Rect, FillRect, Ellipse, FillEllipse, Triangle, FillTriangle,Character };
    uint32_t count = 0;

    while (COMAppRunning()) {                                                                     
        count++;
        GFXDraw(RawColour,random() & 7,0);
        uint32_t command;

        command = COMClock()/1000;                                                  // This line does 1s for each draw type
        // command = count / 1000;                                                  // This line shows how long it takes to draw 1000 of each

        command = commands[command % 9];                                            // Work out the actual command
        if (command != Character) {                                                 // Draw to random point if not character
            GFXDraw(command,random() % 640,random() % 480);
        } else {
            GFXDraw(Move,random() % 640,random() % 480);                            // Random pos, scaling and character
            GFXDraw(Scaling,random()%4+1,random()%4+1);
            GFXDraw(Character,random()%96+32,0);
        }
        COMUpdate();                         
    }
    return 0;
}

/**
 * @brief      General test procedure
 */
static void generalTest(void) {
    GFXDraw(Colour,3,1);
    GFXDraw(ClearWindow,0,0);
    while (COMAppRunning()) {                                                                     
        COMUpdate();
    }    
}

