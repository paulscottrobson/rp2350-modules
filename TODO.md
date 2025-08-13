# TODO list

## VDU module
- Test the various functions.
- Add screen read.
- VDUWrite(8) fails at home (e.g. immediately after mode change) on runtime with GPF.

## Update Bully
- To work with VDU Module.

## ScreenEditor module

- Remove all reads *except* the final extraction.
- Double height support.
- Set ink, height options etc.

## Computer module
- The base computer system

## Sprite module
- Complete reimplementation, possibly two, either xor/palette or double buffer/draw ?

## Audio ?

## Others
- python script with skeleton for runtimes CMakeLists.txt
- builder to allow libraries that aren't in the dependency list (e.g. input and usb in console, for testing keyboard.)

