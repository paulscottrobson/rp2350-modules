# ScreenEditor Module

## Required Modules 

- Common
- USB
- DVI
- Input
- Memory
- PSRAM

## Purpose

This provides a Commodore (PET, Vic20 etc.) style screen editor

## Further Modules

- None

## Usage Notes

SEDInitialise() initialises it. 

SEDReset() adopts it for the current mode, assuming the screen editor occupies the whole of the screen, sets the default colours and puts the cursor at (0,0). It does not physically change the display. Post calling SEDReset(), the sedInfo structure can be modified to not use the full screen, use different colours etc, this is done in the demo program.

SEDClear() clears the display to all background. 

A line is input with the following code :

`while (SEDProcess(buffer,sizeof(buffer),INPGetKey())) COMUpdate();`                         

This repeatedly calls the 'process' method, feeding in any pressed keys, while simultaneously updating the system. Note the COMUpdate() can contain other code that runs in the background, so to speak if the application needs it. This is why this is not wrapped automatically.

The loop ends either when ENTER is pressed, or if the runtime is terminated if that is the platform. You can see an example of this in main.c in modules/screeneditor/app

When it finishes it loads one or more lines of screen text into the buffer as an ASCIIZ string ; the first two parameters are the pointer to a buffer and it's physical size. A line is deemed multiline when a character is printed in the right most column causing it to drop to the next line.


## Revision

Written by Paul Robson, last revised 9 August 2025.









