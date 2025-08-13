// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      common_module.h
//      Purpose :   Common include files
//      Date :      5th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef DEBUG
#define LOG   COMPrint
#define ASSERT(x) if (!(x)) ERROR("Assert")
#else
#define LOG(...)    {}
#define ASSERT(x)   {}
#endif

#define ERROR(x) COMError(x,__LINE__,__FILE__)

typedef void (*COMUPDATEFUNCTION)(void);

void COMInitialise(void);
void COMPrint(char *format,...);
void COMError(char *msg,int line,char *fileName);

uint32_t COMClock(void);
uint32_t COMGetFreeSystemMemory(void);
void COMWait(uint32_t ms);

bool COMAppRunning(void);
void COMAddUpdateFunction(COMUPDATEFUNCTION updateFunc);
void COMUpdate(void);

#ifdef RUNTIME
bool SYSYield(void);
#define MAINPROGRAM MainApplication
#define KEEPINRAM(x) x
#else
#define MAINPROGRAM main
#define KEEPINRAM(x) __not_in_flash_func(x)
#endif

#ifdef RUNTIME
#define USEFLASH
#else
#include "pico/stdlib.h"
#include "hardware/watchdog.h"
#include "pico/multicore.h"
#define USEFLASH __in_flash()
#endif

//
//      BBC Micro compatible key/console codes.
//
#define CTL_NONE        (0)
#define CTL_LEFT        (8)
#define CTL_RIGHT       (9) 
#define CTL_DOWN        (10)
#define CTL_UP          (11)
#define CTL_CLEAR       (12)
#define CTL_CR          (13)
#define CTL_ESCAPE      (27)
#define CTL_HOME        (30)
#define CTL_BACKSPACE   (127)                                                       // e.g. back and erase

#define CTL_PAGEDOWN    (1)                                                         // These are BBC Micro VDU commands that make no sense
#define CTL_PAGEUP      (2)                                                         // Used for other keys.
#define CTL_END         (3)
#define CTL_INSERT      (7)
#define CTL_DELETE      (14) 
#define CTL_TAB         (15)

#define CTL_FUNCTION    (0xF000)                                                    // Fkeys are mapped onto Fk00 so 8 bit returns 00