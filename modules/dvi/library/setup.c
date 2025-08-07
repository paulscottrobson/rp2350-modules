// *******************************************************************************************
// *******************************************************************************************
//
//      Name :       setup.c
//      Purpose :    Pico DVI Driver setup.
//      Date :       7th August 2025
//      Author :     Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "dvi_module.h"
#include "dvi_module_local.h"

/**
 *   This is the pinout for the RP2350PC board.
 */
static struct dvi_serialiser_cfg olimex_rp2350_cfg = {
    .pio = DVI_DEFAULT_PIO_INST,
    .sm_tmds = {0, 1, 2},
    .pins_tmds = {12, 18, 16}, 
    .pins_clk = 14,
    .invert_diffpairs = false
};

/**
 * @brief      Start the DVI driver on Core1.
 */
void DVIInitialise(void) {
    static bool isInitialised = false;
    if (isInitialised) return;
    isInitialised = true;
    COMInitialise();

    DVISetMode(MODE_640_240_8);
    for (int i = 0;i < VIDEO_BYTES;i++) framebuf[i] = rand();

    vreg_set_voltage(VREG_VSEL);                                                    // Set CPU voltage
    sleep_ms(10);                                                                   // Let it settle for 0.01s
    set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);                                // Set the DVI compatible clock speed

    dvi0.timing = &DVI_TIMING;                                                      // Select timing and pinout
    dvi0.ser_cfg = olimex_rp2350_cfg;

    dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());    // Initialise DVI
    multicore_launch_core1(dvi_core1_main);                                         // Run DVI driver on core #1
    stdio_init_all();                                                               // Reinitialise UART as CPU speed has changed.
}
