// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      reports.c
//      Purpose :   Handle USB Reports
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "input_module.h"
#include "input_module_local.h"

/**
 * @brief      Handle USB HID reports, send to the appropriate code.
 *
 * @param      r     USB HID report
 *
 * @return     False as we don't consume them.
 */
static bool _INPReportHandler(USBREPORT *r) {

    switch(r->type) {                                                               // Figure out what to do with it.
        case 'K':
            INPProcessKeyboardReport(r);break;
        case 'M':
            INPProcessMouseReport(r);break;    
        case 'G':
            INPProcessGenericReport(r);break;    
        }
    return false;
}

/**
 * @brief      Initialisation of input handler.
 */
void INPInitialise(void) {
    static bool isInitialised = false;                                              // Only initialise once.
    if (isInitialised) return;
    isInitialised = true;
    gp.known = false;                                                               // Not recognised a gamepad packet.
    COMInitialise();                                                                // Common initialise
    USBInitialise();                                                                // USB Initialise.
    INPInitialiseStatus();                                                          // Initialise keyboard status.
    INPMouseInitialise();                                                           // Initialise mouse.
    USBInstallHandler(_INPReportHandler);                                           // Add a handler for USB HID reports.
    INPResetQueue();                                                                // Reset the queue.
    INPSetLocale("us");                                                             // Default US locale.                    
    COMAddUpdateFunction(INPUpdate);                                                // It has to be updated.
}

