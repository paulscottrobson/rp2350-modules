// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      machine_module.c
//      Purpose :   Machine headers (external)
//      Date :      17th August 2025
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
#include "usb_module.h"
#include "dvi_module.h"
#include "psram_module.h"
#include "input_module.h"
#include "graphics_module.h"
#include "memory_module.h"
#include "screeneditor_module.h"

typedef struct _MachineInfo {
    uint32_t    inkColour, paperColour, errorColour;                                // Main elements (defaults to cyan, blue, red)
    uint32_t    mode;                                                               // The run mode. (defaults to 640x480x8)
    bool        doubleHeight;                                                       // Double height mode (defaults to true)
    uint8_t     *buffer;                                                            // Input buffer.
    uint32_t    bufferSize;                                                         // Size of input buffer.
} MACINFO;

extern MACINFO mcInfo;

void MACInitialise(void);
void MACBootDisplay(void);
void MACStart(void);
void MACSetStandardColour(void);
void MACError(char *message);