// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      locale.c
//      Purpose :   Locale Handling
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "input_module.h"
#include "input_module_local.h"
#include "usb_keycodes.h"

static uint16_t *currentLocale = NULL;                                              // The current locale name.

/**
 * @brief      Set the current locale
 *
 * @param      locale  Locale character code
 *
 * @return     True if successful.
 */
bool INPSetLocale(char *locale) {
    currentLocale = (uint16_t *)localeMapping;                                      // Pointer to locale data and locales.
    char *p = locales;

    while (*p != '\0') {                                                            // Scan for the locale.
        if (toupper(p[0]) == toupper(locale[0]) &&                                  // Locales match.
                            toupper(p[1]) == toupper(locale[1])) {
            LOG("Found locale '%s'",locale); 
            return true;
        }
        p += 3;                                                                     // Next code/data pair.
        currentLocale += 128;                                                       // 128 byte record.
    }
    currentLocale = NULL;                                                           // Not set
    LOG("No locale '%s'",locale);
    return false;
}

/**
 * @brief      Translate a USB code/modifier to ASCII according to the locale.
 *
 * @param[in]  keyID      USB Key ID
 * @param[in]  modifiers  Current state of the modifiers.
 *
 * @return     ASCII code or 0 if not translatable.
 */
uint16_t INPTranslateUSBCode(uint8_t keyID,uint8_t modifiers) {
    if (currentLocale == NULL) INPSetLocale("us");                                  // If no locale default to the US one.
    if (keyID == 0x64) keyID = 0x31;
    bool shift = (modifiers & (KEY_MOD_LSHIFT|KEY_MOD_RSHIFT)) != 0;                // Is shift key pressed
    if (currentLocale == NULL || keyID >= 64) return 0;                             // Key ID must be 0-63. SetLocale shouldn't fail.
    return currentLocale[keyID + (shift ? 64 : 0)];                                 // Extract character from the locale dependent on shift.
}
