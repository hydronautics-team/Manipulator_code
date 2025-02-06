/*
 * manipulator.h
 *
 *  Created on: Feb 5, 2025
 *      Author: 1
 */

#ifndef INC_MANIPULATOR_H_
#define INC_MANIPULATOR_H_

#include "stm32f1xx_hal.h"
#include "stdint.h"
#include "hydroservo.h"

typedef struct
{
	HydroServo servo_grab;
	HydroServo servo_rotation;

	int16_t calibrating_speed;
	int16_t grab_speed;
	int32_t limits_offset;
}Manipulator;

void manipulator_Init(Manipulator *self, hydroservo_Config servo_grab_config, hydroservo_Config servo_rotation_config);

void manipulator_SetOrigin(Manipulator *self);
hydroservo_Status manipulator_SetCalibratingSpeed(Manipulator *self, int16_t speed);
hydroservo_Status manipulator_SetGrabSpeed(Manipulator *self, int16_t speed);
void manipulator_SetLimitsOffset(Manipulator *self, int32_t offset);

hydroservo_Status manipulator_Calibrate(Manipulator *self);
hydroservo_Status manipulator_SetSpeedGrab(Manipulator *self, int16_t speed);
hydroservo_Status manipulator_SetSpeedRotate(Manipulator *self, int16_t speed);
hydroservo_Status manipulator_OpenGrab(Manipulator *self);
hydroservo_Status manipulator_CloseGrab(Manipulator *self);

#endif /* INC_MANIPULATOR_H_ */
