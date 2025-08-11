// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      usb_module_local.h
//      Purpose :   USB code common header files.
//      Date :      5th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once

#ifndef RUNTIME
#include "pico/stdlib.h"
#include "hardware/watchdog.h"
#include "bsp/board_api.h"
#include <ff.h>
#include "tusb.h"
#include "diskio.h"
#endif

#define CHECKFSAVAILABLE() if (!USBIsFileSystemAvailable()) return FSERR_STORAGE    // Wait FS, error if times out.

#define INPUSBKEY_TIMEOUT   (10*1000)                                               // USB Key timeout, in ms.
#define USBHANDLERCOUNT     (4)                                                     // Max # report handlers supported

void USBHIDAppTask(void);
void USBDispatchReport(uint8_t type,uint16_t vid, uint16_t pid, uint8_t *report, uint16_t len);
uint32_t FSGetValidateHandle(uint32_t handle, bool isDirectory,void **fsObjectPtr);
void USBUpdate(void);

void FSInitialise(void);
bool FSProcessFileName(char **pFileName);
uint32_t FSAllocateRecord(bool isDirectory);
void FSFreeRecord(uint32_t handle);
void FSCDInitialise(void);
char *FSCDMapCurrentName(char *name);

#ifndef RUNTIME
uint32_t FSMapErrorCode(FRESULT res);
#endif

#define USE_BLINK_FEEDBACK
#define BLINK_LED_PIN     25                                                        // Blinked during USB boot. 


