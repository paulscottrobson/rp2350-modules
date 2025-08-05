// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      hid_handler.c      
//      Purpose :   Handles HID devices
//      Date :      5th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "usb_module.h"
#include "usb_module_local.h"

#define MAX_REPORT  4                                                               // Max reports per HID device

static struct                                                                       // Reports for each device.
{
    uint8_t report_count;
    tuh_hid_report_info_t report_info[MAX_REPORT];
} hid_info[CFG_TUH_HID];

static void process_kbd_report(hid_keyboard_report_t const *report);
static void process_mouse_report(hid_mouse_report_t const * report);
static void process_generic_report(uint8_t dev_addr, uint8_t instance,uint16_t vid,uint16_t pid, uint8_t const* report, uint16_t len);

/**
 * @brief      Called every USB tick, but doesn't actually do anything at the moment.
 */
void USBHIDAppTask(void) {
}

// *******************************************************************************************
// 
//                                  CALLBACKS & DISPATCHERS
// 
// *******************************************************************************************

/**
 * @brief      Invoked when HID device mounted.
 *
 * @param[in]  dev_addr     The dev address
 * @param[in]  instance     The instance
 * @param      desc_report  The description report
 * @param[in]  desc_len     The description length
 */
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len)
{
    LOG("HID device address = %d, instance = %d is mounted", dev_addr, instance);

    // Interface protocol (hid_interface_protocol_enum_t)
    const char* protocol_str[] = { "None", "Keyboard", "Mouse" };
    uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
    LOG("HID Interface Protocol = %s", protocol_str[itf_protocol]);

    // By default host stack will use activate boot protocol on supported interface.
    // Therefore for this simple example, we only need to parse generic report descriptor (with built-in parser)
    if ( itf_protocol == HID_ITF_PROTOCOL_NONE ) {
        hid_info[instance].report_count = tuh_hid_parse_report_descriptor(hid_info[instance].report_info, MAX_REPORT, desc_report, desc_len);
        LOG("HID has %u reports", hid_info[instance].report_count);
    }

    // request to receive report
    // tuh_hid_report_received_cb() will be invoked when report is available
    if ( !tuh_hid_receive_report(dev_addr, instance) ) {
        LOG("Error: cannot request to receive report");
    }
}

/**
 * @brief      Invoked when device with hid interface is un-mounted
 *
 * @param[in]  dev_addr  The dev address
 * @param[in]  instance  The instance
 */
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance)
{
  LOG("HID device address = %d, instance = %d is unmounted", dev_addr, instance);
}


/**
 * @brief      Invoked when received report from device via interrupt endpoint
 *
 * @param[in]  dev_addr  The dev address
 * @param[in]  instance  The instance
 * @param      report    The report
 * @param[in]  len       The length
 */
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
    uint16_t vid, pid;
    tuh_vid_pid_get(dev_addr, &vid, &pid);

    uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

    switch (itf_protocol) {
        case HID_ITF_PROTOCOL_KEYBOARD:
            process_kbd_report( (hid_keyboard_report_t const*) report );
            break;

        case HID_ITF_PROTOCOL_MOUSE:
            process_mouse_report( (hid_mouse_report_t const*) report );
            break;

        default:
            // Generic report requires matching ReportID and contents with previous parsed report info
            process_generic_report(dev_addr, instance, vid,pid, report, len);
            break;
    }

    // continue to request to receive report
    if ( !tuh_hid_receive_report(dev_addr, instance)) {
        LOG("Error: cannot request to receive report");
    }
}

// *******************************************************************************************
// 
//                                          KEYBOARD
// 
// *******************************************************************************************

/**
 * @brief      Process a keyboard report, if you can call it that. 
 *
 * @param      report   report to process
 */

static void process_kbd_report(hid_keyboard_report_t const *report)
{
    static uint8_t keyboardReport[8];                                               // Unpack into a simple 8 byte buffer.
    keyboardReport[0] = report->modifier;                                           // Okay, this could change but it isn't going to anytime soon.
    keyboardReport[1] = report->reserved;
    for (int i = 0;i < 6;i++) {
        keyboardReport[i+2] = report->keycode[i];
    }
    USBDispatchReport('K',0,0,keyboardReport,sizeof(keyboardReport));                // Tell the handler about it.
}

// *******************************************************************************************
// 
//                                          MOUSE
// 
// *******************************************************************************************

/**
 * @brief      Process the mouse report
 *
 * @param      report  report
 */
static void process_mouse_report(hid_mouse_report_t const * report)
{
    uint8_t mouseReport[9];
    mouseReport[0] = report->x & 0xFF;
    mouseReport[1] = report->x >> 8;
    mouseReport[2] = report->y & 0xFF;
    mouseReport[3] = report->y >> 8;
    mouseReport[4] = report->wheel & 0xFF;
    mouseReport[5] = report->wheel >> 8;
    mouseReport[6] = (report->buttons & MOUSE_BUTTON_LEFT) ? 0xFF:0x00;
    mouseReport[7] = (report->buttons & MOUSE_BUTTON_MIDDLE) ? 0xFF:0x00;
    mouseReport[8] = (report->buttons & MOUSE_BUTTON_RIGHT) ? 0xFF:0x00;
    USBDispatchReport('M',0,0,mouseReport,sizeof(mouseReport));
}

// *******************************************************************************************
// 
//                                      ALL OTHER DEVICES
// 
// *******************************************************************************************

/**
 * @brief      Process generic reports - this usually means gamepads.
 *
 * @param[in]  dev_addr  The dev address
 * @param[in]  instance  The instance
 * @param[in]  vid       Vendor ID
 * @param[in]  pid       Product ID
 * @param      report    The report
 * @param[in]  len       The length
 */
static void process_generic_report(uint8_t dev_addr, uint8_t instance,uint16_t vid,uint16_t pid, uint8_t const* report, uint16_t len)
{
    (void) dev_addr;

    uint8_t const rpt_count = hid_info[instance].report_count;
    tuh_hid_report_info_t* rpt_info_arr = hid_info[instance].report_info;
    tuh_hid_report_info_t* rpt_info = NULL;

    if ( rpt_count == 1 && rpt_info_arr[0].report_id == 0) {
        // Simple report without report ID as 1st byte
        rpt_info = &rpt_info_arr[0];
    } else {
        // Composite report, 1st byte is report ID, data starts from 2nd byte
        uint8_t const rpt_id = report[0];

        // Find report id in the array
        for(uint8_t i=0; i<rpt_count; i++) {
            if (rpt_id == rpt_info_arr[i].report_id) {
                rpt_info = &rpt_info_arr[i];
                break;
            }
        }
        report++;
        len--;
    }

    if (rpt_info != NULL) {
        USBDispatchReport('G',vid,pid,(uint8_t *)report,len);
    } else {
        LOG("Couldn't find report info !");
        return;        
    } 

    // For complete list of Usage Page & Usage checkout src/class/hid/hid.h. For examples:
    // - Keyboard                     : Desktop, Keyboard
    // - Mouse                        : Desktop, Mouse
    // - Gamepad                      : Desktop, Gamepad
    // - Consumer Control (Media Key) : Consumer, Consumer Control
    // - System Control (Power key)   : Desktop, System Control
    // - Generic (vendor)             : 0xFFxx, xx
    if ( rpt_info->usage_page == HID_USAGE_PAGE_DESKTOP) {
        switch (rpt_info->usage) {
            case HID_USAGE_DESKTOP_KEYBOARD:
                // Assume keyboard follow boot report layout
                process_kbd_report( (hid_keyboard_report_t const*) report );
                break;

            case HID_USAGE_DESKTOP_MOUSE:
                // Assume mouse follow boot report layout
                process_mouse_report( (hid_mouse_report_t const*) report );
                break;

        }
    }
}
