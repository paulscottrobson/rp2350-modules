// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Screeneditor test app
//      Date :      14th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "screeneditor_module.h"

int MAINPROGRAM(int argc,char *argv[]) {
    SEDInitialise();
    VDUWrite(22);VDUWrite(MODE_640_480_8);                                          // Video mode
    VDUSetGraphicsColour(0,4);                                                      // Blue
    VDUWrite(17);VDUWrite(2);
    VDUPlot(4,0,0);VDUPlot(101,1280,1024);                                          // Full screen rectangle
    //VDUWrite(1);VDUWrite(1);VDUWrite(2);                                            // Double height
    VDUWrite(28);VDUWrite(2);VDUWrite(18);VDUWrite(12);VDUWrite(5);                 // Operating window
    VDUWrite(12);
    
    while (COMAppRunning()) {                                                       // Our "main program"
        int16_t k = INPGetKey();                                                    // Get key, log to serial and list if F or D pressed
        if (k != 0) {
            LOG("Key %d",k);
            SEDProcess(k);
        }
        COMUpdate();                                                                // Update stuff.
    }
}

