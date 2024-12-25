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

#define TIM_FB_PERIOD 65535
#define SERVO1_PWM_TIM_CHANNEL TIM_CHANNEL_1
#define SERVO2_PWM_TIM_CHANNEL TIM_CHANNEL_2
#define SERVO1_FB_TIM_CHANNEL TIM_CHANNEL_3
#define SERVO2_FB_TIM_CHANNEL TIM_CHANNEL_4

typedef struct
{
	uint16_t angle;
	uint16_t speed;
	uint8_t dir;

	uint16_t fb_angle;
	uint16_t fb_speed;

	TIM_HandleTypeDef *htim_pwm;
	TIM_HandleTypeDef *htim_fb;
	uint16_t tim_channel_pwm;
	uint16_t tim_channel_fb;
}Servo;

//инит для структуры, передается указатель на структуру, заполняет таймеры

void SetSpeed(Servo *srv);
void InitServos(Servo *servo1, Servo *servo2);

#endif /* INC_SERVO_H_ */
