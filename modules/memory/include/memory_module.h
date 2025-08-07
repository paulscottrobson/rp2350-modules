// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      memory_module.h
//      Purpose :   Memory manager header
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "common_module.h"
#include "psram_module.h"

#define MEM_FAST        (0x01)
#define MEM_SLOW        (0x02)
#define MEM_ANY         (MEM_FAST|MEM_SLOW)

void MEMInitialise(void);
uint8_t *MEMReset(uint32_t requiredSRAMMemory);
uint8_t *MEMAlloc(uint32_t ramSize,uint32_t ramType);
bool MEMFree(uint8_t *address);
