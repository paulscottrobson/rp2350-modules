# rp2350-modules
------

To get started, read the getstarted.md document in the documents directory.

------

This is a user library for the RP2350PC from Olimex. It should work for any other RP2350 based board that uses the standard connections.

The board specification is :

- RP2350 Pico 2 chip
- 4 Port USB hub
- SD Card on board.
- HDMI / DVI connector
- 16Mb Flash
- 8Mb PSRAM
- Stereo audio either using PWM
- ES8311 Audio Codec

## *Read This*

At the time of writing PIcoDVI does *not* compile under the current version of Pico SDK (2.2.0) https://github.com/Wren6991/PicoDVI/issues/91 

So it requires 2.1.1 which you can obtain by the following ; you may want to have this and 2.2.0

- git clone https://github.com/raspberrypi/pico-sdk --recursive
- cd pico-sdk
- git checkout bddd20f

This should display "HEAD is now at bddd20f SDK 2.1.1 Release" plus a lot of other verbiage.

The recursive is required because tinyusb is used. If you forget it,as I do regularly try git submodule update --recursive

