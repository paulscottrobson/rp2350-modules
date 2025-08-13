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
 
#include "alt_graphics_module.h"

static void generalTest(void);
static void generalTest2(void);

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

    GFXInitialise();                                                                // Initialise Graphics
    GFXDraw(Mode,MODE_640_480_8,0);                                                 // Mode 640x480 8 colours.
    //GFXDraw(Mode,MODE_320_240_64,0);
    
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
    //      Available for testing.
    //
    // generalTest();return 0;    

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
    GFXDraw(RawColour,6,4);
    GFXDraw(Clear,0,0);
    GFXDraw(Move,100,100);
    GFXDraw(Line,100,200);
    GFXDraw(Line,200,200);
    GFXDraw(Line,200,150);
    GFXDraw(Line,170,150);
    GFXDraw(Move,50,150);GFXDraw(Line,25,50);
    GFXDraw(Move,50,150);GFXDraw(Line,75,50);
    GFXDraw(Move,50,150);GFXDraw(Line,25,220);
    GFXDraw(Move,50,150);GFXDraw(Line,75,220);

    GFXDraw(Move,10,10);GFXDraw(Rect,70,30);
    GFXDraw(Move,80,10);GFXDraw(FillRect,140,30);
    GFXDraw(Move,150,10);GFXDraw(Ellipse,210,30);
    GFXDraw(Move,220,10);GFXDraw(FillEllipse,280,30);

    GFXDraw(Move,160,40);GFXDraw(Character,'A',0);GFXDraw(Character,'a',0);

    GFXDraw(Move,230,40);GFXDraw(Move,300,50);GFXDraw(Triangle,270,100);
    GFXDraw(Move,230,140);GFXDraw(Move,300,150);GFXDraw(FillTriangle,270,200);

    GFXDraw(RawColour,3,0);
    for (int i = 0;i < 70;i++) {
        GFXDraw(Move,120+i,120-i);GFXDraw(Line,260-i,120-i);
    }
    while (COMAppRunning()) {                                                                     
        COMUpdate();
    }    
}

static void generalTest2(void) {
    sleep_ms(500);
    GFXDraw(Desktop,0,0);
    GFXDraw(RawColour,3,0);
    for (int i = 0;i < 70;i++) {
        GFXDraw(Move,120+i,120-i);GFXDraw(Line,260-i,120-i);
    }
    while (COMAppRunning()) {                                                                     
        COMUpdate();
    }    
}

