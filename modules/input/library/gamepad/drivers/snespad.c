// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      snespad.c
//      Purpose :   Driver for the Olimex badged SNES pad 081F:E401
//      Date :      4th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "input_module.h"
#include "input_module_local.h"

/**
 * @brief      Handle report for 081f:e401
 *
 * @param      r     USB report data
 */

void INPGeneric_081f_e401(USBREPORT *r) {
    gp.known = true;                                                                // Gamepad marked as in use.
    gp.dx = gp.dy = 0;                                                              // Read DPAD
    if (r->data[0] != 0x7F) gp.dx = (r->data[0] == 0x00) ? -1 : 1;
    if (r->data[1] != 0x7F) gp.dy = (r->data[1] == 0x00) ? -1 : 1;
    gp.a = (r->data[5] & 0x20) != 0;                                                // Read buttons
    gp.b = (r->data[5] & 0x40) != 0;
    gp.x = (r->data[5] & 0x10) != 0;
    gp.y = (r->data[5] & 0x80) != 0;
}

//
//      Byte 0 : DX 7F (centred) 00 (left) FF (right)
//      Byte 1 : DY 7F (centred) 00 (up)   FF (down)
//      Byte 5 : A (bit 5) B (bit 6) X (bit 4) Y (bit 7) all logic 1 when pressed.
//      