//скорость знаковой переменной устанавливать и потом думать про направление
//set_speed, get_angle, init, прерывания от таймера(считываем только угол) - внешняя, определие ошибок (если тик не пришел, циклически вызываемая), пока скорость не считать
//как лучше: хранить статус ошибки в структуре сервы или возвращать в функциях?




#include "Servo.h"

static void SetError_(HydroServo *hydroservo_self)
{
	hydroservo_self->status = HYDROSERVO_STATUS_ERROR;
}

static void SetDirection_(HydroServo *hydroservo_self)
{
	if(hydroservo_self->direction == 1)
	{
		HAL_GPIO_WritePin(hydroservo_self->GPIOx, hydroservo_self->GPIO_Pin, SET);
	}
	else if(hydroservo_self->direction == -1)
	{
		HAL_GPIO_WritePin(hydroservo_self->GPIOx, hydroservo_self->GPIO_Pin, RESET);
	}
	else
	{
		SetError_(hydroservo_self);
	}
}
static void SetPWM_(HydroServo *hydroservo_self)
{
	__HAL_TIM_SET_COMPARE(hydroservo_self->tim_pwm, hydroservo_self->tim_channel_pwm, hydroservo_self->target_speed);
}



void hydroservo_SetSpeed(HydroServo *hydroservo_self)
{
	SetDirection_(hydroservo_self);
	SetPWM_(hydroservo_self);
}

void hydroservo_Init(HydroServo *hydroservo_self, TIM_HandleTypeDef *htim_pwm,
		TIM_HandleTypeDef *htim_fb, uint16_t channel_pwm, uint16_t channel_fb, GPIO_TypeDef *GPIOx, uint16_t *GPIO_Pin)
{
	hydroservo_self->tim_pwm = htim_pwm;
	hydroservo_self->tim_fb = htim_fb;

	hydroservo_self->tim_channel_pwm = channel_pwm;
	hydroservo_self->tim_channel_fb = channel_fb;

	hydroservo_self->GPIOx = GPIOx;
	hydroservo_self->GPIO_Pin = GPIO_Pin;

	hydroservo_self->target_angle = 0;
	hydroservo_self->target_speed = TIM_PWM_COUNTER;
	hydroservo_self->direction = 1;
	hydroservo_self->current_angle = 0;
	hydroservo_self->current_speed = 0;
	hydroservo_self->status = HYDROSERVO_STATUS_OK;

	hydroservo_SetSpeed(hydroservo_self);
}

int32_t hydroservo_GetAngle(HydroServo *hydroservo_self)
{
	return hydroservo_self->current_angle;
}

void hydroservo_Callback(HydroServo *hydroservo_self)
{
	if(hydroservo_self->direction == 1)
	{
		hydroservo_self->current_angle++;
	}
	else if(hydroservo_self->direction == -1)
	{
		hydroservo_self->current_angle--;
	}
	else
	{
		SetError_(hydroservo_self);
	}
}

void hydroservo_CheckError(HydroServo *hydroservo_self, int32_t previous_angle)
{
	if(hydroservo_self->current_angle == previous_angle)
	{
		hydroservo_self->status = HYDROSERVO_STATUS_ERROR;
	}
}
