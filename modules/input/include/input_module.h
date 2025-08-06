// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      input_module.h
//      Purpose :   Input Manager System Header files
//      Date :      2nd July 2025
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

typedef struct _GamePad {
    bool        known;                                                              // Set when gamepad has been in receipt of a legal packet.
    int16_t     dx,dy;                                                              // Main control
    bool        a,b,x,y;                                                            // 4 x control buttons.
} INPGAMEPAD;

void INPInitialise(void);
bool *INPGetKeyboardState(void);
bool INPSetLocale(char *locale);
uint32_t INPGetKey(void);
bool INPIsKeyAvailable(void);
void INPSetFunctionKey(uint8_t keyNumber,char *text);
void INPGetMouseStatus(int16_t *px,int16_t *py,int16_t *pButtons);
INPGAMEPAD *INPReadGamepad(uint32_t player);

