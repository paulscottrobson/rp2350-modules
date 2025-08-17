// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Machine wrapper test app
//      Date :      17th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "machine_module.h"
#include "machine_module_local.h"           /* REMOVE LATER */

int MAINPROGRAM(int argc,char *argv[]) {
    MACInitialise();

    char *buffer = MEMAlloc(MAX_INPUT_SIZE+1,MEM_ANY);

    VDUWrite(22);VDUWrite(mcInfo.mode);                                            // Video mode
    VDUWrite(17);VDUWrite(6);
    VDUWrite(17);VDUWrite(132);
    VDUWrite(1);VDUWrite(1);VDUWrite(2);                                          // Double height
    VDUWrite(12);
    
    while (COMAppRunning()) {                                                       // Our "main program"
        bool isOk = SEDInputLine(buffer,MAX_INPUT_SIZE+1);
        if (isOk) LOG("Entered '%s'",buffer);
        COMUpdate();                                                                // Update stuff.
    }
}
