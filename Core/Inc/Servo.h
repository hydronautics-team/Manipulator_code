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

typedef struct
{
	uint16_t angle;
	uint16_t speed;
	uint16_t fb;
	uint8_t dir;
}Servo;

void set_speed(uint16_t speed, TIM_HandleTypeDef *htim, uint16_t tim_channel);
void read_fb(uint16_t period, Servo *servo);

#endif /* INC_SERVO_H_ */
