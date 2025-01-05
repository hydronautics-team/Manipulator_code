//скорость знаковой переменной устанавливать и потом думать про направление
//как лучше: хранить статус ошибки в структуре сервы или возвращать в функциях?

#include "hydroservo.h"

static void SetError_(HydroServo *servo_self)
{
	servo_self->status = HYDROSERVO_STATUS_ERROR;
}

static void SetDirection_(HydroServo *servo_self)
{
	if(servo_self->direction == 1)
	{
		HAL_GPIO_WritePin(servo_self->direction_port, servo_self->direction_pin, SET);
	}
	else if(servo_self->direction == -1)
	{
		HAL_GPIO_WritePin(servo_self->direction_port, servo_self->direction_pin, RESET);
	}
	else
	{
		SetError_(servo_self);
	}
}
static void SetPWM_(HydroServo *servo_self)
{
	__HAL_TIM_SET_COMPARE(servo_self->tim_pwm, servo_self->tim_channel_pwm, servo_self->target_speed);
}



void hydroservo_SetSpeed(HydroServo *servo_self)
{
	SetDirection_(servo_self);
	SetPWM_(servo_self);
}

void hydroservo_Init(HydroServo *servo_self, TIM_HandleTypeDef *htim_pwm,
		TIM_HandleTypeDef *htim_fb, uint16_t channel_pwm, uint16_t channel_fb, GPIO_TypeDef *direction_port, uint16_t *direction_pin)
{
	servo_self->tim_pwm = htim_pwm;
	servo_self->tim_fb = htim_fb;

	servo_self->tim_channel_pwm = channel_pwm;
	servo_self->tim_channel_fb = channel_fb;

	servo_self->direction_port = direction_port;
	servo_self->direction_pin = direction_pin;

	servo_self->target_angle = 0;
	servo_self->target_speed = HYDROSERVO_TIM_PWM_COUNTER;
	servo_self->direction = 1;
	servo_self->current_angle = 0;
	servo_self->current_speed = 0;
	servo_self->status = HYDROSERVO_STATUS_OK;

	hydroservo_SetSpeed(servo_self);
}

int32_t hydroservo_GetAngle(HydroServo *servo_self)
{
	return servo_self->current_angle;
}

void hydroservo_CallbackByFeedback(HydroServo *servo_self)
{
	if(servo_self->direction == 1)
	{
		servo_self->current_angle++;
	}
	else if(servo_self->direction == -1)
	{
		servo_self->current_angle--;
	}
	else
	{
		SetError_(servo_self);
	}
}

void hydroservo_CheckError(HydroServo *servo_self, int32_t previous_angle)
{
	if(servo_self->current_angle == previous_angle)
	{
		servo_self->status = HYDROSERVO_STATUS_ERROR;
	}
}
