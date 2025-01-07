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

#define HYDROSERVO_MAX_SPEED 399

#define HYDROSERVO_STATUS_OK '0'
#define HYDROSERVO_STATUS_ERROR '1'


typedef struct
{
	int32_t target_angle;
	uint16_t target_speed;

	int32_t current_angle;
	uint16_t current_speed;

	TIM_HandleTypeDef *tim_pwm;
	TIM_HandleTypeDef *tim_fb;
	uint16_t tim_channel_pwm;
	uint16_t tim_channel_fb;

	GPIO_TypeDef *direction_port;
	uint16_t direction_pin;

	uint8_t status;
}HydroServo;

void hydroservo_Init(HydroServo *servo_self, TIM_HandleTypeDef *htim_pwm,
		TIM_HandleTypeDef *htim_fb, uint16_t channel_pwm, uint16_t channel_fb, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void hydroservo_SetSpeed(HydroServo *servo_self);
int32_t hydroservo_GetAngle(HydroServo *servo_self);
void hydroservo_CallbackByFeedback(HydroServo *servo_self);

#endif /* INC_SERVO_H_ */
