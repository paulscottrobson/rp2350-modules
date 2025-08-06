// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      input_module_local.h
//      Purpose :   Input Manager System Header files
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#pragma once

extern INPGAMEPAD gp;

typedef struct _KeyStatus {
    uint8_t     keyID;                                                              // ID of key
    uint8_t     modifiers;                                                          // Modifier at time key is pressed.
    bool        isDown;                                                             // true when key active (down), false when inactive
    uint32_t    repeatTime;                                                         // time to repeat, if still down.
} INPKEYSTATUS;

void INPProcessKeyboardReport(USBREPORT *r);
void INPProcessMouseReport(USBREPORT *r);
void INPProcessGenericReport(USBREPORT *r);
void INPInitialiseStatus(void);
void INPHandleKeyEvent(uint8_t keyID,uint8_t modifiers);
uint16_t INPTranslateUSBCode(uint8_t keyID,uint8_t modifiers);
void INPInsertIntoQueue(int16_t key);
void INPResetQueue(void);
void INPUpdate(void);
void INPMouseInitialise(void);

extern char *locales;
extern const uint16_t localeMapping[];

//
//      Generic drivers (mostly gamepad ?)
//
void INPGeneric_081f_e401(USBREPORT *r);

