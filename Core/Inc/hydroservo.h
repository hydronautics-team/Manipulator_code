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

typedef enum
{
	HYDROSERVO_OK = 0,
	HYDROSERVO_ERROR = -1
}HYDROSERVO_STATUS;

typedef struct
{
	int32_t target_angle;
	int16_t target_speed;

	int32_t current_angle;
	int16_t current_speed;

	int32_t max_angle;

	TIM_HandleTypeDef *tim_pwm;
	TIM_HandleTypeDef *tim_fb;
	uint16_t tim_channel_pwm;
	uint16_t tim_channel_fb;

	uint16_t tim_pwm_period;
	uint16_t fb_period;

	GPIO_TypeDef *direction_port;
	uint16_t direction_pin;

}HydroServo;

void hydroservo_Init(HydroServo *servo_self,
		TIM_HandleTypeDef *htim_pwm,TIM_HandleTypeDef *htim_fb, uint16_t channel_pwm, uint16_t channel_fb,
		uint16_t tim_pwm_period, uint16_t fb_period,
		GPIO_TypeDef *direction_port, uint16_t direction_pin);
void hydroservo_SetSpeed(HydroServo *servo_self, int16_t speed);
int32_t hydroservo_GetAngleRaw(HydroServo *servo_self);
int32_t hydroservo_GetAngleDeciDegrees(HydroServo *servo_self);
void hydroservo_CallbackByFeedback(HydroServo *servo_self);
void hydroservo_SetOrigin(HydroServo *servo_self, int32_t origin_angle);
void hydroservo_SetAngleMax(HydroServo *servo_self, int32_t max_angle);

#endif /* INC_SERVO_H_ */
