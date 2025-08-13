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
    VDUWrite(24);
    VDUWriteWord(150);VDUWriteWord(300);VDUWriteWord(1100);VDUWriteWord(700);

    generalTest();

    uint8_t plotList[] = { 5,21,69,85,101,149,157,197,205 };
    while (COMAppRunning()) {                         
        VDUSetGraphicsColour(0,rand() & 63);                                            
        uint8_t element = plotList[(COMClock()/1000+5) % sizeof(plotList)];
        if (element < 149) {
            VDUPlotCommand(element,rand() % 512,rand() % 200+400);
        } else {
            x = rand() % 1024+128; y = rand() % 768+128;
            VDUPlotCommand(4,x,y);
            VDUPlotCommand(element,x+rand() % 256,y+rand() % 256);
        }
        COMUpdate();
    }   }

/**
 * @brief      General test procedure
 */
static void generalTest(void) {
    //VDUWrite(22);
    //VDUWrite(MODE_320_240_8);
    for (int i = 32;i < 256;i++) VDUWrite(i);
    VDUWrite(30);
    VDUWrite(11);
    VDUSetGraphicsColour(0,2);
    VDUPlotCommand(4,150,0);VDUPlotCommand(5,150,1024);
    VDUSetGraphicsColour(0,3);
    uint32_t next = 0;
    VDUPlotCommand(4,155,473);
    VDUPlotCommand(5,145,476);
    while (COMAppRunning()) {                                                                     
        COMUpdate();
    }    
}
