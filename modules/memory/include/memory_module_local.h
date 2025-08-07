// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      memory_module_local.h
//      Purpose :   Memory manager internal header
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************


#pragma once

#define MAXBLOCKS   (254)                                                           // Fixed # of blocks for now.

//
//      This structure tracks the usage of all the memory, it divides it up into MAXBLOCKS chunks of equal size.
//      There is no garbage collection and no compaction.
//
typedef struct _MemoryUsageTracker {
    uint8_t     *baseAddress;                                                       // Base address of memory.
    uint32_t    totalSize;                                                          // Total size in bytes (0 if nothing)
    uint32_t    blockCount;                                                         // Count of blocks here.
    uint32_t    blockSize;                                                          // Size of each blocks.
    uint32_t    lastAllocated;                                                      // Last block allocated.
    uint8_t     usage[MAXBLOCKS];                                                   // Usage for each block.
} MEMORYTRACKER;

//
//      Usage
//          0       : Block is not in use
//          1-254   : Allocated block here, (usage) blocks
//          255     : Block is part of an allocated block.
//
void MEMInitialiseBlock(MEMORYTRACKER *tracker,uint8_t *address,uint32_t size);
uint8_t *MEMAllocateBlock(MEMORYTRACKER *tracker,uint32_t size);
bool MEMFreeBlock(MEMORYTRACKER *tracker,uint8_t *memory);

#ifndef RUNTIME
#endif
