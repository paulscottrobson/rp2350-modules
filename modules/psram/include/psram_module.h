// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      psram_module.h
//      Purpose :   PSRAM External module
//      Date :      6th August 2025
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

void PSRInitialise(void);
uint8_t *PSRGetMemoryAddress(void);
uint32_t PSRGetMemorySize(void);