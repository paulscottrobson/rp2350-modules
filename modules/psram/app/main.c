// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   PSRAM test program
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "psram_module.h"

static uint32_t sreg = 0;

/**
 * @brief      Reset the LFSR random seed
 */
void lfsrReset(void) {
    sreg = 0;
}

/**
 * @brief      Get the next number in the LFSR sequence
 *
 * @return     { description_of_the_return_value }
 */
uint8_t lfsr(void) {
    uint32_t bit;
    /* polynomial: x^24 + x^23 + x^22 + x^20 + x^19 + x^18 + x^17 + x^16 + x^15 + x^13 + x^12 + x^8 + x^7 + x^6 + 1 */
    bit = ((sreg >> 7) ^ (sreg >> 8) ^ (sreg >> 9) ^ (sreg >> 11) ^ (sreg >> 12) ^ (sreg >> 13) ^ (sreg >> 14) ^ (sreg >> 15) ^ (sreg >> 16) ^ (sreg >> 18) ^ (sreg >> 19) ^ (sreg >> 23) ^ (sreg >> 24) ^ (sreg >> 25))& 1u;
    sreg = (sreg >> 1) | (bit << 31);
    sreg = (sreg == 0) ? 0x1234 : sreg;
    return sreg & 0xFF;
}

void wait(uint32_t ms) {
    uint32_t end = COMClock() + ms;
    while (COMClock() < end) { COMUpdate(); }
}


int MAINPROGRAM(int argc,char *argv[]) {
    PSRInitialise();
    uint8_t *psRAM = PSRGetMemoryAddress();
    wait(500);
    LOG("Starting.");    
    uint32_t count = 4096;
    LOG("Testing %d of %d",count,PSRGetMemorySize());
    lfsrReset();
    for (int i = 0;i < count;i++) {
        psRAM[i] = lfsr();
    }

    lfsrReset();
    for (int i = 0;i < count;i++) {
        uint8_t n = lfsr();
        if (n != psRAM[i]) LOG("%d %d %d",i,psRAM[i],n);
    }

    while (COMAppRunning()) {
        LOG("End.");
        wait(1000);
    }
}   