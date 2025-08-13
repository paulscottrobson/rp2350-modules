// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      graphics_module.h
//      Purpose :   External headers.
//      Date :      13th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#include "common_module.h"
#include "dvi_module.h"

void VDUInitialise(void);
void VDUWrite(int c);
void VDUWriteString(char *fmt, ...);
void VDUPlotCommand(int cmd,int x,int y);
void VDUSetGraphicsColour(int mode,int colour);
int  VDUReadPixel(int x,int y);
void VDUScrollRect(int ext, int direction);
void VDUGetCursor(int *x, int *y);

/**
 *      Drawing notes (from the BBC Micro user guide and the GXR user guide)
 *      
 *      GCOL
 *          First parameter : 0 Draw, 1 Or 2 And 3 Xor 4 Logical Invert [This is for FGR & BGR]
 *          Second parameter : set foreground (0-127) background (128-255)
 *      
 *      PLOT
 *      
 *      In chunks of 8.
 *          Bits 0..1   00:Move 01:Draw FGR 10:Draw Inverse 11:Draw BGR
 *          Bit 2       0: Relative to last, 1: Absolute
 *          
 *      0-7
 *              Line
 *      8-15 
 *              Line but with no final point drawn in inverting actions (GCOL 4)
 *      16-23
 *              Dotted Line
 *      24-31
 *              as 8-15 but dotted
 *      32-63
 *              Reserved for the GXR
 *      64-71
 *              Plot point
 *      72-79
 *              Horizontal flood fill until non BGR reached
 *      80-87
 *              Triangle
 *      88-95
 *              Horizontal flood fill right only.             
 *      96-103
 *              Solid Rectangle (opposite corners) [GXR]
 *      144-151
 *              Outline Circle (centre, radius) [GXR]
 *      152-159
 *              Solid Circle (centre, radius) [GXR]
 *      192-199
 *              Outline Ellipse (centre, x ext, y ext (both can be -ve so abs)) [GXR, no angled ellipse]
 *      200-207
 *              Filled Ellipse (centre, x ext, y ext (both can be -ve so abs)) [GXR, no angled ellipse]
 *      
 */
