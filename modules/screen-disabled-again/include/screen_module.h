// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      screenr_module.h
//      Purpose :   Screen  Headers
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

typedef struct _ScreenInfo {
    int32_t     x,y;                                                                // Character position on screen
    uint32_t    width,height;                                                       // Width & height in characters
    int32_t     xCursor,yCursor;                                                    // Cursor position.
    uint8_t     *charMem;                                                           // Character memory
    uint8_t     *extendLine;                                                        // Extended line flags.
    uint8_t     ink;                                                                // Text colour.
    uint32_t    currentMode;                                                        // Current display mode.
} SCRSTATUS;

extern SCRSTATUS scrInfo;

void SCRInitialise(void);
void SCRReset(void);
bool SCRProcess(uint8_t *buffer,uint32_t size,uint32_t key);
void SCRGetLine(uint8_t *buffer,uint32_t size);
void SCRWriteChar(uint32_t key);
