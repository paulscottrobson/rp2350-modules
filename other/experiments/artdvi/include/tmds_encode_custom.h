/**
 * @file       tmds_encode_custom.h
 *
 * @brief      Header file, new TMDS functions
 *
 * @author     Lennart Benschop
 *
 * @date       07/01/2025
 *
 */

#pragma once

#include "hardware/interp.h"
#include "dvi_config_defs.h"

// Functions from tmds_encode.c

void tmds_encode_custom_1bpp(const uint32_t *pixbuf, uint32_t *symbuf, size_t n_pix);
void tmds_encode_custom_2bpp(const uint32_t *pixbuf, uint32_t *symbuf, size_t n_pix);


