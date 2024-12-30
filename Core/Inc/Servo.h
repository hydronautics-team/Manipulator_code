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
#define HYDROSERVO_STATUS_OK '0'
#define HYDROSERVO_STATUS_ERROR '1'

typedef struct
{
	int32_t target_angle;
	uint16_t target_speed;
	int16_t direction;

	int32_t current_angle;
	uint16_t current_speed;

	TIM_HandleTypeDef *tim_pwm;
	TIM_HandleTypeDef *tim_fb;
	uint16_t tim_channel_pwm;
	uint16_t tim_channel_fb;

	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;

	uint8_t status;
}HydroServo;

void hydroservo_Init(HydroServo *hydroservo_self, TIM_HandleTypeDef *htim_pwm,
		TIM_HandleTypeDef *htim_fb, uint16_t channel_pwm, uint16_t channel_fb, GPIO_TypeDef *GPIOx, uint16_t *GPIO_Pin);
int32_t hydroservo_GetAngle(HydroServo *hydroservo_self);
void hydroservo_Callback(HydroServo *hydroservo_self);

#endif /* INC_SERVO_H_ */
