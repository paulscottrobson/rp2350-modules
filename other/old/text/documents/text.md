# Text Module

## Required Modules 

- common
- usb
- modes
- graphics

## Purpose

This provides a text window, one by default though more are possible, with screen editing functionality/

## Further Modules

Could be used as the basis of a screen editor.

## Usage Notes

By default there is a single text window, which can be accessed via TXTDefault (), which returns a pointer to a TXTWINDOW structure. This allows you to set a position (in graphic coordinates), a size (in character units), and the working colours.

TXTWrite() sends characters to it, which are converted to a screen editor not dissimilar to the Commodore machines. Other TXTWINDOW structures can be set up, which behave similarly, which are initialised with the TXTSetDefault() method.

By default, window state - e.g. the characters in it - are not stored. However, by defining a state manager function, the application can track what is in what cell of the text window. There is a simple example in the test application app/main.c

This affects scrolling behaviour.  If there is no state manager, when scrolling becomes necessary, the window clears and the cursor returns to the top of the window ; if there is a state manager this data is accessed to scroll normally.

## Revision

Written by Paul Robson, last revised 27 July 2025.

