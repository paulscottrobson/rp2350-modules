// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      runtime.h
//      Purpose :   Runtime header
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once

//
//      Low level stuff implemented directly.
//
#include "common_module.h"
#include "usb_module.h"
#include "dvi_module.h"
#include "psram_module.h"

#include <SDL.h>
#include <unistd.h> 
#include <sys/stat.h>
#include <errno.h>

#define AS_SCALEX 	(3)
#define AS_SCALEY   (2)

#define __in_flash()

void SYSOpen(bool muteSound);
int SYSPollUpdate(void);
void SYSClose(void);
void SYSRectangle(SDL_Rect *rc,int colour);
SDL_Surface *SYSGetSurface(void);

void RNDRender(SDL_Surface *surface);

void KBDProcessEvent(int scanCode,int modifiers,bool isDown);
void USBDispatchPacket(USBREPORT *r);
void SYSUpdateMouse(void);
void CTLFindControllers(void);

int32_t FSMapErrorCode(void);
int32_t FSGetValidateHandle(int32_t handle, bool isDirectory,void **fsObjectPtr);
void FSInitialise(void);
bool FSProcessFileName(char **pFileName);
int32_t FSAllocateRecord(bool isDirectory);
void FSFreeRecord(uint32_t handle);

#define CHECKFSAVAILABLE() {}

void SOUNDOpen(void);
void SOUNDClose(void);
void SOUNDPlay(void);
void SOUNDStop(void);
