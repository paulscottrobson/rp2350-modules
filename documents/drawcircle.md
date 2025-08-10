# DrawCircle walkthrough.

This is a walkthrough showing you how to create an application which ... draws a circle. Okay, it's not very exciting, but it should help you get started.

This is designed for the Olimex board. I haven't tried it on any other boards .... I know Adafruit are doing something very similar, but I do not have one.

None of it is particularly complex, it may be rather longwinded for people with more experience, but it's designed so it can be done without too much knowledge. It does require the ability to use the Command Line though.

## Prerequisites

- Working hardware. The debugger/serial is not required for this. It might actually confuse things, like RISCV, so I'd be inclined to do it the easy way via boot mode to start with.
- Development software. I would do this in ARM first not because of preference, but the ARM compilers are more accessible and easier to install at present. Once this works, changing to RISCV is straightforward.

## Downloading the SDK

At the time of writing, PicoDVI will not compile on v2.2.0 of the pico SDK (this has been raised as an issue). So you may want to download your own version if you use 2.2.0 for anything else.

To download it

*git clone https://github.com/raspberrypi/pico-sdk --recursive*

To set it back to version 2.2.1

*cd pico-sdk*

*git checkout bddd20f*

It should announce "HEAD is now at bddd20f SDK 2.1.1 Release" at the bottom of the text following the last command.

## Downloading PicoDVI as well (if you didn't first time)

If you cloned this somewhere, which you likely have as you are reading this, you may have not cloned PicoDVI at the same time, it is a submodule of this repository. Easy check - is there anything in the PicoDVI subdirectory.

*git submodule update --recursive*

should install this missing module. I think sometimes it might need init rather than update.

## Switching the board to ARM mode.

We are building an ARM version for the moment. This means the board needs to be running ARM if we are using the debugger (if you are uploading via boot mode you don't need to do this)

Move the USB C connector to the socket near the USB hub.

Put the board into boot mode by holding the boot button and pressing and releasing reset. This should pop up a window called "RP2350" which you can copy the file other/prebuild/blinky_arm.uf2 to.  The green LED should start flashing.

Move the USB C connector back to the socket near the HDMI (not, honest guv) connector.

## Setting up the environment

The environment directory contains setup files for Linux, macOS and (hopefully eventually) Windows. 

You need to check the file pico.linux.make or pico.macOS.make depending on your operating system.

1) Check the target is set to ARM, and the RISCV is commented out (e.g. TARGET=ARM)
2) Set the path to your 2.1.1 repository (the export PICO_SDK_PATH line)

There are options later on to set the serial port and its application and the upload via debugger parameters, which you can ignore for now.

## Building Blinky.

So to check all this works, we're going to build blinky, which is in other/experiments/blinky, which does what blinky apps always do, blink an on-board LED.

This requires two steps.

### CMake

CMake is used to set up the installation, this needs to be done the first time and whenever you change between ARM and RISCV. The makefile does all the stuff about creating builds etc. for you.

I'd be half inclined to edit blinky.c because this is actually the test program used above. If you change the timings in 21/23 to 250 then you'll know it's worked because the blinks will be much quicker.

*make cmake*

You don't need to do this if you simply edit the source, it is required for additional source files or library changes.

### Building

Once CMaked (?), it needs compiling.

*make compile*

At the end of this you should have a 'blinky.uf2' file in the build subdirectory.  

You upload this in the same way as you did in 'Switching to ARM mode', switch the cables, put it into boot mode with reset/boot, and copy the file over. It should reboot and start flashing again, which it was doing anyway. 

(If you have the debugger you can use *make upload* which will load this without the boot/reset thing which is much quicker if you are developing on it)

## Building Input.

So having done that, we are going to do the same thing with one of the module 'input' which takes the raw HID data from USB and makes it useable.

Switch to the modules/input directory:

*make cmake*

*make compile*

as before and copy the uf2 file from the build directory (input.uf2) to the board using boot/reset . It should display a blue screen with some red buttons and a pattern in the middle. 

The buttons represents a gamepad (the Olimex model, which is a SNES like keypad which plugs into USB) ; if you don't plug in a gamepad this is keys WASD IJKL and if you press these keys on a keyboard the buttons should go yellow.

If you plug in a USB mouse you should be able to move the pattern about as if it's the world's worst painting program (excepting anything with Clippy in it)

## Creating a new application

Next stage is to create our ultra exciting circle drawing program. This requires the use of builder.py which requires Python3.

### Creation

Go to the module directory, which should have builder.py in it. Run

*python3 builder.py -ocircle graphics*

This should create a new directory circle, which has one dependent module, graphics. Graphics itself has dependencies (dvi and common) which is handled automatically.

If you need to rerun this program (builder.py) it will not let you overwrite existing code, so you will need to delete the directory. This isn't normally necessary. 

Even if you forget to add a library, it is not difficult to add to CMakeLists.txt (and dependencies.info) - just add it to the includes, the file(GLOB_RECURSE bit and the sources list

### First Build

First up, we'll build this module. So

*make cmake*

*make compile*

and check it works. You can't upload yet, well you can, but it won't do anything.

## Adding in the source code

Add the following code into app/main.c in the MAINFUNCTION function.

    GFXInitialise();                                                                
    GFXDraw(Mode,MODE_320_240_8,0);     
    GFXDraw(RawColour,3,0);
    GFXDraw(Move,10,10);
    GFXDraw(FillEllipse,300,200);
    while (COMAppRunning()) {                                                                      COMUpdate();                         
    }

## Building and uploading.

*make compile* and either boot/reset copying or uploading via debug (as before) should run this program.

You should get a yellow circle on a black background. Well, an ellipse.  An explanation of the lines in order.

1. Initialises the graphics system (and any subsystems)
2. Sets the display mode to 320x240 pixels, 8 colours. Other modes are available.
3. Sets the "raw colour" to 3 (Yellow) foreground, 0 (Black) background the colours are the same as the BBC Micro in 8 colour mode (BGR) ; there is a 64 colour mode (BBGGRR) as well.
4. Moves the drawing cursor to position 10,10
5. Draws a filled ellipse in the rectangle formed by that position and 300,200

You can experiment with these if you like.

The purpose of the last three lines is two fold.

1. It repeatedly calls COMUpdate() which updates things that ... need updating, like the USB system. This should be called in any loop. 
2. It allows the runtime program, which is a sort of emulator/simulator of the hardware, to exit and refresh, as it's written using the SDL library.

## Doing it on the Runtime

The runtime is a sort of "emulator". It emulates the hardware functionality, not the processor itself. It is not speed limited, so it will do some things much faster than the RP2350 will, so while it is a useful tool for development if you are doing a lot of work check it does work on the hardware.

To make it run "circle" do the following (I'll tidy this up as it's a bit rubbish)

1. Switch to the "runtime" directory
2. Edit CMakeLists.txt
3. Look for the line file(GLOB_RECURSE APP_SOURCES "${MODULEDIR}/screeneditor/app/*.[csS]") - the 'screeneditor' may change, but the APP_SOURCES should be easy to find.
4. Change 'screeneditor' to 'circle' (e.g. take sources from the circle directory)
5. Change the line below that from 'screeneditor' (or whatever) to 'circle'

Then do *make cmake* as before, but after that *make run* and you should see the yellow ellipse again ... unless you changed it, in which case you'll get whatever you changed it to. 

You can exit this using the ESC key, or clicking the close button.

## Changing to RISCV

To build these under RISC-V

1. Change the TARGET= in pico.linux.make or pico.macOS.make to RISCV
2. Make  PICO_TOOLCHAIN_PATH and PICO_RISCV_TOOLCHAIN_PATH point to the RISC-V compilers. Information on getting these is in the 'getting started' document.
3. If using the debugger reboot the board using boot/reset and upload the RISC-V version of blinky into it (other/prebuild/blinky_riscv.uf2)
4. Build using make cmake and make compile, then upload by the preferred method.

At the time of writing running under RISC-V seems identical, and everything seems to work. 

------

Paul Robson

10 August 2025

