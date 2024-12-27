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
#include "main.h"

#define TIM_FB_PERIOD 65535
#define TIM_PWM_COUNTER 399
#define SERVO1_PWM_TIM_CHANNEL TIM_CHANNEL_1
#define SERVO2_PWM_TIM_CHANNEL TIM_CHANNEL_2
#define SERVO1_FB_TIM_CHANNEL TIM_CHANNEL_3
#define SERVO2_FB_TIM_CHANNEL TIM_CHANNEL_4
#define ServoGapK 10

typedef struct
{
	int32_t angle;
	uint16_t speed;
	int16_t direction;

	int32_t fb_angle;
	uint16_t fb_speed;

	TIM_HandleTypeDef *tim_pwm;
	TIM_HandleTypeDef *tim_fb;
	uint16_t tim_channel_pwm;
	uint16_t tim_channel_fb;

	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
}Servo;

void InitServo(Servo *srv, TIM_HandleTypeDef *htim_pwm, TIM_HandleTypeDef *htim_fb, uint16_t channel_pwm, uint16_t channel_fb, GPIO_TypeDef *GPIOx, uint16_t *GPIO_Pin);

void SetDirection(Servo *srv);
void SetSpeed(Servo *srv);
void ResetSpeed(Servo *srv);

void ErrorHandlerServo(Servo *srv);
void Rotate(Servo *srv);
void RotateByAngle(Servo *srv);
void CalibrateServo(Servo *srv);

//void SetPwm(TIM_HandleTypeDef *htim, uint16_t tim_channel, uint16_t speed);

#endif /* INC_SERVO_H_ */
