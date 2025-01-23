//в set speed принмать скорость, посмотреть что возвращают hal функции и ставить ошибку когда у них ошибка

#include "hydroservo.h"

#define SPEED_TO_PWM_(speed, pwm_period) (pwm_period - (speed >= 0 ? speed : -speed))
#define ANGLE_TO_DECIDEGREES_(angle, fb_period) ((angle * 3600) / fb_period)

static void SetDirection_(HydroServo *servo_self);
static void SetPWM_(HydroServo *servo_self);

void hydroservo_Init(HydroServo *servo_self, TIM_HandleTypeDef *htim_pwm,
		TIM_HandleTypeDef *htim_fb, uint16_t channel_pwm, uint16_t channel_fb, uint16_t tim_pwm_period, uint16_t fb_period,
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
	servo_self->current_angle = 0;
	servo_self->current_speed = 0;
	servo_self->max_angle = 0;
	servo_self->fb_period = fb_period;
	hydroservo_SetSpeed(servo_self, 0);
}

void hydroservo_SetSpeed(HydroServo *servo_self, int16_t speed)
{
	servo_self->target_speed = speed;
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

void hydroservo_SetOrigin(HydroServo *servo_self, int32_t origin_angle)
{
	servo_self->current_angle = origin_angle;
}

void hydroservo_SetAngleMax(HydroServo *servo_self, int32_t max_angle)
{
	servo_self->max_angle = max_angle;
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
	__HAL_TIM_SET_COMPARE(servo_self->tim_pwm, servo_self->tim_channel_pwm,
			SPEED_TO_PWM_(servo_self->target_speed, servo_self->tim_pwm_period));
}
