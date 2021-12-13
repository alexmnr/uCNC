/*
	Name: boarddefs.h
	Description: Defines the available board types.

	Copyright: Copyright (c) João Martins
	Author: João Martins
	Date: 07/02/2020

	µCNC is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version. Please see <http://www.gnu.org/licenses/>

	µCNC is distributed WITHOUT ANY WARRANTY;
	Also without the implied warranty of	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the	GNU General Public License for more details.
*/

#ifndef BOARDDEFS_H
#define BOARDDEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/boards/boards.h"
#include "hal/mcus/mcus.h"

/*
	MCU port map
*/
#ifndef BOARD
#error Undefined board
#endif

#if (BOARD)
#else
#error Invalid board configuration
#endif

#if (BOARD == BOARD_UNO)
#define MCU MCU_AVR
#include "hal/boards/avr/boardmap_uno.h"
#endif

#if (BOARD == BOARD_MKS_DLC)
#define MCU MCU_AVR
#include "hal/boards/avr/boardmap_mks_dlc.h"
#endif

#if (BOARD == BOARD_RAMBO14)
#define MCU MCU_AVR
#include "hal/boards/avr/boardmap_rambo14.h"
#endif

#if (BOARD == BOARD_RAMPS14)
#define MCU MCU_AVR
#include "hal/boards/avr/boardmap_ramps14.h"
#endif

#if (BOARD == BOARD_BLUEPILL)
#define MCU MCU_STM32F10X
#include "hal/boards/stm32/boardmap_bluepill.h"
#endif

#if (BOARD == BOARD_MZERO)
#define MCU MCU_SAMD21
#include "hal/boards/samd21/boardmap_mzero.h"
#endif

#if (BOARD == BOARD_ZERO)
#define MCU MCU_SAMD21
#include "hal/boards/samd21/boardmap_zero.h"
#endif

#if (BOARD == BOARD_VIRTUAL)
#endif

#ifndef BOARD
#error Undefined board
#endif

#include "hal/mcus/mcudefs.h" //configures the MCU for the selected board

#ifdef __cplusplus
}
#endif

#endif
