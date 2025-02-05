/*
 * manipulator.c
 *
 *  Created on: Feb 5, 2025
 *      Author: 1
 */
#include "manipulator.h"


void manipulator_Init(Manipulator *self, HydroServo *servo_grab, HydroServo *servo_rotation,
		hydroservoConfig servo_grab_config, hydroservoConfig servo_rotation_config)
{
	self->servo_grab = servo_grab;
	self->servo_rotation = servo_rotation;

	hydroservo_Init(self->servo_grab, servo_grab_config);
	hydroservo_Init(self->servo_rotation, servo_rotation_config);
}

HYDROSERVO_STATUS manipulator_Calibrate(Manipulator *self)
{
	HYDROSERVO_STATUS status = hydroservo_Calibrate(self->servo_grab, MANIPULATOR_CALIBRATING_SPEED);
	if(!status)
	{
		hydroservo_SetLimitsOffset(self->servo_grab, MANIPULATOR_LIMITS_OFFSET);
		return status;
	}
	else
	{
		return status;
	}
}

void manipulator_SetOrigin(Manipulator *self)
{
	hydroservo_SetOrigin(self->servo_grab, hydroservo_GetAngleRaw(self->servo_grab));
	hydroservo_SetOrigin(self->servo_rotation, hydroservo_GetAngleRaw(self->servo_rotation));
}

HYDROSERVO_STATUS manipulator_DoWork(Manipulator *self, int16_t servo_grab_speed, int16_t servo_rotation_speed)
{
	HYDROSERVO_STATUS status = hydroservo_SetSpeed(self->servo_grab, servo_grab_speed);
	if(!status)
	{
		return hydroservo_SetSpeed(self->servo_rotation, servo_rotation_speed);
	}
	else
	{
		return status;
	}
}
