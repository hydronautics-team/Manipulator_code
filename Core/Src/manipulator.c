/*
 * manipulator.c
 *
 *  Created on: Feb 5, 2025
 *      Author: 1
 */
#include "manipulator.h"


void manipulator_Init(Manipulator *self, hydroservo_Config servo_grab_config, hydroservo_Config servo_rotation_config)
{
	hydroservo_Init(&(self->servo_grab), servo_grab_config);
	hydroservo_Init(&(self->servo_rotation), servo_rotation_config);
}

void manipulator_SetOrigin(Manipulator *self)
{
	hydroservo_SetOrigin(&(self->servo_grab), hydroservo_GetAngleRaw(&(self->servo_grab)));
	hydroservo_SetOrigin(&(self->servo_rotation), hydroservo_GetAngleRaw(&(self->servo_rotation)));
}

hydroservo_Status manipulator_SetCalibratingSpeed(Manipulator *self, int16_t speed)
{
	if(speed == 0)
	{
		return HYDROSERVO_ERROR_INCOR_DATA;
	}
	else
	{
		self->calibrating_speed = speed;
		return HYDROSERVO_OK;
	}
}

hydroservo_Status manipulator_SetGrabSpeed(Manipulator *self, int16_t speed)
{
	if(speed == 0)
	{
			return HYDROSERVO_ERROR_INCOR_DATA;
	}
	else
	{
		self->calibrating_speed = speed;
		return HYDROSERVO_OK;
	}
}

void manipulator_SetLimitsOffset(Manipulator *self, int32_t offset)
{
	self->limits_offset = offset;
	hydroservo_SetLimitsOffset(&(self->servo_grab), offset);
}

hydroservo_Status manipulator_Calibrate(Manipulator *self)
{
	return hydroservo_Calibrate(&(self->servo_grab), self->calibrating_speed);
}

hydroservo_Status manipulator_SetSpeedGrab(Manipulator *self, int16_t speed)
{
	return hydroservo_SetSpeed(&(self->servo_grab), speed);
}

hydroservo_Status manipulator_SetSpeedRotate(Manipulator *self, int16_t speed)
{
	return hydroservo_SetSpeed(&(self->servo_rotation), speed);
}

hydroservo_Status manipulator_OpenGrab(Manipulator *self)
{
	return hydroservo_SetSpeed(&(self->servo_grab), -self->grab_speed);
}

hydroservo_Status manipulator_CloseGrab(Manipulator *self)
{
	return hydroservo_SetSpeed(&(self->servo_grab), self->grab_speed);
}
