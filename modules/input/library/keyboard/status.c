// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      status.c
//      Purpose :   Tracks key presses and autorepeat.
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "input_module.h"
#include "input_module_local.h"
#include "usb_keycodes.h"

#define MAXKEYDOWN      (8)                                                         // Max # of simultaneous keys.

static uint8_t lastKeyPressed;                                                      // Index of last key pressed.
static INPKEYSTATUS keys[MAXKEYDOWN];                                               // Keys currently down.
static bool keyState[256];                                                          // State of every key 

static uint16_t repeatDelay = 500;                                                  // ms before delay starts.
static uint16_t repeatRate = 70;                                                    // ms for each repeat

/**
 * @brief      Initialise the status manager.
 */
void INPInitialiseStatus(void) {
    lastKeyPressed = 0;
    for (int i = 0;i < MAXKEYDOWN;i++) keys[i].isDown = false;                      // No keys currently down.
    for (int i = 0;i < 256;i++) keyState[i] = false;        
}

/**
 * @brief      Read the keyboard state. This contains one boolean for each of
 *             the 256 possible values.
 *
 * @return     Boolean array, 1 per USB HID Keyboard code.
 */
bool *INPGetKeyboardState(void) {
    return keyState;
}

/**
 * @brief      Process a keyboard report, converts into keyboard events
 *
 * @param      r     Keyboard report.
 */
void INPProcessKeyboardReport(USBREPORT *r) {

    #define REBOOT_MASK (KEY_MOD_LCTRL|KEY_MOD_LALT|KEY_MOD_RALT)                   // Reboot on CTRL Alt Alt

    //
    //      In the runtime, we don't need this, so we don't bother compiling it at all.
    //
    #ifndef RUNTIME
    if ((r->data[0] & REBOOT_MASK) == REBOOT_MASK) {                                // Check for C/A/A reboot.
        watchdog_enable(1,1);                                                       // Enable the watchdog timer
        while (true) {}                                                             // Wait for it to be not-reset.
    }
    #endif

    //
    //      Check if any keys currently considered down have been released.
    //
    for (int i = 0;i < MAXKEYDOWN;i++) {                                            // For each key thought to be down.
        if (keys[i].isDown) {
            bool stillDown = false;                                                 // Check if it is still down.
            for (int j = 2;j < r->length;j++) {
                if (r->data[j] == keys[i].keyID) stillDown = true;
            }
            if (!stillDown) {                                                       // Key released.
                keys[i].isDown = false;                                             // Mark released.
            }
        }
    }

    //
    //      Check for any new key presses. If found, set up a new record for that new key pressed.
    //
    for (int j = 2;j < r->length;j++) {                                             // Look for keys that are down.
        if (r->data[j] != 0) {
            bool isKnown = false;                                                   // Is there a record for it already.
            for (int i = 0;i < MAXKEYDOWN;i++) {
                if (keys[i].isDown && keys[i].keyID == r->data[j]) isKnown = true;
            }
            if (!isKnown) {                                                         // New key down.
                uint8_t emptyRec = 0xFF;                                            // Find a slot for it.
                for (int i = 0;i < MAXKEYDOWN;i++) {
                    if (!keys[i].isDown) emptyRec = i;
                }
                if (emptyRec != 0xFF) {                                             // Not pressing 8 keys at once :)
                    keys[emptyRec].keyID = r->data[j];                              // Create the new key state record.
                    keys[emptyRec].isDown = true;
                    keys[emptyRec].modifiers = r->data[0];
                    keys[emptyRec].repeatTime = COMClock() + repeatDelay;
                    lastKeyPressed = emptyRec;                
                    INPHandleKeyEvent(keys[emptyRec].keyID,                         // Convert to ASCII and push into queue.
                                                keys[emptyRec].modifiers);
                }
            }
        }
    }
    //
    //      Update the key state array which is a boolean for each key , recording wheteher it is is up or down.
    //
    for (int i = 0;i < 256;i++) keyState[i] = false;                            
    for (int j = 2;j < r->length;j++) {
        if (r->data[j] != 0) keyState[r->data[j]] = true;
    }
}

/**
 * @brief      Update timed events, implements the keyboard repeat.
 */
void INPUpdate(void) {    
    //
    //      Check for repeats.
    //
    if (keys[lastKeyPressed].isDown &&                                              // Last key pressed is still down, and repeat expired.
            COMClock() > keys[lastKeyPressed].repeatTime) {
                INPHandleKeyEvent(keys[lastKeyPressed].keyID,                       // Convert to ASCII again and put in the queue.
                                            keys[lastKeyPressed].modifiers);
                keys[lastKeyPressed].repeatTime = COMClock() + repeatRate;          // Time for next repeat, much less than the initial one.
    }
}
