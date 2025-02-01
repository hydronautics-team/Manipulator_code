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
	HYDROSERVO_ROTATE,
	HYDROSERVO_CALIBRATE,
	HYDROSERVO_ERROR_TIMEOUT,
	HYDROSERVO_ERROR_LIMITS,
	HYDROSERVO_ERROR_INCOR_DATA
}HYDROSERVO_STATUS;

typedef struct
{
	TIM_HandleTypeDef *tim_pwm;
	TIM_HandleTypeDef *tim_fb;
	uint16_t tim_channel_pwm;
	uint16_t tim_channel_fb;
	uint16_t tim_pwm_period;
	uint16_t tim_fb_period;
	uint32_t tim_fb_clock;

	GPIO_TypeDef *direction_port;
	uint16_t direction_pin;

	uint16_t fb_impulse_per_rotate;
}hydroservoConfig;
//может лучше hydroservo или Hydroservo
typedef struct
{
	int32_t target_angle;
	int16_t target_speed;
	int32_t current_angle;
	uint16_t current_speed;

	uint16_t fb_buffer;
	uint8_t fb_flag;

	int32_t max_angle;
	int32_t min_angle;

	HYDROSERVO_STATUS status;

	hydroservoConfig config;

}HydroServo;

void hydroservo_Init(HydroServo *self, hydroservoConfig config);

void hydroservo_CallbackPeriodElapsed(HydroServo *self);
void hydroservo_CallbackByFeedback(HydroServo *self);

int32_t hydroservo_GetSpeedRaw(HydroServo *self);
int32_t hydroservo_GetSpeedMilliRPM(HydroServo *self);
int32_t hydroservo_GetAngleRaw(HydroServo *self);
int32_t hydroservo_GetAngleDeciDegrees(HydroServo *self);
int32_t hydroservo_GetAngleMax(HydroServo *self);
int32_t hydroservo_GetAngleMin(HydroServo *self);
HYDROSERVO_STATUS hydroservo_GetStatus(HydroServo *self);

HYDROSERVO_STATUS hydroservo_SetSpeed(HydroServo *self, int16_t speed);
void hydroservo_SetOrigin(HydroServo *self, int32_t origin_angle);
void hydroservo_SetAngleMax(HydroServo *self, int32_t max_angle);
void hydroservo_SetAngleMin(HydroServo *self, int32_t min_angle);
void hydroservo_SetLimitsOffset(HydroServo *self, uint16_t offset);

HYDROSERVO_STATUS hydroservo_CheckAngleLimits(HydroServo *self);
//HYDROSERVO_STATUS hydroservo_SearchAngleLimit(HydroServo *self, int16_t speed, uint16_t min_speed_milli_rpm);
HYDROSERVO_STATUS hydroservo_SearchAngleLimit(HydroServo *self, int16_t speed, uint16_t delay);
HYDROSERVO_STATUS hydroservo_Calibrate(HydroServo *self, int16_t speed, uint16_t min_speed_milli_rpm);

#endif /* INC_SERVO_H_ */
