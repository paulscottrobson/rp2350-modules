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
#include "usb_module.h"

static void showColours(void);
static void generalTest(void);

uint8_t vRAM[640*480];

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

    USBInitialise();                                                                // Initialise USB
    GFXInitialise();                                                                // Initialise Graphics
    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                           // Set video ram and size
    GFXDraw(Mode,MODE_640_480_16,0);                                               // Set mode.

    int speckle = 640*480;
    speckle = 320;

    GFXDraw(Desktop,0,0);                                                           // Clear to desktop
    GFXDraw(Move,20,30);                                                            // Clip not whole screen.
    GFXDraw(SetClip,610,440);

    uint32_t size = GFXDraw(CharExtent,'W',0);                                      // Tells us the extent of 'W' in pixels
    LOG("%x\n",size);                                                               // Should be 80008 e.g. 8 high 8 wide.

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
    // generalTest();return 0;    

    //
    //      Demo code. Cycles through lots of drawing with the various drawing commands.
    //
    static uint32_t commands[] = { Plot, Line, Rect, FillRect, Ellipse, FillEllipse, Triangle, FillTriangle,Character };
    uint32_t count = 0;

    while (COMAppRunning()) {                                                                     
        count++;
        vi.drawSurface[random()%speckle] = (random() & 1) ? 0:random();             // Knows we aren't crashing. May consider flashing LED :)
        GFXDraw(RawColour,random() & 0xFF,0);
        uint32_t command;

        command = COMTimeMS()/1000;                                                 // This line does 1s for each draw type
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
 * @brief      Test for RGB to Raw conversion.
 */
static void showColours(void) {
    GFXDrawP(SetClip,NULL,0);
    int x = 4;
    for (int r = 0;r < 4;r++) {
        for (int g = 0;g < 4;g++) {
            for (int b = 0;b < 4;b++) {
                int raw = (r << 6)|(g << 3)|b;
                if (vi.pixelsPerByte == 2) raw = ((r & 2) << 2) | ((g & 3) << 1) | ((b & 2) >> 1);
                if (vi.pixelsPerByte == 4) raw = b;
                if (vi.pixelsPerByte == 8) raw = b & 1;
                GFXDraw(RawColour,raw,0);
                GFXDraw(Move,x,10);
                GFXDraw(FillRect,x+9,200);

                int rgb4 = (r << 10)|(g << 6)|(b << 2);
                if (vi.pixelsPerByte == 4) rgb4 = (b << 2) | (b << 6) | (b << 10);
                if (vi.pixelsPerByte == 8) rgb4 = (raw << 3) | (raw << 7) | (raw << 11);
                GFXDraw(Colour,rgb4,0);
                GFXDraw(Move,x,210);
                GFXDraw(FillRect,x+9,410);
                x += 9;
            }
        }
    }
    while (COMAppRunning()) {                                                                     
        COMUpdate();                     
    }    
}

/**
 * @brief      General test procedure
 */
static void generalTest(void) {
    GFXDraw(Colour,0x0F0,0xF80);
    GFXDraw(ClearWindow,0,0);
    while (COMAppRunning()) {                                                                     
        COMUpdate();
    }    
}

