// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      text_module_local.h
//      Purpose :   Text module internal include
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************


#pragma once
#ifndef RUNTIME
#endif

void TXTCheckStatus(TXTWINDOW *txt);
void TXTWriteChar(TXTWINDOW *window,uint16_t x,uint16_t y,uint16_t ch,bool updateState);
uint16_t TXTReadChar(TXTWINDOW *window,uint16_t x,uint16_t y);