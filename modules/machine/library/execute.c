// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      execute.c
//      Purpose :   Run the machine wrapper.
//      Date :      17th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "machine_module.h"
#include "machine_module_local.h"           

/**
 * @brief      Start the main read/execute/update loop.
 */
void MACStart(void) {
    mcInfo.buffer = MEMAlloc(mcInfo.bufferSize,MEM_ANY);                            // Allocate input buffer in PSRAM.
    VDUWrite(22);VDUWrite(mcInfo.mode);                                             // Set Video mode to whatever ....
    MACSetStandardColour();                                                         // Standard colours
    if (mcInfo.doubleHeight) {                                                      // If wanted, set Double height
        VDUWrite(1);VDUWrite(1);VDUWrite(2);                                            
    }
    VDUWrite(12);                                                                   // Clear screen to background.
    MACBootDisplay();                                                               // Boot display.
    
    while (COMAppRunning()) {                                                       // Our "main program" loop.
        MACSetStandardColour();
        bool isOk = SEDInputLine(mcInfo.buffer,mcInfo.bufferSize);
        if (isOk) LOG("Entered '%s'",mcInfo.buffer);
        MACError("Command not understood");
        COMUpdate();                                                                // Update stuff.
    }
}

/**
 * @brief      Set back to standard colours.
 */
void MACSetStandardColour(void) {
    VDUWrite(17);VDUWrite(mcInfo.inkColour);
    VDUWrite(17);VDUWrite(mcInfo.paperColour+0x80);
}

/**
 * @brief      Report an error.
 *
 * @param      message  The message
 */
void MACError(char *message) {
    MACSetStandardColour();
    VDUWrite(17);VDUWrite(mcInfo.errorColour);
    VDUWriteString("%s\r\n",message);
}

