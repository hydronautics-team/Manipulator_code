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

//стоит ли писать error перед названием ошибки?

#define HYDROSERVO_NO_MAX_ANGLE 2147483647
#define HYDROSERVO_NO_MIN_ANGLE -2147483647

typedef enum
{
	HYDROSERVO_OK = 0,
	HYDROSERVO_ERROR_TIMEOUT = 1,
	HYDROSERVO_ERROR_LIMITS = 2
}HYDROSERVO_STATUS;

typedef struct
{
	int32_t target_angle;
	int16_t target_speed;
	int32_t current_angle;
	uint16_t current_speed;
	uint8_t fb_flag;

	int32_t max_angle;
	int32_t min_angle;
	uint16_t fb_impulse_per_rotate;

	TIM_HandleTypeDef *tim_pwm;
	TIM_HandleTypeDef *tim_fb;
	uint16_t tim_channel_pwm;
	uint16_t tim_channel_fb;
	uint16_t tim_pwm_period;
	uint16_t tim_fb_period;
	uint16_t fb_buffer;

	GPIO_TypeDef *direction_port;
	uint16_t direction_pin;

}HydroServo;

void hydroservo_Init(HydroServo *self, TIM_HandleTypeDef *htim_pwm, TIM_HandleTypeDef *htim_fb,
		uint16_t channel_pwm, uint16_t channel_fb, uint16_t tim_pwm_period, uint16_t tim_fb_period, uint16_t fb_impulse_per_rotate,
		GPIO_TypeDef *direction_port, uint16_t direction_pin);
HYDROSERVO_STATUS hydroservo_SetSpeed(HydroServo *self, int16_t speed);
int32_t hydroservo_GetAngleRaw(HydroServo *self);
int32_t hydroservo_GetAngleDeciDegrees(HydroServo *self);
void hydroservo_CallbackPeriodElapsed(HydroServo *self);
void hydroservo_CallbackByFeedback(HydroServo *self);
void hydroservo_SetOrigin(HydroServo *self, int32_t origin_angle);
void hydroservo_SetAngleMax(HydroServo *self, int32_t max_angle);
void hydroservo_SetAngleMin(HydroServo *self, int32_t min_angle);
int32_t hydroservo_GetAngleMax(HydroServo *self);
int32_t hydroservo_GetAngleMin(HydroServo *self);
HYDROSERVO_STATUS hydroservo_CheckAngleLimits(HydroServo *self);
HYDROSERVO_STATUS hydroservo_SearchOrigin(HydroServo *self, int16_t speed);

#endif /* INC_SERVO_H_ */
