# DVI Interface

This is a complete rewrite of the DVI module and operates differently. Displays are now planar bitmaps, usually 3, but one mode has 6.

## Required Modules 

- Common module

## Purpose

This is the low level driver for DVI output. It creates a display using Core 1.

## Further Modules

- Graphics and other UI functionality.

## Usage Notes

DVIInitialise() sets up the DVI system on Core 1 and returns.

DVISetMode() has one parameter, which is the mode which can be one of the following. It returns true if successful.

| Mode            | Size                                                         |
| --------------- | ------------------------------------------------------------ |
| MODE_640_240_8  | 640 x 240 8 colours BGR                                      |
| MODE_320_240_8  | 320 x 240 8 colours BGR                                      |
| MODE_320_240_64 | 320 x 240 64 colours BBGGRR                                  |
| MODE_320_256_8  | 320 x 256 8 colours BGR, slightly tweaked for BBC Mode support |
| MODE_640_480_8  | 640 x 480 8 colours BGR                                      |

DVIGetModeInformation() returns a structure of information about the current graphics mode. This structure is documented in dvi_module.h

## Note

The source and include files (dvi_driver.c tmds_encode_custom.S and the headers) are copied from other/experiments/artdvi which is where I experiment with different modes.

## Revision

Written by Paul Robson, last revised 5 August 2025.







