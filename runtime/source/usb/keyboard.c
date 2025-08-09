// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      keyboard.c
//      Purpose :   Keyboard event processor.
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "runtime.h"
#include "usb_keycodes.h"
/**
 * @brief      Convert an SDL event to one suitable for the keyboard system
 *
 * @param[in]  scanCode   The scan code 
 * @param[in]  modifiers  The modifiers
 * @param[in]  isDown     Indicates if key down
 */
void KBDProcessEvent(int scanCode,int modifiers,bool isDown) {

    static uint8_t packet[8];

    //
    //      Ignore key codes for modifiers.
    //
    if (scanCode >= 224) return;

    USBREPORT report;
    //
    //      Convert the modifier to the USB format.
    //
    uint8_t usbModifier = 0;
    if (modifiers & KMOD_LCTRL) usbModifier |= KEY_MOD_LCTRL;
    if (modifiers & KMOD_RCTRL) usbModifier |= KEY_MOD_RCTRL;
    if (modifiers & KMOD_LSHIFT) usbModifier |= KEY_MOD_LSHIFT;
    if (modifiers & KMOD_RSHIFT) usbModifier |= KEY_MOD_RSHIFT;
    if (modifiers & KMOD_LALT) usbModifier |= KEY_MOD_LALT;
    if (modifiers & KMOD_RALT) usbModifier |= KEY_MOD_RALT;
    //
    //      Convert the scancode to the USB format. Handily these are the same :)
    //
    uint8_t usbScanCode = scanCode;
    //
    //      Construct a fake USB packet & report
    //
    for (int i = 0;i < 8;i++) packet[i] = 0;
    packet[0] = usbModifier;
    if (isDown) packet[2] = usbScanCode;

    report.type = 'K';
    report.vid = report.pid = 0;
    report.data = packet;report.length = 8;
    USBDispatchPacket(&report);
}
