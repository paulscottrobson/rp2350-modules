// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      serial.c
//      Purpose :   Serial test program
//      Date :      5th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "stdio.h"
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    sleep_ms(1000);
    printf("Starting.\n");
    printf("End.\n");
    while (true) {
        printf("Hello, world !\n");
        sleep_ms(500);
        sleep_ms(500);
    }
}   