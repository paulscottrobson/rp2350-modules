// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      setup.c
//      Purpose :   Initialise the PSRAM interface
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
// 
#include "psram_module.h"
#include "psram_module_local.h"
#include "rp2350pc_psram.h"

static uint32_t ramSize;

/**
 * @brief      Initialise the PSRAM System.
 */
void PSRInitialise(void) {
    static bool isInitialised = false;
    if (isInitialised) return;
    isInitialised = true;

    COMInitialise();
    ramSize = PSRHW_Initialise(8);    
}

/**
 * @brief      Get the amount of PSRAM available.
 *
 * @return     Number of bytes
 */
uint32_t PSRGetMemorySize(void) {
    return ramSize;
}

/**
 * @brief      Get the base address of PSRAM
 *
 * @return     Address of PSRAM in memory space, to disable return NULL.
 */
uint8_t *PSRGetMemoryAddress(void) {
    return (uint8_t *)PSRAM_LOCATION;
}