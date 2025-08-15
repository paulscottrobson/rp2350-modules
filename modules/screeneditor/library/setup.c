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

/**
 * @brief      Input a line into the buffer with updates
 *
 * @param      buffer      Buffer
 * @param[in]  bufferSize  Buffer length
 *
 * @return     true if ok, false if failed.
 */
bool SEDInputLine(uint8_t *buffer,uint16_t bufferSize) {
    bool bCompleted = false;
    bool bResult = false;
    while (!bCompleted && COMAppRunning()) {
        uint16_t k = INPGetKey();
        if (k == 13) {
            bResult = SEDExtract(buffer,bufferSize);
            bCompleted = true;
        } else {
            if (k != 0) SEDProcess(k);
        }
        COMUpdate();
    }
    return bResult;
}