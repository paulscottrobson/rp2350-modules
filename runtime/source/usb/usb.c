// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      usb.c
//      Purpose :   USB functions
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include <runtime.h>

static int handlerCount = 0;
static USBHANDLERFUNCTION handlers[8];

/**
 * @brief      Initialise the USB system.
 */
void USBInitialise(void) {
    handlerCount = 0;
}


/**
 * @brief      Add a new packet handler
 *
 * @param[in]  handler  packet handler
 *
 * @return     true if successful.
 */
bool USBInstallHandler(USBHANDLERFUNCTION handler) {
    if (handlerCount == 8) return false;
    handlers[handlerCount++] = handler;
    return true;
}

/**
 * @brief      Dispatch packet to handlers
 *
 * @param      r     Packet to dispatch.
 */
void USBDispatchPacket(USBREPORT *r) {
    // printf("%c %04x:%04x ",r->type,r->vid,r->pid);
    // for (int i = 0;i < r->length;i++) printf("%02x ",r->data[i]);
    // printf("(%d)\n",r->length);

    bool consumed = false;
    for (int i = 0;i < handlerCount;i++) {
        if (!consumed) {
            consumed = (*handlers[i])(r);
        }
    }
}
