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
    char buffer[256];    
    SEDInitialise();
    VDUWrite(22);VDUWrite(MODE_640_480_8);                                          // Video mode
    VDUWrite(17);VDUWrite(6);
    VDUWrite(17);VDUWrite(132);
    VDUWrite(1);VDUWrite(1);VDUWrite(2);                                          // Double height
    //VDUWrite(28);VDUWrite(2);VDUWrite(38);VDUWrite(20);VDUWrite(5);                 // Operating window
    VDUWrite(12);
    
    while (COMAppRunning()) {                                                       // Our "main program"
        bool isOk = SEDInputLine(buffer,sizeof(buffer));
        if (isOk) LOG("Entered '%s'",buffer);
        COMUpdate();                                                                // Update stuff.
    }
}

