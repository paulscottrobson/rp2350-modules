# Graphics Module

## Required Modules

- DVI

## Purpose

This module provides basic graphics functionality ; lines, shapes text and so on. It borrows the 'VDU' idea from Acorn machines, where a byte/word stream of commands perform drawing operations, set the screen up etc.

The Acorn definition of this is in the document 'vdu commands.md', and also borrows partly from the BBC Micro Graphics extension ROM.

## Further Modules

- Add more/better functionality in the graphics
- Add a sprite system.

## Usage Notes

The graphics functionality takes place in a Window which is 1280 x 960 or 1280x1024 (the latter for 256 line mode.

All graphics operations take place within a window which is by default the whole screen. There is also a text window which works similarly for text. The text and graphics system are mostly seperate (apart from being on the same display), however the facility to draw characters in graphics mode does exist.

Graphics commands work on a sequence of coordinates, as previous commands are tracked. So, for example, to draw a filled triangle, you would Move to one point, move to a second point, and draw to the third point. If you follow this with another triangle draw command, it will use the second, third and the newest point.

## Changes from Standards

Printer commands (1,2,3) Beep (7), Page Mode (14,15) are not implemented and won't be. 19 (Redefine logical colour) is not used because there is at the time of writing no palette.

## Functions

These are fairly straightforward ; VDUInitialise() does the set up, defaulting to 640x480 mode. VDUWrite() VDUWriteWord()  and VDUWriteString() send data to the graphics system.

VDUPlotCommand() VDUSetGraphicsColour() are convenient shorthands.

VDUReadPixel() reads a pixel on the display, VDUScrollRect() scrolls a rectangular area of the display, and VDUGetTextCursor()/VDUSetTextCursor() read and write the current text cursor position.


## Revision

Written by Paul Robson, last revised 13 August 2025.
