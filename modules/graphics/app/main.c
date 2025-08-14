// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   VDU test application.
//      Date :      13th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"

static void generalTest(void);

int MAINPROGRAM() {
    uint32_t x,y;
    DVIInitialise();
    VDUInitialise();
    generalTest();

    VDUWrite(24);VDUWriteWord(50);VDUWriteWord(50);VDUWriteWord(1230);VDUWriteWord(900);


    uint8_t plotList[] = { 5,21,69,85,101,149,157,197,205 };
    while (COMAppRunning()) {                         
        VDUSetGraphicsColour(0,rand() & 63);                                            
        uint8_t element = plotList[(COMClock()/1000+5) % sizeof(plotList)];
        if (element < 149) {
            VDUPlot(element,rand() % 1280,rand() % 1024);
        } else {
            x = rand() % 1024+128; y = rand() % 768+128;
            VDUPlot(4,x,y);
            VDUPlot(element,x+rand() % 256,y+rand() % 256);
        }
        COMUpdate();
    }   }

/**
 * @brief      General test procedure
 */
static void generalTest(void) {
    VDUWrite(22);VDUWrite(MODE_320_240_8);
    for (int i = 0;i < 10;i++) {
        uint8_t c1 = random() & 7,c2 = c1 ^ 7;
        
        VDUWrite(17);VDUWrite(c1);
        VDUWrite(17);VDUWrite(c2+128);
        VDUWrite(i*5+38);
        if (i == 5) VDUWrite(10);  
    }
    VDUSetGraphicsColour(0,7);
    VDUPlot(12,42,0);VDUPlot(5,42,1020);

    for (int i = 0;i < 10;i++) {
        uint8_t c = VDURead((i >= 6) ? 1 : 0,i);
        LOG("%d : %d %x",i,c,c);
    }
    uint32_t next = 0;
    while (COMAppRunning()) {                                                                     
        COMUpdate();
    }    
}
