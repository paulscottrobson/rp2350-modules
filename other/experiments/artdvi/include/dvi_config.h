/**
 * @file       dvi_config.h
 *
 * @brief      Header file, DVI hardware
 *
 * @author     Paul Robson
 *
 * @date       07/01/2025
 *
 */


#pragma once

//
//      PicoDVI Configuration
//
#define VREG_VSEL VREG_VOLTAGE_1_20
#define DVI_TIMING dvi_timing_640x480p_60hz

void DVIStart(void);

