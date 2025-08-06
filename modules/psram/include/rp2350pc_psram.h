// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      rp2350pc_psram.h
//      Purpose :   Interface for PSRAM on the RP2350PC board
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once

#define PSRAM_LOCATION _u(0x11000000)
#define PSRAM_SIZE      (8*1024*1024)

extern void PSRHW_SetQMITiming();
extern size_t PSRHW_Initialise(uint cs_pin);

