// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      mouse.c
//      Purpose :   Mouse handler
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "runtime.h"

static bool hasMousePacket = false;
static int lastX,lastY;

/**
 * @brief      Convert SDL mouse to packet information.
 */
void SYSUpdateMouse(void) {
    static uint8_t packet[9];                                                       // The mouse packet.
    static USBREPORT report;                                                        // The report.
    int x,y;

    Uint32 buttons = SDL_GetMouseState(&x,&y);                                      // Converted to absolute position
    if (hasMousePacket) {                                                           // If received a packet, convert back to an offset.
        int dx = x - lastX;
        int dy = y - lastY;
        //printf("%x %d,%d\n",buttons,dx,dy);
        for (int i = 0;i < 9;i++) packet[i] = 0;                                    // Erase the packet data.
        packet[0] = dx & 0xFF;packet[1] = (dx >> 8) & 0xFF;                         // Convert offset to packet data
        packet[2] = dy & 0xFF;packet[3] = (dy >> 8) & 0xFF;  
        packet[6] = (buttons & SDL_BUTTON(1)) ? 0xFF:0x00;                          // LMB
        packet[7] = (buttons & SDL_BUTTON(2)) ? 0xFF:0x00;                          // MMB
        packet[8] = (buttons & SDL_BUTTON(3)) ? 0xFF:0x00;                          // RMB

        report.type = 'M';                                                          // Build the USB report.
        report.vid = report.pid = 0;
        report.data = packet;report.length = 9;

        USBDispatchPacket(&report);
    }
    lastX = x;lastY = y;                                                            // Save mouse absolute and mark lastx/y as now valid.
    hasMousePacket = true;
}
