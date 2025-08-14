// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      setup.c
//      Purpose :   Setup/Configure
//      Date :      14th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "screeneditor_module.h"
#include "screeneditor_module_local.h"

/**
 * @brief      Initialise screen editor. 
 */
void SEDInitialise(void) {
    static bool isInitialised = false;
    if (isInitialised) return;
    isInitialised = true;
    VDUInitialise();
    INPInitialise();
}