/*
 * Servo.h
 *
 *  Created on: Dec 18, 2024
 *      Author: 1
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include "stm32f1xx_hal.h"
#include "stdint.h"

void set_speed(uint16_t speed, TIM_HandleTypeDef *htim, uint16_t tim_channel);

#endif /* INC_SERVO_H_ */
