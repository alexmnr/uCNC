/*
		Name: kinematic_cartesian.c
		Description: Implements all kinematics math equations to translate the motion of a cartesian machine.
				Also implements the homing motion for this type of machine.

		Copyright: Copyright (c) João Martins
		Author: João Martins
		Date: 26/09/2019

		µCNC is free software: you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, either version 3 of the License, or
		(at your option) any later version. Please see <http://www.gnu.org/licenses/>

		µCNC is distributed WITHOUT ANY WARRANTY;
		Also without the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
		See the	GNU General Public License for more details.
*/

#include "../../cnc.h"

#if (KINEMATIC == KINEMATIC_CARTESIAN)
#include <stdio.h>
#include <math.h>

void kinematics_init(void)
{
}

void kinematics_apply_inverse(float *axis, int32_t *steps)
{
	for (uint8_t i = 0; i < AXIS_COUNT; i++)
	{
		steps[i] = (int32_t)lroundf(g_settings.step_per_mm[i] * axis[i]);
	}
}

void kinematics_apply_forward(int32_t *steps, float *axis)
{
	for (uint8_t i = 0; i < AXIS_COUNT; i++)
	{
		axis[i] = (((float)steps[i]) / g_settings.step_per_mm[i]);
	}
}

uint8_t kinematics_home(void)
{
	float target[AXIS_COUNT];



#ifndef HOMING_CYCLE_0_AXIS_MASK
#define HOMING_CYCLE_0_AXIS_MASK         AXIS_Z_HOMING_MASK
#define HOMING_CYCLE_0_ERROR             KINEMATIC_HOMING_ERROR_Z
#define HOMING_CYCLE_0_LIMIT_MASK        LINACT2_LIMIT_MASK
#endif  // HOMING_CYCLE_0_AXIS_MASK

#ifndef ENABLE_XY_SIMULTANEOUS_HOMING

#ifndef HOMING_CYCLE_1_AXIS_MASK
#define HOMING_CYCLE_1_AXIS_MASK         AXIS_X_HOMING_MASK
#define HOMING_CYCLE_1_ERROR             KINEMATIC_HOMING_ERROR_X
#define HOMING_CYCLE_1_LIMIT_MASK        LINACT0_LIMIT_MASK
#endif  // HOMING_CYCLE_1_AXIS_MASK

#ifndef HOMING_CYCLE_2_AXIS_MASK
#define HOMING_CYCLE_2_AXIS_MASK         AXIS_Y_HOMING_MASK
#define HOMING_CYCLE_2_ERROR             KINEMATIC_HOMING_ERROR_Y
#define HOMING_CYCLE_2_LIMIT_MASK        LINACT1_LIMIT_MASK
#endif  // HOMING_CYCLE_2_AXIS_MASK

#ifndef HOMING_CYCLE_3_AXIS_MASK
#define HOMING_CYCLE_3_AXIS_MASK         0
#define HOMING_CYCLE_3_ERROR             /*not-used*/
#define HOMING_CYCLE_3_LIMIT_MASK        /*not-used*/
#endif  // HOMING_CYCLE_3_AXIS_MASK

#else // ENABLE_XY_SIMULTANEOUS_HOMING

#ifndef HOMING_CYCLE_1_AXIS_MASK
#define HOMING_CYCLE_1_AXIS_MASK         (AXIS_X_HOMING_MASK | AXIS_Y_HOMING_MASK)
#define HOMING_CYCLE_1_ERROR             KINEMATIC_HOMING_ERROR_XY
#define HOMING_CYCLE_1_LIMIT_MASK        (LINACT0_LIMIT_MASK | LINACT1_LIMIT_MASK)
#endif  // HOMING_CYCLE_1_AXIS_MASK

#ifndef HOMING_CYCLE_2_AXIS_MASK
#define HOMING_CYCLE_2_AXIS_MASK         AXIS_X_HOMING_MASK
#define HOMING_CYCLE_2_ERROR             KINEMATIC_HOMING_ERROR_X
#define HOMING_CYCLE_2_LIMIT_MASK        LINACT0_LIMIT_MASK
#endif  // HOMING_CYCLE_2_AXIS_MASK

#ifndef HOMING_CYCLE_3_AXIS_MASK
#define HOMING_CYCLE_3_AXIS_MASK         AXIS_Y_HOMING_MASK
#define HOMING_CYCLE_3_ERROR             KINEMATIC_HOMING_ERROR_Y
#define HOMING_CYCLE_3_LIMIT_MASK        LINACT1_LIMIT_MASK
#endif  // HOMING_CYCLE_3_AXIS_MASK

#endif // ENABLE_XY_SIMULTANEOUS_HOMING

#ifndef HOMING_CYCLE_4_AXIS_MASK
#define HOMING_CYCLE_4_AXIS_MASK         AXIS_A_HOMING_MASK
#define HOMING_CYCLE_4_ERROR             KINEMATIC_HOMING_ERROR_A
#define HOMING_CYCLE_4_LIMIT_MASK        LINACT3_LIMIT_MASK
#endif  // HOMING_CYCLE_4_AXIS_MASK

#ifndef HOMING_CYCLE_5_AXIS_MASK
#define HOMING_CYCLE_5_AXIS_MASK         AXIS_B_HOMING_MASK
#define HOMING_CYCLE_5_ERROR             KINEMATIC_HOMING_ERROR_B
#define HOMING_CYCLE_5_LIMIT_MASK        LINACTB_LIMIT_MASK
#endif  // HOMING_CYCLE_5_AXIS_MASK

#ifndef HOMING_CYCLE_6_AXIS_MASK
#define HOMING_CYCLE_6_AXIS_MASK         AXIS_C_HOMING_MASK
#define HOMING_CYCLE_6_ERROR             KINEMATIC_HOMING_ERROR_C
#define HOMING_CYCLE_6_LIMIT_MASK        LINACT5_LIMIT_MASK
#endif  // HOMING_CYCLE_5_AXIS_MASK

// Home the axes in the order specified 0,1,2,3,4,5
// corresponding to the X,Y,Z,A,B,C axes mapped to each
// of them.

#ifndef DISABLE_ALL_LIMITS
#if HOMING_CYCLE_0_AXIS_MASK != 0
	if (mc_home_axis(HOMING_CYCLE_0_AXIS_MASK, HOMING_CYCLE_0_LIMIT_MASK))
	{
		return HOMING_CYCLE_0_ERROR;
	}
#endif

#if HOMING_CYCLE_1_AXIS_MASK != 0
	if (mc_home_axis(HOMING_CYCLE_1_AXIS_MASK, HOMING_CYCLE_1_LIMIT_MASK))
	{
		return HOMING_CYCLE_1_ERROR;
	}
#endif

#if HOMING_CYCLE_2_AXIS_MASK != 0
	if (mc_home_axis(HOMING_CYCLE_2_AXIS_MASK, HOMING_CYCLE_2_LIMIT_MASK))
	{
		return HOMING_CYCLE_2_ERROR
	}
#endif


#if HOMING_CYCLE_3_AXIS_MASK != 0
	if (mc_home_axis(HOMING_CYCLE_3_AXIS_MASK, HOMING_CYCLE_3_LIMIT_MASK))
	{
		return HOMING_CYCLE_3_ERROR;
	}
#endif

#if HOMING_CYCLE_4_AXIS_MASK != 0
	if (mc_home_axis(HOMING_CYCLE_4_AXIS_MASK, HOMING_CYCLE_4_LIMIT_MASK))
	{
		return HOMING_CYCLE_4_ERROR;
	}
#endif

#if HOMING_CYCLE_5_AXIS_MASK != 0
	if (mc_home_axis(HOMING_CYCLE_5_AXIS_MASK, HOMING_CYCLE_5_LIMIT_MASK))
	{
		return HOMING_CYCLE_5_ERROR;
	}
#endif

#if HOMING_CYCLE_6_AXIS_MASK != 0
	if (mc_home_axis(HOMING_CYCLE_6_AXIS_MASK, HOMING_CYCLE_6_LIMIT_MASK))
	{
		return HOMING_CYCLE_6_ERROR;
	}
#endif

	cnc_unlock(true);
	motion_data_t block_data = {0};
	mc_get_position(target);

	for (uint8_t i = 0; i < AXIS_COUNT; i++)
	{
		target[i] += ((g_settings.homing_dir_invert_mask & (1 << i)) ? -g_settings.homing_offset : g_settings.homing_offset);
	}

	block_data.feed = g_settings.homing_fast_feed_rate;
	block_data.spindle = 0;
	block_data.dwell = 0;
	// starts offset and waits to finnish
	mc_line(target, &block_data);
	itp_sync();
#endif

#ifdef SET_ORIGIN_AT_HOME_POS
	memset(target, 0, sizeof(target));
#else
for (uint8_t i = AXIS_COUNT; i != 0;)
{
	i--;
	target[i] = (!(g_settings.homing_dir_invert_mask & (1 << i)) ? 0 : g_settings.max_distance[i]);
}
#endif

	// reset position
	itp_reset_rt_position(target);

	return STATUS_OK;
}

bool kinematics_check_boundaries(float *axis)
{
	if (!g_settings.soft_limits_enabled || cnc_get_exec_state(EXEC_HOMING))
	{
		return true;
	}

	for (uint8_t i = AXIS_COUNT; i != 0;)
	{
		i--;
#ifdef SET_ORIGIN_AT_HOME_POS
		float value = !(g_settings.homing_dir_invert_mask & (1 << i)) ? axis[i] : -axis[i];
#else
		float value = axis[i];
#endif
		if (value > g_settings.max_distance[i] || value < 0)
		{
			return false;
		}
	}

	return true;
}

#endif
