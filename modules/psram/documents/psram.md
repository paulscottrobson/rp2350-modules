# PSRAM Hardware Interface

## Required Modules 

- Common module

## Purpose

This is a low level interface model, which abstracts the set up of PSRAM. This is only tested on the RP2350PC board, though as the initialisation code comes from the Adafruit library, it may run on others.

## Further Modules

- A module allowing proper allocation of memory


## Usage Notes

PSRInitialise sets everything up ; there are two helper function PSRGetMemoryAddress() and PSRGetMemorySize() which return the physical address and byte size respectively.


## Revision

Written by Paul Robson, last revised 1 August 2025.







