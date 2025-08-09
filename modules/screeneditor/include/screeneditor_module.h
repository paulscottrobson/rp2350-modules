// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      screeneditor_module.h
//      Purpose :   Screen Editor Headers
//      Date :      9th August 2025
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
#include "memory_module.h"

typedef struct _ScreenEditor {
    uint32_t    x,y;                                                                // Character position on screen
    uint32_t    width,height;                                                       // Width & height in characters
    uint8_t     *charMem;                                                           // Character memory
    uint8_t     *extendLine;                                                        // Extended line flags.
} SEDSTATUS;

extern SEDSTATUS sedInfo;

void SEDInitialise(void);
void SEDReset(void);