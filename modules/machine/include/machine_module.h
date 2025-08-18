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
#include <ctype.h>

#include "common_module.h"
#include "usb_module.h"
#include "dvi_module.h"
#include "psram_module.h"
#include "input_module.h"
#include "graphics_module.h"
#include "memory_module.h"
#include "screeneditor_module.h"

typedef int (*MACCOMMANDHANDLER)(int argc,char **argv);                             // Command handler

typedef struct _MachineInfo {
    uint32_t    inkColour, paperColour, errorColour;                                // Main elements (defaults to cyan, blue, red)
    uint32_t    mode;                                                               // The run mode. (defaults to 640x480x8)
    bool        doubleHeight;                                                       // Double height mode (defaults to true)
    uint8_t     *buffer;                                                            // Input buffer.
    uint32_t    bufferSize;                                                         // Size of input buffer.
    uint8_t     argumentCount;                                                      // Number of arguments.
    char        **arguments;                                                        // Array of arguments, like 'main'
    uint8_t     handlerCount;                                                       // Number of command handlers
    MACCOMMANDHANDLER *commandHandlers;                                             // Command handlers.
} MACINFO;

extern MACINFO mcInfo;

void MACInitialise(void);
void MACBootDisplay(void);
void MACStart(void);
void MACSetStandardColour(void);
void MACError(char *message);
uint8_t MACParseInput(char *p,char **arguments,int argumentMax);                
void MACAddCommandHandler(MACCOMMANDHANDLER handler);
int MACLogCommands(int argc,char **argv);

int MACOSCommand(int argc,char **argv);                                             // Execute an OS command (parsed)
int MACOSCommandString(char *cmd);                                                  // Not parsed
