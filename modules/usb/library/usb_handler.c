// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      usb_handler.c
//      Purpose :   USB Interfaces
//      Date :      5th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "usb_module.h"
#include "usb_module_local.h"

static USBHANDLERFUNCTION usbReportHandlers[USBHANDLERCOUNT];

/**
 * @brief      Initialise the USB system.
 */
void USBInitialise(void) {

    static bool isInitialised = false;                                              // Only initialise once.
    if (isInitialised) return;
    isInitialised = true;

    COMInitialise();                                                                // Initialise common code.
    FSInitialise();                                                                 // Initialise the file wrapper (not a module, part of USB)
    FSCDInitialise();                                                               // Initialise the chdir() tracking.
    for (int i = 0;i < USBHANDLERCOUNT;i++) usbReportHandlers[i] = NULL;            // Remove all report handlers

    board_init();                                                                   // Most of this code comes from the tinyUSB examples
    tuh_init(BOARD_TUH_RHPORT);                                                     // init host stack on configured roothub port

    if (board_init_after_tusb) {                                                    // Your guess is as good as mine ... if it ain't broke ...
        board_init_after_tusb();
    }
    COMAddUpdateFunction(USBUpdate);                                                // It has to be updated.
}


/**
 * @brief      Install a USB Report Handler
 *
 * @param[in]  handler  The handler function
 *
 * @return     true if successfully installed. 
 */
bool USBInstallHandler(USBHANDLERFUNCTION handler) {
    for (int i = 0;i < USBHANDLERCOUNT;i++) {                                       // Find a blank entry
        if (usbReportHandlers[i] == NULL) {
            usbReportHandlers[i] = handler;                                         // Mark this as getting a report.
            return true;
        }
    }
    return false;
}
/**
 * @brief      Dispatch a report to handler or handlers.
 *
 * @param[in]  type    Report type 'K' 'M' 'G'
 * @param[in]  vid     Vendor ID
 * @param[in]  pid     Product ID
 * @param      report  Data in report
 * @param[in]  len     Length of the report in bytes.
 */
void USBDispatchReport(uint8_t type,uint16_t vid, uint16_t pid, uint8_t *report, uint16_t len) {
    bool consumed = false;
    USBREPORT r;                                                                    // Create a report record
    r.type = type;r.vid = vid;r.pid = pid;r.data = report;r.length = len; 
    for (int i = 0;i < USBHANDLERCOUNT;i++) {
        if (!consumed && usbReportHandlers[i] != NULL) {                            // If not consumed, and handler found.
            consumed = (*usbReportHandlers[i])(&r);                                 // Call the handler, and do no more if the handler consumes it.
        }
    }
}

/**
 * @brief      Update the USB System and any associated code.
 */
void USBUpdate(void) {
    static int nextUpdate = 0;
    if (COMClock() > nextUpdate) {                                                  // Limits USB update rate to 50Hz, which is fast enough to update
        nextUpdate = COMClock()+20;                                                 // the mouse. Doing it flat out crashes the DVI Driver.
        tuh_task();                                                                 // I think it's a reentrancy problem.
        USBHIDAppTask();
    }
}

/**
 * @brief      Mount callback
 *
 * @param[in]  dev_addr  The device address
 */
void tuh_mount_cb(uint8_t dev_addr) {
    LOG("A device with address %d is mounted", dev_addr);
}

/**
 * @brief      Unmount callback
 *
 * @param[in]  dev_addr  The device address
 */
void tuh_umount_cb(uint8_t dev_addr) {
    LOG("A device with address %d is unmounted", dev_addr);
}
