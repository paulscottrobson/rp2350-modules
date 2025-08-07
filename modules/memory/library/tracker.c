// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      tracker.c
//      Purpose :   Memory usage tracker
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "memory_module.h"
#include "memory_module_local.h"


/**
 * @brief      Initialise the block tracker structure.
 *
 * @param      tracker  The structure to initialise
 * @param      address  Address of the memory block
 * @param[in]  size     Size of the memory block.
 */
void MEMInitialiseBlock(MEMORYTRACKER *tracker,uint8_t *address,uint32_t size) {
    tracker->baseAddress = address;                                                 // Set memory location and range
    tracker->totalSize = size;
    tracker->blockSize = (size / MAXBLOCKS) & 0xFFFFFFFC;                           // Size of one block. Force to word size.
    tracker->blockCount = size / tracker->blockSize;                                // Number of complete blocks available.
    tracker->lastAllocated = 0;                                                     // Last allocated block.
    for (int i = 0;i < tracker->blockCount;i++) tracker->usage[i] = 0;
}

/**
 * @brief      Allocate a block of memory in this tracker
 *
 * @param      tracker  Memory tracker
 * @param[in]  size     Bytes to allocate.
 *
 * @return     NULL if not possible, or a pointer to the data.
 */
uint8_t *MEMAllocateBlock(MEMORYTRACKER *tracker,uint32_t size) {
    return NULL;
}

/**
 * @brief      Free a block of memory in this tracker.
 *
 * @param      tracker  Memory tracker
 * @param      memory   Address of block ; must be a returned value from
 *                      MEMAllocateBlock
 *
 * @return     true if successful ; false if not in this block.
 */
bool MEMFreeBlock(MEMORYTRACKER *tracker,uint8_t *memory) {
    return false;
}   