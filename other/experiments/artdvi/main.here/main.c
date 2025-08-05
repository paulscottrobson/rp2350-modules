// *******************************************************************************************
// *******************************************************************************************
//
//      Name :       main.c
//      Purpose :    DVI Module main demo.
//      Date :       5th August 2025
//      Author :     Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "dvi_module.h"

#include "dvi_common.h"

int main() {
    setup_default_uart();                                                           // Initialise the UART
    DVIInitialise();
    DVISetMode(MODE_320_240_8);
    DVISetMode(MODE_320_256_8);
    DVISetMode(MODE_320_240_64);
    DVISetMode(MODE_640_480_8);

    while (1) {    
    }
}
