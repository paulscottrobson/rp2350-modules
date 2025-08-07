# Memory manager Module

## Required Modules 

- PSRAM

## Purpose

This manages the available SRAM left over on the Pico, and available PSRAM. 

## Further Modules

- None

## Usage Notes

The module is initialised with MEMInitialise(). 

There is a method MEMReset(n) which reserves a fixed number of bytes for system usage, this cannot be allocated or freed, and frees up *all* the remaining PSRAM or Static RAM

Memory is allocate with MEMAlloc(bytes,type) which returns an address, or NULL and freed with MEMFree(address) which returns a boolean.

Types are MEM_FAST (Pico SRAM), MEM_SLOW (PSRAM) or MEM_ANY. For MEM_ANY it will try PSRAM first.


## Revision

Written by Paul Robson, last revised 7 August 2025.
