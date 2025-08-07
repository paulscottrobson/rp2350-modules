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
//      The control codes are here because they are used for input and console, and I don't want a dependency
//      on either.
//
#define CTL_NONE        (0)
#define CTL_LEFT        (1)
#define CTL_RIGHT       (2) 
#define CTL_DOWN        (3)
#define CTL_UP          (4)
#define CTL_PAGEDOWN    (5)
#define CTL_PAGEUP      (6)
#define CTL_BACKSPACE   (8)                                                         // e.g. back and erase
#define CTL_TAB         (9) 
#define CTL_CRLF        (10)                                                        // This matches the PICO C compiler.
#define CTL_CLEAR       (12)
#define CTL_CR          (13)
#define CTL_F1          (14)                                                        // 14-25 are the function keyaas.
#define CTL_F12         (25)
#define CTL_ESCAPE      (27)

#define CTL_HOME        (28)
#define CTL_END         (29)
#define CTL_INSERT      (30)
#define CTL_DELETE      (31)                                                        // e.g. erase at cursor

