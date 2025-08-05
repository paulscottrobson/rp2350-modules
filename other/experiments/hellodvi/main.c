#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"

#include "dvi.h"
#include "dvi_serialiser.h"
#include "common_dvi_pin_configs.h"

#include "testcard_320x240_rgb565.h"

// DVDD 1.2V (1.1V seems ok too)
#define FRAME_WIDTH 320
#define FRAME_HEIGHT 240
#define VREG_VSEL VREG_VOLTAGE_1_20
#define DVI_TIMING dvi_timing_640x480p_60hz

// // Olimex RP2040PC, RP2040-Pico-PC, Neo6502, 
// static struct dvi_serialiser_cfg olimex_rp2040_cfg = {
//     .pio = DVI_DEFAULT_PIO_INST,
//     .sm_tmds = {0, 1, 2},
//     .pins_tmds = {14, 18, 16}, // D0- D1- D2-
//     .pins_clk = 12,            // CLK
//     .invert_diffpairs = true
// };

// Olimex RP2040PC, RP2040-Pico-PC, Neo6502, 
static struct dvi_serialiser_cfg olimex_rp2350_cfg = {
    .pio = DVI_DEFAULT_PIO_INST,
    .sm_tmds = {0, 1, 2},
    .pins_tmds = {12, 18, 16}, 
    .pins_clk = 14,
    .invert_diffpairs = false
};


struct dvi_inst dvi0;

void core1_main() {
	dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
	while (queue_is_empty(&dvi0.q_colour_valid))
		__wfe();
	dvi_start(&dvi0);
	dvi_scanbuf_main_16bpp(&dvi0);
}

int main() {
	vreg_set_voltage(VREG_VSEL);
	sleep_ms(10);
	set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);

	setup_default_uart();

	dvi0.timing = &DVI_TIMING;
	dvi0.ser_cfg = olimex_rp2350_cfg;
	dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());

	// Core 1 will wait until it sees the first colour buffer, then start up the
	// DVI signalling.
	multicore_launch_core1(core1_main);

	// Pass out pointers into our preprepared image, discard the pointers when
	// returned to us. Use frame_ctr to scroll the image
	uint frame_ctr = 0;
	while (true) {
		for (uint y = 0; y < FRAME_HEIGHT; ++y) {
			uint y_scroll = (y + frame_ctr) % FRAME_HEIGHT;
			const uint16_t *scanline = &((const uint16_t*)testcard_320x240)[y_scroll * FRAME_WIDTH];
			queue_add_blocking_u32(&dvi0.q_colour_valid, &scanline);
			while (queue_try_remove_u32(&dvi0.q_colour_free, &scanline))
				;
		}
		++frame_ctr;
	}
}
