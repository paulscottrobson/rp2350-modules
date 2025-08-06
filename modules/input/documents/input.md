# Input Module

## Required Modules 

- common
- usb

## Purpose

This provides a cleaner and simpler input to the HID devices plugged into the board (Keyboard, Mouse and Gamepad)

## Further Modules

None planned.

## Usage Notes

### Keyboard

INPUpdate() needs to be called at regular intervals. This implements the keyboard repeat.

INPGetKeyboardState() returns an array of booleans of size 256, one for each USB HID Keyboard code, which is true if that key is currently pressed.

Key presses end up in a FIFO queue. This queue can be interrogated by INPIsKeyAvailable()  and INPGetKey(). Note some of the ASCII codes are more than 255

INPSetLocale() takes a two character country code which is the locale to select, the default being "us". Currently supported are uk,de,dk,fr,it,pl,no,us,se and fi .

INPSetFunctionKey() takes a function key number and text to be pushed into the keyboard queue when a function key is pressed. This functionality can be disabled by passing NULL, which returns the default, the values CTL_F1 through CTL_F12. The module does not copy the function key text, this is the responsibility of the caller.

Finally, this enables the use of Ctrl-Alt-Alt Gr as a reboot (and yes, I did own an Amiga many years ago)

### Mouse

The mouse can be interrogates using INPGetMouseStatus() which takes three pointers to int16_t that return the x y and button state respectively. The range of x is 0..1279 and y is 0..959. The buttons are bit 0: left, bit 1: middle, bit 2: right.

### Gamepad

Gamepads can be read using INPReadGamepad() which returns a gamepad structure , or NULL if no device is present. This takes one parameter, the player number (indexed from zero).

Gamepad zero is always present ; if none is plugged in then the dPad is emulated using the WASD keys and the buttons using IJKM.

The gamepad is the same format as a Super Nintendo, e.g.  a D-Pad and 4 buttons in a diamond arrangement.

Support only exists for the Olimex sold gamepad at present (this is a Super Nintendo form with a USB connector) which is device 081F:E401  . It is not difficult to add other devices and there is a 'packet sniffer' available to view what Generic HID devices are doing. Uncommenting the monitoring line in INPProcessGenericReport in gamepad/generic.c will show changes in the packet in response to button/pad changes over the serial port.


## Revision

Written by Paul Robson, last revised 26 July 2025.







