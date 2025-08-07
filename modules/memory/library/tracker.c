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

static bool _MEMIsAllocatable(MEMORYTRACKER *tracker,uint32_t b,uint32_t reqd);

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

    if (tracker->totalSize == 0) return NULL;                                       // Nothing to allocate (usually no PSRAM).

    uint32_t firstBlock = tracker->lastAllocated;                                   // Start searching from here.
    uint32_t blocks = (size + tracker->blockSize-1) / tracker->blockSize;           // Number of blocks required

    while (!_MEMIsAllocatable(tracker,firstBlock,blocks)) {                         // While we can't allocate here.
        firstBlock = (firstBlock + 1) % tracker->blockCount;                        // Go to the next slot.
        if (firstBlock == tracker->lastAllocated) return NULL;                      // All the way round, no block big enough.
    }

    tracker->usage[firstBlock] = blocks;                                            // First entry is the size of this chunk.
    for (int i = 1;i < blocks;i++) {                                                // Mark all blocks as part of a block except first.
        tracker->usage[(firstBlock+i) % tracker->blockCount] = 0xFF;
    }
    tracker->lastAllocated = (firstBlock + blocks) % tracker->blockCount;           // Where to search from next time.
    LOG("Allocating %d",firstBlock);
    return tracker->baseAddress + firstBlock * tracker->blockSize;                  // Return address.
}

/**
 * @brief      Check to see if an area of the usage is availble.
 *
 * @param      tracker  Memory tracker
 * @param[in]  b        Start block to check
 * @param[in]  reqd     The number of consecutive clear blocks required.
 *
 * @return     { description_of_the_return_value }
 */
static bool _MEMIsAllocatable(MEMORYTRACKER *tracker,uint32_t b,uint32_t reqd) {
    for (uint32_t i = 0;i < reqd;i++) {                                             // Check this many blocks on.
        if (tracker->usage[b] != 0) return false;                                   // Block is in use.
        b++;if (b == tracker->blockCount) b = 0;                                    // Go to next block as still okay.
    }
    return true;
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
    int32_t offset = memory-tracker->baseAddress;                                   // Offset from the start of the memory block
    if (offset < 0 || offset >= tracker->totalSize) return false;                   // Not in this block.
    if (offset % tracker->blockSize != 0) ERROR("Memory free not aligned");         // It's not at the start of a block.

    uint32_t blockID = offset / tracker->blockSize;                                 // This is the first block.
    uint32_t count = tracker->usage[blockID];                                       // The number of blocks
    if (count == 0) ERROR("Memory free not allocated");                             // Validate that - developer errors.
    if (count == 0xFF) ERROR("Memory free in middle of block");

    while (count-- > 0) {                                                           // For each live block.
        tracker->usage[blockID] = 0;                                                // Free it by setting blockCount to zero.
        blockID = (blockID + 1) % tracker->blockCount;                              // Go to next.
    }
    LOG("Freed %d",offset / tracker->blockSize);
    return true;
}   

