// *******************************************************************************************
// *******************************************************************************************
//
//      Name :       dvi_driver.c
//      Purpose :    Pico DVI Driver.
//      Date :       5th August 2025
//      Author :     Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "dvi_module.h"
#include "dvi_module_local.h"

//
//        Frame buffer, these are 3 planar bitmaps for 640x480
//

uint8_t framebuf[VIDEO_BYTES];                                                      // Bitplane memory

struct dvi_inst dvi0;                                                               // PicoDVI structure

/**
 * @brief      Main core driver
 *
 */

static uint8_t _buffer[80];                                                         // Buffer for line
static uint16_t _mapping[256];                                                      // Table mapping 320 bits to 640 bits
static uint32_t all_zero[20];
static uint32_t all_one[20];

void __not_in_flash("main") dvi_core1_main() {

    uint32_t *tmdsbuf;
    dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
    dvi_start(&dvi0);
    uint y = -1;
    uint y0 = 0;
    uint y1 = 0;
    //
    //    This table maps an 8 bit bit pattern into a 'double width' 16 bit pattern.
    //
    for (int i = 0;i < 256;i++) {
        _mapping[i] = 0;
        for (int b = 0;b < 8;b++) {
            if (i & (1 << b)) _mapping[i] |= (3 << (b*2));
        }
        _mapping[i] = (_mapping[i] >> 8) | (_mapping[i] << 8);
    }
    for (int i=0; i<20; i++) {
        all_one[i] = 0xffffffff;
        all_zero[i] = 0;
    }
    while (true) {
        y = (y + 1) % FRAME_HEIGHT;
        switch(dvi_modeInfo.mode) {
            //
            //    Mode is 640x240x8 colours as 3 bitplanes.
            //
            case MODE_640_240_8:
                queue_remove_blocking_u32(&dvi0.q_tmds_free, &tmdsbuf);
                for (uint component = 0; component < 3; ++component) {
                tmds_encode_custom_1bpp((const uint32_t*)(framebuf+(y/2)*640/8 + component * dvi_modeInfo.bitPlaneSize),
                                        tmdsbuf + (2-component) * FRAME_WIDTH / DVI_SYMBOLS_PER_WORD,   // The (2-x) here makes it BGR Acorn standard
                                        FRAME_WIDTH);
                }
                queue_add_blocking_u32(&dvi0.q_tmds_valid, &tmdsbuf);
                break;

            //
            //    Mode is 640x480x8 colours as 3 bitplanes.
            //
            case MODE_640_480_8:
                queue_remove_blocking_u32(&dvi0.q_tmds_free, &tmdsbuf);
                for (uint component = 0; component < 3; ++component) {
                tmds_encode_custom_1bpp((const uint32_t*)(framebuf+y*640/8 + component * dvi_modeInfo.bitPlaneSize),
                                        tmdsbuf + (2-component) * FRAME_WIDTH / DVI_SYMBOLS_PER_WORD,   // The (2-x) here makes it BGR Acorn standard
                                        FRAME_WIDTH);
                }
                queue_add_blocking_u32(&dvi0.q_tmds_valid, &tmdsbuf);
                break;

            //
            //    Mode is 320x240x8 colours as 3 bitplanes, also supports 320x256x8 for BBC Micro compatibility
            //
            case MODE_320_240_8:
            case MODE_320_256_8:
                queue_remove_blocking_u32(&dvi0.q_tmds_free, &tmdsbuf);
                for (uint component = 0; component < 3; ++component) {
                    uint8_t *_source = framebuf+(y0)*320/8 + component * dvi_modeInfo.bitPlaneSize;
                    uint16_t *_target = (uint16_t *)_buffer;

                    for (int i = 0;i < 320/8;i++) {
                    *_target++ = _mapping[*_source++];
                    }

                    tmds_encode_custom_1bpp((const uint32_t*)_buffer,
                                            tmdsbuf + (2-component) * FRAME_WIDTH / DVI_SYMBOLS_PER_WORD,   // The (2-x) here makes it BGR Acorn standard
                                            FRAME_WIDTH);
                }
                queue_add_blocking_u32(&dvi0.q_tmds_valid, &tmdsbuf);

                if (dvi_modeInfo.mode == MODE_320_256_8 && (y1==14))  {         // This squeezes 256 lines into 480 :)
                    y0 += 1; y1 = 0;
                } else {
                    if (y1 & 1) {
                        y0+=1;
                    }
                    y1 +=1;
                }
                if (y == 479) {
                    y0 = 0;y1 = 0;
                }
                break;

            //
            //    Mode is 320x240x64 colours
            //
            case MODE_320_240_64:
                queue_remove_blocking_u32(&dvi0.q_tmds_free, &tmdsbuf);
                for (uint component = 0; component < 3; ++component) {
                    tmds_encode_custom_2bpp((const uint32_t*)(framebuf+(y/2)*640/8 + component * dvi_modeInfo.bitPlaneSize),
                                            tmdsbuf + (2-component) * FRAME_WIDTH / DVI_SYMBOLS_PER_WORD,   // The (2-x) here makes it BGR Acorn standard
                                            FRAME_WIDTH);
                }
                queue_add_blocking_u32(&dvi0.q_tmds_valid, &tmdsbuf);
            break;

            default:
                break;
        }
    }
}
