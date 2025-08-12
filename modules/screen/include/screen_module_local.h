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

uint8_t *SCRCharAccess(uint32_t x,uint32_t y);
void SCRWrite(uint32_t x,uint32_t y,uint32_t ch);
void SCRDraw(uint32_t x,uint32_t y,uint8_t colour,uint32_t ch,bool bHighlight);
void SCRClear(void);
void SCRCopy(uint32_t xSrc,uint32_t ySrc,uint32_t xTgt,uint32_t yTgt);

#ifndef RUNTIME
#endif