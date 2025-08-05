// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      blinky.c
//      Purpose :   Blinky test program
//      Date :      5th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "stdio.h"
#include "pico/stdlib.h"

#define LED_PIN     25  

int main() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        sleep_ms(1000);
        gpio_put(LED_PIN, true);        
        sleep_ms(500);
        gpio_put(LED_PIN, false);        
    }
}   