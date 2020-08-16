/*
	Name: utils.h
	Description: Some useful constants and macros.

	Copyright: Copyright (c) João Martins
	Author: João Martins
	Date: 19/09/2019

	µCNC is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version. Please see <http://www.gnu.org/licenses/>

	µCNC is distributed WITHOUT ANY WARRANTY;
	Also without the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the	GNU General Public License for more details.
*/

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#ifndef BYTE_OPS
#define SETBIT(x, y) ((x) |= (1 << (y)))	/* Set bit y in byte x*/
#define CLEARBIT(x, y) ((x) &= ~(1 << (y))) /* Clear bit y in byte x*/
#define CHECKBIT(x, y) ((x) & (1 << (y)))	/* Check bit y in byte x*/
#define TOGGLEBIT(x, y) ((x) ^= (1 << (y))) /* Toggle bit y in byte x*/

#define SETFLAG(x, y) ((x) |= (y))	  /* Set byte y in byte x*/
#define CLEARFLAG(x, y) ((x) &= ~(y)) /* Clear byte y in byte x*/
#define CHECKFLAG(x, y) ((x) & (y))	  /* Check byte y in byte x*/
#define TOGGLEFLAG(x, y) ((x) ^= (y)) /* Toggle byte y in byte x*/
#endif

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define ABS(a) (((a) > 0) ? (a) : -(a))

#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define __UINT32_R0__ 0
#define __UINT32_R1__ 1
#define __UINT32_R2__ 2
#define __UINT32_R3__ 3
#else
#define __UINT32_R0__ 3
#define __UINT32_R1__ 2
#define __UINT32_R2__ 1
#define __UINT32_R3__ 0
#endif

#if (defined(ENABLE_FAST_MATH) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ && __SIZEOF_FLOAT__ == 4)
//performs direct float manipulation and bit shifting. At the very end spectrum of the float (to infinity and to 0) makes aproximation either to 0 or infinity
//div2 takes about 26 clock cycles on AVR instead of 144 if multiply by 0.5f (x5.5 faster)
#define fast_flt_div2(x) ({int32_t result = (*(int32_t*)&x); if((result&0x7f800000)!=0) result-=0x00800000; else result = 0; (*(float*)&result); })
#define fast_flt_div4(x) ({int32_t result = (*(int32_t*)&x); if((result&0x7f000000)!=0) result-=0x01000000; else result = 0; (*(float*)&result); })
#define fast_flt_mul2(x) ({int32_t result = (*(int32_t*)&x); if((result&0x7f800000)!=0x7f800000) result+=0x00800000; else result |= 0x7f800000; (*(float*)&result); })
#define fast_flt_mul4(x) ({int32_t result = (*(int32_t*)&x); if((result&0x7f800000)!=0x7f000000) result+=0x01000000; else result |= 0x7f800000; (*(float*)&result); })
//Quake III based fast sqrt calculation
#define fast_sqrt(x) ({int32_t result = 0x1fbb4000 + (*(int32_t*)&x >> 1);*(float*)&result; })
#define fast_inv_sqrt(x) ({int32_t result = 0x5f3759df - (*(int32_t*)&x >> 1);*(float*)&result; })
#else
#define fast_flt_div2(x) ((x)*0.5f)
#define fast_flt_div4(x) ((x)*0.25f)
#define fast_flt_mul2(x) ((x)*2.0f)
#define fast_flt_mul4(x) ((x)*4.0f)
#define fast_sqrt(x) sqrtf(x)
#define fast_inv_sqrt(x) 1.0f / sqrtf(x)
#endif

#ifndef fast_mult10
#define fast_mult10(x) (x * 10)
#endif

#define MM_INCH_MULT 0.0393700787401574803
#define MIN_SEC_MULT 0.0166666666666666667
#define UINT8_MAX_INV 0.0039215686274509804
#endif

#define FORCEINLINE __attribute__((always_inline)) inline
