# Screen Editor Interface

## Required Modules 

- Common module
- DVI module
- Graphics module
- Input module

## Purpose

This converts the text screen to a screen editor similar to those on the Commodore 64, you can list code and edit it live on screen.

## Further Modules

- None.

## Usage Notes

SEDInitialise() initialises the module

SEDInputLine(buffer,bufferSize) inputs a line at the keyboard ; you can type things in and when you press return the line or lines you are on are returned.

If it returns false it means the buffer may not be large enough, or a character has been corrupted ; there is no background copy of text, it is pulled from the screen bitmaps, so if you corrupt that bitmap (say draw a line across it) the character will not be recognisable.

The others allow you to create your own line input routine ; the likely reason is that there is some background updating going on. SEDInputLine() only updates the modules (e.g. the USB system)


## Revision

Written by Paul Robson, last revised 16 August 2025.
