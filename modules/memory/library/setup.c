// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      setup.c
//      Purpose :   Initialise the Memory manager
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "memory_module.h"
#include "memory_module_local.h"

static uint32_t psRAMSize,sRAMSize;                                                 // Size of PSRAM and SRAM available in bytes.
static uint8_t  *psRAMAddress,*sRAMAddress;                                         // Memory location.
static MEMORYTRACKER psRAMTracker,sRAMTracker;                                      // Tracker objects.

/**
 * @brief      Initialise the PSRAM and SRAM memory managers.
 */
void MEMInitialise(void) {
    PSRInitialise();                                                                // Initialise PSRAM - or myself, these are my initials :)
    psRAMSize = PSRGetMemorySize();
    psRAMAddress = PSRGetMemoryAddress();

    sRAMSize = COMGetFreeSystemMemory() * 9 / 10;                                   // We claim 90% of free SRAM. This is a complete guess.
    sRAMAddress = malloc(sRAMSize);  
    if (sRAMAddress == NULL) ERROR("Cannot allocate SRAM. Change fraction in MEMInitialise()");

    MEMReset(0);                                                                    // Reset the memory system with no system allocated SRAM.
    sleep_ms(500);
    LOG("PSRAM %d SRAM %d",psRAMSize,sRAMSize);                                     // Log memory availability.
    LOG("PSRAM $%x SRAM $%x",psRAMAddress,sRAMAddress);
}

/**
 * @brief      Reset the memory managers to the default ; this is the same as
 *             freeing *everything*. Allows system memory to be allocated, which
 *             is never freed or cleared.
 *
 * @param[in]  requiredSRAMMemory  The amount of SRAM required for the system.
 *
 * @return     The address of the required SRAM.
 */
uint8_t *MEMReset(uint32_t requiredSRAMMemory) {
    MEMInitialiseBlock(&psRAMTracker,psRAMAddress,psRAMSize);                       // Initialise tracking for PSRAM
    MEMInitialiseBlock(&sRAMTracker,                                                // Initialise tracking for SRAM
                            sRAMAddress+requiredSRAMMemory,sRAMSize-requiredSRAMMemory);
    return sRAMAddress;
}

