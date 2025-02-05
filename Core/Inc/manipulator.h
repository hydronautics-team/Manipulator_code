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

#define MANIPULATOR_LIMITS_OFFSET 20
#define MANIPULATOR_CALIBRATING_SPEED 1000

typedef struct
{
	HydroServo *servo_grab;
	HydroServo *servo_rotation;
}Manipulator;

void manipulator_Init(Manipulator *self, HydroServo *servo_grab, HydroServo *servo_rotation,
		hydroservoConfig servo_grab_config, hydroservoConfig servo_rotation_config);
HYDROSERVO_STATUS manipulator_Calibrate(Manipulator *self);
void manipulator_SetOrigin(Manipulator *self);
HYDROSERVO_STATUS manipulator_DoWork(Manipulator *self, int16_t servo_grab_speed, int16_t servo_rotation_speed);

#endif /* INC_MANIPULATOR_H_ */
