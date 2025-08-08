// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      text_module.h
//      Purpose :   Text module external include
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
#include "usb_module.h"
#include "dvi_module.h"
#include "graphics_module.h"

//
//      Defines a single text window.
//
typedef struct _textWindow {
    uint16_t    xPos,yPos;                                                          // Graphic position of window
    uint8_t     textColour,backColour;                                              // Colour 
    void        *stateManager;                                                      // Function to manage state if required (void as refers to self)

    uint16_t    width,height;                                                       // Size in characters
    uint8_t     xSize,ySize;                                                        // Size of character in pixels.
    bool        isInitialised;                                                      // True when initialised.
    uint32_t    currentMode;                                                        // Mode when initialised
    bool        isCursorDrawn;                                                      // True if cursor drawn.
    uint16_t    xCursor,yCursor;                                                    // Cursor position.
} TXTWINDOW;

typedef uint16_t (*TXTSTATEMANAGER)(TXTWINDOW *window,uint16_t x,uint16_t y,uint16_t value);

void TXTInitialise(void);
void TXTWrite(uint32_t ch);
void TXTWriteWindow(TXTWINDOW *window,uint32_t ch);
TXTWINDOW *TXTDefault(void);
void TXTSetDefault(TXTWINDOW *window);
