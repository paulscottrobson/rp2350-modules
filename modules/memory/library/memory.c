// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      memory.c
//      Purpose :   Initialise & Use the Memory manager
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

/**
 * @brief      Allocate a block of memory.
 *
 * @param[in]  ramSize  Amount to allocate
 * @param[in]  ramType  Type of RAM Slow, Fast or either
 *
 * @return     NULL if failed, else address of RAM.
 */
uint8_t *MEMAlloc(uint32_t ramSize,uint32_t ramType) {
    uint8_t *addr;
    if (ramType & MEM_SLOW) {                                                       // So try PSRAM first as we've lots more of that !
        addr = MEMAllocateBlock(&psRAMTracker,ramSize);                             // Try to allocate it.
        if (addr != NULL) return addr;                                              // Successful.
    }
    if (ramType & MEM_FAST) {                                                       // Now try Pico SRAM
        addr = MEMAllocateBlock(&sRAMTracker,ramSize);                              // Try to allocate it.
        if (addr != NULL) return addr;                                              // Successful.
    }
    return NULL;                                                                    // Couldn't allocate it.
}
/**
 * @brief      Free a block of memory in slow or fast ram
 *
 * @param      address  The address of the block
 *
 * @return     True if successful.
 */
bool MEMFree(uint8_t *address) {
    if (MEMFreeBlock(&sRAMTracker,address)) return true;                            // Try to free SRAM
    return MEMFreeBlock(&psRAMTracker,address);                                     // Try to free PSRAM
}
