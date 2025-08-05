# USB Interface

## Required Modules 

- Common module

## Purpose

This module is the low level interface to the USB sub system. It has three main components.

### HID Interface

HID Devices (Keyboard, Mice and other Generics, primarily game pads) are tracked, and their 'reports' - status updates are converted to a common format.

Other programs or modules can access these to (say) see what keys are pressed by installing a listener which receives those status updates.

### USB Key Interface

The system provides a stable (apparently !) interface to a USB key plugged into the board. On initialisation, this can take time to 'settle' so it is possible to initialise it in such a way that it waits for the USB Key to become available.

The key is accessed via the FATFS interface. Normally this is not used, but it can be if you want full access.

There is no support (at present) for multiple USB keys or SD Cards.

### File I/O Wrapper

A standardised I/O wrapper is provided which gives much of the functionality required to access files on the USB key. This hides the rather messy FATFS interface in something simpler. Fatfs is hidden, so any code using the wrapper does not need to include FATFS headers. You can add that header if you wish, and access it using the full power of FATFS.

## Further Modules

A module currently under consideration will provide higher level interface to the HID hardware

- converting USB Keyboard packets to ASCII with localisation
- tracking mouse positions and button presses
- providing a standardised game pad interface, (with a keyboard alternative)

## Important

If using the USB and HSTX system together, USB has to be initialised first. Experimentally if HSTX is started first, some event in the USB stack (which I do not actively control) causes HSTX to stop.

## Usage Notes

The file usb_manager.h is the main include file, the source in the usb subdirectory and the example CMake should compile with nothing other than the standard SDK.

Note that files from FATFS are copies into these working directories. So sue me.

### USB Functions

There is an initialisation method, and an update method. The update method needs to be called periodically to check for mounts and unmounts and similar. The file I/O functionality does not need this tick to be called while the I/O is taking place (e.g. ListDirectory in main.c)

USBIsFileSystemAvailable checks if File I/O can take place .... there is no guarantee you have a key plugged in.

USBInstallHandler installs a function that takes a pointer to a USBReport structure , and any reports are translated to this format and sent to it. In main.c there is a handler "_ReportHandler" which simply dumps the contents of the report, and it is installed in main()

Handlers can 'consume' the report, otherwise it is passed on to all the handlers defined, in the order they are defined, or they can leave it for another handler to process.

#### Keyboard Report

The keyboard report packs the USB standard into 8 bytes ; the first byte is the modifiers (Shift etc.) the second is reserved, and the third onwards are the standard USB data, the USB keycodes of keys that are currently depressed (up to 6). 

#### Mouse Report

The mouse report is 9 bytes long. The first 3 pairs of bytes are the adjustment for x and y position and the wheel, in low/high order. These are *offsets*, not positions. So if you want to know where the cursor is you'll have to add these values to a position.

The last 3 bytes (offset 6-8) are one byte per button (left, middle, right) which is $00 if it is up and $FF if it is down.

#### Generic Report

This covers everything else, but is primarily game pads here.  It sends the data straight from the report. This is the only report that fills in VID and PID which allows you to directly identify the vendor and product, so you know where the data is coming from and thus how to interpret it. 

Unfortunately there appears to be no standard for Gamepads.

### File I/O functions

Hopefully these are largely self explanatory - handles are now 32 bit signed integers, and the only structure used is that which returns information on a file or directory. Each I/O function returns a negative number for an error and a zero or positive number for a successful result.

There are simple examples in main.c

## Revision

Written by Paul Robson, last revised 15 July 2025.







