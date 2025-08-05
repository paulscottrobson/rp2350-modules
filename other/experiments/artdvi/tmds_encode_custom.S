// BSD 3-Clause License

// Copyright (c) 2021, Luke Wren
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.

// IS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// D ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// PLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// SCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// R ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// MAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// RVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// USED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"
#include "dvi_config_defs.h"

// This file contains both Arm and RISC-V source, with the correct version
// selected via the __arm__ and __riscv predefined macros. The targeted Arm
// dialect is Armv6-M, and the targeted RISC-V dialect is RV32IZba

// Offsets suitable for ldr/str (must be <= 0x7c):
#define ACCUM0_OFFS     (SIO_INTERP0_ACCUM0_OFFSET     - SIO_INTERP0_ACCUM0_OFFSET)
#define ACCUM1_OFFS     (SIO_INTERP0_ACCUM1_OFFSET     - SIO_INTERP0_ACCUM0_OFFSET)
#define ACCUM1_ADD_OFFS (SIO_INTERP0_ACCUM1_ADD_OFFSET - SIO_INTERP0_ACCUM0_OFFSET)
#define PEEK0_OFFS      (SIO_INTERP0_PEEK_LANE0_OFFSET - SIO_INTERP0_ACCUM0_OFFSET)
#define PEEK1_OFFS      (SIO_INTERP0_PEEK_LANE1_OFFSET - SIO_INTERP0_ACCUM0_OFFSET)
#define PEEK2_OFFS      (SIO_INTERP0_PEEK_FULL_OFFSET  - SIO_INTERP0_ACCUM0_OFFSET)
#define INTERP1         (SIO_INTERP1_ACCUM0_OFFSET     - SIO_INTERP0_ACCUM0_OFFSET)
// Note the entirety of INTERP0 and INTERP1 fits inside this 5-bit
// word-addressed space... almost as though it were intentional! :)

// Modifications 2025-01-02 by L.C. Benschop.
// - Renamed functions to tmds_encode_custom_1bpp
// - Keep only tmds_encode_1bpp function.
// - Add tmds_encode_2bpp. It converts a scanline of n bits into n/2 doubled
//   pixels, each 2 bits per pixel. Whereas the 1pp function converts 2 bits
//   to a pair of pixels, with different colours (each either dark or max bright)
//   the 2bpp function converts 2 bits to a pair pixels of the same colour,
//   that can be one of four levels. They only differ in the lookup table
//   they use.
	
#if defined(__arm__) && defined(__riscv)
#error "wat"
#endif

#ifdef __arm__
.syntax unified
.cpu cortex-m0plus
.thumb
#endif

.macro decl_func_x name
.section .scratch_x.\name, "ax"
.global \name
#ifdef __arm__
.type \name,%function
.thumb_func
#endif
\name:
.endm

.macro decl_func_y name
.section .scratch_y.\name, "ax"
.global \name
#ifdef __arm__
.type \name,%function
.thumb_func
#endif
\name:
.endm

#define decl_func decl_func_x


// ----------------------------------------------------------------------------
// Fast 1bpp black/white encoder (full res)

// Taking the encoder from DVI spec, with initial balance 0:
// 
// - Encoding either 0x00 or 0xff will produce a running balance of -8, with
//   output symbol of 0x100 or 0x200
// 
// - Subsequently encoding either 0x01 or 0xfe will return the balance to 0, with
//  output symbol of 0x1ff or 0x2ff
// 
// So we can do 1bpp encode with a lookup of x coordinate LSB, and input
// colour bit. If we process pixels in even-sized blocks, only the colour
// lookup is needed.

// Encode 8 pixels @ 1bpp (using two table lookups)
// r3 contains lookup mask (preshifted)
// r8 contains pointer to encode table
// 2.125 cyc/pix

#if defined(__arm__)
.macro tmds_encode_1bpp_body shift_instr0 shamt0 shift_instr1 shamt1
	\shift_instr0 r4, r2, #\shamt0
	ands r4, r3
	add r4, r8
	ldmia r4, {r4, r5}
	\shift_instr1 r6, r2, #\shamt1
	ands r6, r3
	add r6, r8
	ldmia r6, {r6, r7}
	stmia r1!, {r4, r5, r6, r7}
.endm

// r0: input buffer (word-aligned)
// r1: output buffer (word-aligned)
// r2: output pixel count
decl_func tmds_encode_custom_2bpp
	push {r4-r7, lr}
	mov r7, r8
	push {r7}
	lsls r2, #1
	add r2, r1
	mov ip, r2
	ldr r4, =tmds_2bpp_custom_table
	b 1f
	// r0: input buffer (word-aligned)
// r1: output buffer (word-aligned)
// r2: output pixel count
decl_func tmds_encode_custom_1bpp
	push {r4-r7, lr}
	mov r7, r8
	push {r7}
	lsls r2, #1
	add r2, r1
	mov ip, r2
	adr r4, tmds_1bpp_custom_table
1:	
	mov r8, r4
	// Mask: 4 bit index, 8 bytes per entry
	movs r3, #0x78
	b 2f
1:
	ldmia r0!, {r2}
#if !DVI_1BPP_BIT_REVERSE
	tmds_encode_1bpp_body lsls 3  lsrs 1
	tmds_encode_1bpp_body lsrs 5  lsrs 9
	tmds_encode_1bpp_body lsrs 13 lsrs 17
	tmds_encode_1bpp_body lsrs 21 lsrs 25
#else
	tmds_encode_1bpp_body lsrs 1   lsls 3
	tmds_encode_1bpp_body lsrs 9   lsrs 5
	tmds_encode_1bpp_body lsrs 17  lsrs 13
	tmds_encode_1bpp_body lsrs 25  lsrs 21
#endif
2:
	cmp r1, ip
	blo 1b

	pop {r7}
	mov r8, r7
	pop {r4-r7, pc}

#elif defined(__riscv)
// TODO the register allocation is not optimal here for code size
.macro tmds_encode_1bpp_body shift_instr0 shamt0 shift_instr1 shamt1
	\shift_instr0 a4, a2, \shamt0
	and a4, a4, a3
	add a4, a4, t1
	lw a5, 4(a4)
	lw a4, 0(a4)
	\shift_instr1 a6, a2, \shamt1
	and a6, a6, a3
	add a6, a6, t1
	lw a7, 4(a6)
	lw a6, 0(a6)
	sw a4, 0(a1)
	sw a5, 4(a1)
	sw a6, 8(a1)
	sw a7, 12(a1)
	addi a1, a1, 16
.endm

// a0: input buffer (word-aligned)
// a1: output buffer (word-aligned)
// a2: output pixel count
decl_func tmds_encode_custom_2bpp
	slli a2, a2, 1
	add t0, a2, a1
	la t1, tmds_2bpp_custom_table
	j 1f
	
	// a0: input buffer (word-aligned)
// a1: output buffer (word-aligned)
// a2: output pixel count
decl_func tmds_encode_custom_1bpp
	slli a2, a2, 1
	add t0, a2, a1
	la t1, tmds_1bpp_custom_table
1:	
	// Mask: 4 bit index, 8 bytes per entry
	li a3, 0x78
	bgeu a1, t0, 2f
1:
	lw a2, (a0)
	addi a0, a0, 4
#if !DVI_1BPP_BIT_REVERSE
	tmds_encode_1bpp_body slli 3  srli 1
	tmds_encode_1bpp_body srli 5  srli 9
	tmds_encode_1bpp_body srli 13 srli 17
	tmds_encode_1bpp_body srli 21 srli 25
#else
	tmds_encode_1bpp_body srli 1   slli 3
	tmds_encode_1bpp_body srli 9   srli 5
	tmds_encode_1bpp_body srli 17  srli 13
	tmds_encode_1bpp_body srli 25  srli 21
#endif
	bltu a1, t0, 1b
2:
	ret

#else
#error "Unknown architecture"
#endif

.align 2
tmds_1bpp_custom_table:
#if !DVI_1BPP_BIT_REVERSE
	.word 0x7fd00, 0x7fd00  // 0000
	.word 0x7fe00, 0x7fd00  // 0001
	.word 0xbfd00, 0x7fd00  // 0010
	.word 0xbfe00, 0x7fd00  // 0011
	.word 0x7fd00, 0x7fe00  // 0100
	.word 0x7fe00, 0x7fe00  // 0101
	.word 0xbfd00, 0x7fe00  // 0110
	.word 0xbfe00, 0x7fe00  // 0111
	.word 0x7fd00, 0xbfd00  // 1000
	.word 0x7fe00, 0xbfd00  // 1001
	.word 0xbfd00, 0xbfd00  // 1010
	.word 0xbfe00, 0xbfd00  // 1011
	.word 0x7fd00, 0xbfe00  // 1100
	.word 0x7fe00, 0xbfe00  // 1101
	.word 0xbfd00, 0xbfe00  // 1110
	.word 0xbfe00, 0xbfe00  // 1111
#else
	.word 0x7fd00, 0x7fd00  // 0000
	.word 0x7fd00, 0xbfd00  // 1000
	.word 0x7fd00, 0x7fe00  // 0100
	.word 0x7fd00, 0xbfe00  // 1100
	.word 0xbfd00, 0x7fd00  // 0010
	.word 0xbfd00, 0xbfd00  // 1010
	.word 0xbfd00, 0x7fe00  // 0110
	.word 0xbfd00, 0xbfe00  // 1110
	.word 0x7fe00, 0x7fd00  // 0001
	.word 0x7fe00, 0xbfd00  // 1001
	.word 0x7fe00, 0x7fe00  // 0101
	.word 0x7fe00, 0xbfe00  // 1101
	.word 0xbfe00, 0x7fd00  // 0011
	.word 0xbfe00, 0xbfd00  // 1011
	.word 0xbfe00, 0x7fe00  // 0111
	.word 0xbfe00, 0xbfe00  // 1111
#endif



.align 2
tmds_2bpp_custom_table:
	.word 0x7f103, 0x7f103 // 00 00
	.word 0x7f103, 0x73d30 // 01 00
	.word 0x7f103, 0xb3e30 // 10 00
	.word 0x7f103, 0xbf203 // 11 00
	.word 0x73d30, 0x7f103 // 00 01
	.word 0x73d30, 0x73d30 // 01 01
	.word 0x73d30, 0xb3e30 // 10 01
	.word 0x73d30, 0xbf203 // 11 01
	.word 0xb3e30, 0x7f103 // 00 10
	.word 0xb3e30, 0x73d30 // 01 10
	.word 0xb3e30, 0xb3e30 // 10 10
	.word 0xb3e30, 0xbf203 // 11 10
	.word 0xbf203, 0x7f103 // 00 11
	.word 0xbf203, 0x73d30 // 01 11
	.word 0xbf203, 0xb3e30 // 10 11
	.word 0xbf203, 0xbf203 // 11 11


