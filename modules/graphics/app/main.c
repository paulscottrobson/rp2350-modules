// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      
//      Purpose :   
//      Date :      13th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"

static void generalTest(void);

int MAINPROGRAM() {
    DVIInitialise();
    VDUInitialise();
    generalTest();
}

/**
 * @brief      General test procedure
 */
static void generalTest(void) {
    //VDUWrite(22);
    //VDUWrite(MODE_320_240_8);
    VDUPlotCommand(4,0,0);
    VDUPlotCommand(5,640,512);
    for (int i = 32;i < 256;i++) VDUWrite(i);
    VDUWrite(30);
    VDUWrite(11);
    while (COMAppRunning()) {                                                                     
        COMUpdate();
    }    
}
