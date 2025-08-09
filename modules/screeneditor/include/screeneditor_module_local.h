// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      screeneditor_module_local.h
//      Purpose :   Screen Editor Headers
//      Date :      9th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once

#define SED_MAXWIDTH    (80)
#define SED_MAXHEIGHT   (50)

void SEDRepaint(void);
uint8_t *SEDCharAccess(uint32_t x,uint32_t y);
void SEDDraw(uint32_t x,uint32_t y,uint8_t colour,bool bHighlight);

#ifndef RUNTIME
#endif