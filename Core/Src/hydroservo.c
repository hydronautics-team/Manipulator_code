//как лучше: хранить статус ошибки в структуре сервы или возвращать в функциях? возврат кода

#include "hydroservo.h"

#define SPEED_TO_PWM_(speed, pwm_period) (pwm_period - (speed >= 0 ? speed : -speed))
#define ANGLE_TO_DECIDEGREES_(angle, fb_period) (angle * 3600 / fb_period)

static void SetDirection_(HydroServo *servo_self);
static void SetPWM_(HydroServo *servo_self);

void hydroservo_Init(HydroServo *servo_self, TIM_HandleTypeDef *htim_pwm,
		TIM_HandleTypeDef *htim_fb, uint16_t channel_pwm, uint16_t channel_fb, uint16_t tim_pwm_period,
		GPIO_TypeDef *direction_port, uint16_t direction_pin)
{
	servo_self->tim_pwm = htim_pwm;
	servo_self->tim_fb = htim_fb;
	servo_self->tim_channel_pwm = channel_pwm;
	servo_self->tim_channel_fb = channel_fb;
	servo_self->tim_pwm_period = tim_pwm_period;

	servo_self->direction_port = direction_port;
	servo_self->direction_pin = direction_pin;

	servo_self->target_angle = 0;
	servo_self->target_speed = 0;
	servo_self->current_angle = 0;
	servo_self->current_speed = 0;

	hydroservo_SetSpeed(servo_self);
}

void hydroservo_SetSpeed(HydroServo *servo_self)
{
	SetDirection_(servo_self);
	SetPWM_(servo_self);
}

int32_t hydroservo_GetAngleRaw(HydroServo *servo_self)
{
	return servo_self->current_angle;
}

int32_t hydroservo_GetAngleDeciDegrees(HydroServo *servo_self)
{
	return ANGLE_TO_DECIDEGREES_(servo_self->current_angle, servo_self->fb_period);
}

void hydroservo_CallbackByFeedback(HydroServo *servo_self)
{
	if(servo_self->target_speed >= 0)
	{
		servo_self->current_angle++;
	}
	else
	{
		servo_self->current_angle--;
	}
}


 HYDROSERVO_STATUS hydroservo_CheckError(HydroServo *servo_self, int32_t previous_angle)
{
	if(servo_self->current_angle == previous_angle) return HYDROSERVO_ERROR;
	else return HYDROSERVO_OK;
}

static void SetDirection_(HydroServo *servo_self)
{
	if(servo_self->target_speed >= 0)
	{
		HAL_GPIO_WritePin(servo_self->direction_port, servo_self->direction_pin, SET);
	}
	else
	{
		HAL_GPIO_WritePin(servo_self->direction_port, servo_self->direction_pin, RESET);
	}
}
static void SetPWM_(HydroServo *servo_self)
{
	__HAL_TIM_SET_COMPARE(servo_self->tim_pwm, servo_self->tim_channel_pwm, SPEED_TO_PWM_(servo_self->target_speed, servo_self->tim_pwm_period));
}
