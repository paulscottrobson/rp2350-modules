// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      process.c
//      Purpose :   Converts the USB keyboard/Modifiers to ASCII.
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "input_module.h"
#include "input_module_local.h"
#include "usb_keycodes.h"

static uint16_t _INPTranslateControl(uint8_t keyID,uint8_t modifiers);

/**
 * @brief      Handle a key press event
 *
 * @param[in]  keyID      USB Key ID
 * @param[in]  modifiers  Current state of the modifiers.
 */
void INPHandleKeyEvent(uint8_t keyID,uint8_t modifiers) {

    if (keyID >= KEY_KP1 && keyID <= KEY_KP0) {                                     // Make Keypad number keys the same as 
        keyID = keyID-KEY_KP1+KEY_1;                                                // The actual number keys
    }

    uint16_t key = INPTranslateUSBCode(keyID,modifiers);                            // Translate to ASCII.

    if (key == 0) {                                                                 // Didn't work, so probably a control character.
        key = _INPTranslateControl(keyID,modifiers);
    }
    
    if (key != 0 && (modifiers & (KEY_MOD_LCTRL|KEY_MOD_RCTRL)) != 0) {             // Ctrl + (something).
        key &= 0x1F;                                                                // Lower 5 bits only of code so Ctrl+C => $03
    }
    INPInsertIntoQueue(key);                                                        // Insert key into keyboard queue.
}


/**
 * @brief      Translate a USB code/modifier to a control code.
 *
 * @param[in]  keyID      USB Key ID
 * @param[in]  modifiers  Current state of the modifiers.
 *
 * @return     Control code or 0 if not translatable.
 */
static uint16_t _INPTranslateControl(uint8_t keyID,uint8_t modifiers) {
    uint16_t ret;

    switch(keyID) {                                                                 // Map USB codes onto the internal equivalent.
        case KEY_LEFT:
            ret = CTL_LEFT;break;
        case KEY_RIGHT:
            ret = CTL_RIGHT;break;
        case KEY_DOWN:
            ret = CTL_DOWN;break;
        case KEY_UP:
            ret = CTL_UP;break;
        case KEY_PAGEUP:
            ret = CTL_PAGEUP;break;
        case KEY_PAGEDOWN:
            ret = CTL_PAGEDOWN;break;
        case KEY_BACKSPACE:
            ret = CTL_BACKSPACE;break;
        case KEY_TAB:
            ret = CTL_TAB;break;
        case KEY_ENTER:
            ret = CTL_CRLF;break;
        case KEY_ESC:
            ret = CTL_ESCAPE;break;
        case KEY_HOME:
            ret = CTL_HOME;break;
        case KEY_END:
            ret = CTL_END;break;
        case KEY_INSERT:
            ret = CTL_INSERT;break;
        case KEY_DELETE:
            ret = CTL_DELETE;break;
            break;

        default:
            if (keyID >= KEY_F1 && keyID <= KEY_F12) {                              // Convert function key codes
                ret = keyID-KEY_F1+CTL_F1;                                          // from the HID to our internal code.
            }
            break;
    }
    return ret;
}

