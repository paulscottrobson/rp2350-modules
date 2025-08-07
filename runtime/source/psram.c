// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      psram.c
//      Purpose :   Dummy PSRAM
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include <runtime.h>

uint8_t fakePSRAM[4*1024*1024];

void PSRInitialise(void) {
}

uint8_t *PSRGetMemoryAddress(void) {
    return fakePSRAM;
}

uint32_t PSRGetMemorySize(void) {
    return sizeof(fakePSRAM);
}