// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      setup.c
//      Purpose :   Machine wrapper setup
//      Date :      17th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "machine_module.h"
#include "machine_module_local.h"           

MACINFO mcInfo;

char *arguments[MAX_ELEMENTS];                                                      // Array of element pointers.

/**
 * @brief      Initialise the machine.
 */
void MACInitialise(void) {
    static bool isInitialised = false;
    if (isInitialised) return;
    isInitialised = true;

    MEMInitialise();
    INPInitialise();
    VDUInitialise();
    SEDInitialise();

    //
    //      Initialise machine information structure.
    //
    mcInfo.inkColour = 6;
    mcInfo.paperColour = 4;
    mcInfo.errorColour = 1;
    mcInfo.mode = MODE_640_480_8;
    mcInfo.doubleHeight = true;
    mcInfo.buffer = NULL;
    mcInfo.bufferSize = 256;
    mcInfo.argumentCount = 0;
    mcInfo.arguments = arguments;
}
