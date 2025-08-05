## Getting Started

The picture below shows the basic set up. There is no sound or microphone connections, and the SD Card is not currently working. 

Along the top, there is a data/power connection and a DVI/HDMI connector. 

If you want to load a file onto the board using UF2 files (boot mode) by holding boot and pressing and releasing reset, then releasing boot, you need a data connection connected into the USB-C connector on the left hand side of the board beneath the switch.

The slide switch is the main power, this position (e.g. towards the USB hubs) is on.

You can see 4 USB devices plugged into the hb, a Sandisk USB key, a keyboard, and a USB mouse. The spare slot is for a game controller. Currently only the Olimex type SNES controller is supported, though it isn't difficult to add others.



<img src="./assets/wiring.jpg" alt="wiring" style="zoom: 33%;" />

## Power

There are 2 USB C sockets on the board, you can see these in the two images.  The one above has the USB cable plugged into the socket near the HDMI connector. This is for running and uploading via the debug pins.

The other one (the below picture) is plugged into the programming socket between the slide switch and the USB, this one is required for "Reset/Boot" uploading via UF2.

Note ; on my hardware it has to be set up as above (e.g. power near HDMI) or the USB times out  on start up. I have two cables normally, and just plug one in, to protect the sockets on the board.

## Debugging

The board is set up for debugging. There are two connectors from the 'standard debug device' (the little 1" square box). On the right, there are three connections to the USB serial port. On the left, the debug connector. 

The picture below is the same image with the connectors removed so you can see the colouring of the plugs in the connectors.

The RP2350PC does not come with the pins soldered in to the connector nearest the toggle switch (or my prototype didn't) so you may have to solder in 3 x 0.1" pins to make this connection.

<img src="./assets/Connections to RP2350PC.jpg" alt="Connections to RP2350PC" style="zoom: 33%;" />

## Building

Two things are required. One is the Pico SDK, which can be downloaded or git cloned. The other is the C compiler, which is the same as for the RP2040.

By default it is set up for ARM mode. To switch to RISCV mode edit pico.linux.make ; this involves changing the PLATFORM environment variable and setting the toolchain path below.

The RISC-V compiler is here https://embecosm.com/downloads/tool-chain-downloads/#corev and the usage is described here https://www.cnx-software.com/2024/08/31/using-risc-v-cores-on-the-raspberry-pi-pico-2-board-and-rp2350-mcu-from-blinking-an-led-to-building-linux/ , at the moment this is not directly available in distributions.

I used the Ubuntu 22.04 one which works fine on Arch Linux.

The locations of these are in pico.linux.make in the environment directory. 

If anyone would fix this for Windows (can't abide it) or Macs (not a billionaire) it would be much appreciated.

There is a 3 file build in 'experimental', blinky, which blinks the green LED on the board. This is probably the best way to check.

OpenOcd is also required if you are going to upload via the debugger rather than rebooting, which is a bit long winded even with a reset and boot button on the board. I think the one downloaded from the Raspi website is required.  

If you are uploading from the debugger you have to have previously booted it using a UF2 and the file system using the core processor you want to use - so to switch from ARM to RISCV you have to upload a RISCV built UF2 manually. 

There are provided UF2s for 'blinky' - this just flashes the green LED for ever in the other/prebuilt directory.

## Booting Up

The boot up has a period of about 5 seconds or so where nothing seems to be happening. This is because the HSTX doesn't seem to like the USB booting. 

As feedback during this boot, the Green LED on the board will flash. This indicates it is waiting for the USB to stabilise.  Shortly after that the monitor will sync.

## Or not booting up

Sometimes when the altcore library is used, it doesn't boot up - the display doesn't work. I *think* this is to do with HSTX not initialising correctly (by not being in reset state ?). Power cycling or pressing the reset button, or simply reuploading seems to solve it and it seems to only happen when uploading via the debugger.

## Code

This is a brief description on how the module/runtime system worksModules

The code is not a big lump like previously, it is divided into hierarchical modules. 

Each module has a simple demo (except common) in app/main.c which shows the various functionalities.

These are designed to do the bare minimum to abstract the hardware ; the user can use as much or as little as they want. Plans include for example a function to provide standard modes (the DVI module just does enough to generate video), and graphics functions on top of that etc.

Each function is build using cmake. There is a Makefile which has several options 

- make cmake : this rebuilds the Make structure so you can actually build it
- make compile : this compiles the code
- make upload : this uploads it to the board using openocd
- make monitor : this starts the serial TTY program picocom to see what's coming off the board
- make all : does compile upload monitor to run a program

There is nothing for PSRAM, the Codec chip, or PWM audio yet because I haven't looked at it.

### Runtime

The runtime, which is under development, is a PC/SDL based version of the low level modules (e.g. common, dvi and usb) so that programs can be developed without having to upload all the time. It's not an emulator (it's not speed limited, and some things won't work), but it's quite good for fast testing.

Paul Robson

19 July 2025