// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      read.c
//      Purpose :   Read characters from display
//      Date :      14th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

/**
 * @brief      Read the character at text position (x,y)
 *
 * @param[in]  x     x character position
 * @param[in]  y     y character position
 *
 * @return     character code, of 0 if cannot be recognised.
 */
uint8_t VDURead(uint8_t x,uint8_t y) {
    return x;
}
