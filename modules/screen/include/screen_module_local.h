// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      screen_module_local.h
//      Purpose :   Screen Headers
//      Date :      9th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once

#define SCR_MAXWIDTH    (80)
#define SCR_MAXHEIGHT   (60)

void SCRRepaint(void);
uint8_t *SCRCharAccess(uint32_t x,uint32_t y);
void SCRDraw(uint32_t x,uint32_t y,uint8_t colour,bool bHighlight);

#ifndef RUNTIME
#endif