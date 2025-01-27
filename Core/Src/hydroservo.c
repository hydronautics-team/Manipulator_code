//в set speed принмать скорость, посмотреть что возвращают hal функции и ставить ошибку когда у них ошибка

#include "hydroservo.h"

#define SPEED_TO_PWM_(speed, pwm_period) (pwm_period - (speed >= 0 ? speed : -speed))
#define ANGLE_TO_DECIDEGREES_(angle, fb_period) ((angle * 3600) / fb_period)

#define SEARCH_ORIGIN_DELAY_MILLISECONDS_ 7
#define SEARCH_ORIGIN_WAITING_COUNT_ 1000

static void SetDirection_(HydroServo *self);
static void SetPWM_(HydroServo *self);

void hydroservo_Init(HydroServo *self, TIM_HandleTypeDef *htim_pwm,
		TIM_HandleTypeDef *htim_fb, uint16_t channel_pwm, uint16_t channel_fb, uint16_t tim_pwm_period, uint16_t fb_period,
		GPIO_TypeDef *direction_port, uint16_t direction_pin)
{
	self->tim_pwm = htim_pwm;
	self->tim_fb = htim_fb;
	self->tim_channel_pwm = channel_pwm;
	self->tim_channel_fb = channel_fb;
	self->tim_pwm_period = tim_pwm_period;

	self->direction_port = direction_port;
	self->direction_pin = direction_pin;

	self->target_angle = 0;
	self->current_angle = 0;
	self->current_speed = 0;
	self->max_angle = 0;
	self->fb_period = fb_period;
	hydroservo_SetSpeed(self, 0);
}

//нормально ли писать такие длинные условия?

HYDROSERVO_STATUS hydroservo_SetSpeed(HydroServo *self, int16_t speed)
{
	if((speed == 0 || self->max_angle == 0) ||
			(speed > 0 && self->current_angle < self->max_angle) ||
			(speed < 0 && self->current_angle > 0))
	{
		self->target_speed = speed;
		SetDirection_(self);
		SetPWM_(self);
		return HYDROSERVO_OK;
	}
	else return HYDROSERVO_ERROR_LIMITS;
}

int32_t hydroservo_GetAngleRaw(HydroServo *self)
{
	return self->current_angle;
}

int32_t hydroservo_GetAngleDeciDegrees(HydroServo *self)
{
	return ANGLE_TO_DECIDEGREES_(self->current_angle, self->fb_period);
}

void hydroservo_CallbackByFeedback(HydroServo *self)
{
	if(self->target_speed >= 0)
	{
		self->current_angle++;
	}
	else
	{
		self->current_angle--;
	}
}
//смещать мин макс при установке нуля
//макрос no min no max angle в котором лежит максимальное значение переменной
//hal max delay
//макрос с макс значением типа в станд либе
void hydroservo_SetOrigin(HydroServo *self, int32_t origin_angle)
{
	self->current_angle -= origin_angle;
}

void hydroservo_SetAngleMax(HydroServo *self, int32_t max_angle)
{
	self->max_angle = max_angle;
}

//limits
//проверку ограничений в callback
//сделать скорость как скорость пересчитывать когда надо считать
//сделать флаг для определения нулевой скорости
HYDROSERVO_STATUS hydroservo_CheckAngleRestrictions(HydroServo *self)
{
	if(((self->current_angle >= self->max_angle && self->target_speed > 0) || (self->current_angle <= 0 && self->target_speed < 0))
			&& self->max_angle != 0)
	{
		hydroservo_SetSpeed(self, 0);
		return HYDROSERVO_ERROR_LIMITS;
	}
	return HYDROSERVO_OK;
}

//почему переменные нельзя объявлять как static?

//сделать конечный автомат пометка в каком состоянии серва
//сделать неблокирующей
//отмечать крайние положения автоматически
//сбрасывать
//ноль мин макс и возможность получить эти значения

HYDROSERVO_STATUS hydroservo_SearchOrigin(HydroServo *self, int16_t speed)
{
	int32_t angle_previous_ = hydroservo_GetAngleRaw(self);
	hydroservo_SetSpeed(self, speed);

	//нормально ли объявлять счетчик внутри объявления цикла for??
	for(int16_t i = 0; i < SEARCH_ORIGIN_WAITING_COUNT_; i++)
	{
		angle_previous_ = hydroservo_GetAngleRaw(self);
		HAL_Delay(SEARCH_ORIGIN_DELAY_MILLISECONDS_);
		if(hydroservo_GetAngleRaw(self) == angle_previous_)
		{
			hydroservo_SetSpeed(self, 0);
			return HYDROSERVO_OK;
		}
	}
	return HYDROSERVO_ERROR_TIMEOUT;
}


static void SetDirection_(HydroServo *self)
{
	if(self->target_speed >= 0)
	{
		HAL_GPIO_WritePin(self->direction_port, self->direction_pin, SET);
	}
	else
	{
		HAL_GPIO_WritePin(self->direction_port, self->direction_pin, RESET);
	}
}
static void SetPWM_(HydroServo *self)
{
	__HAL_TIM_SET_COMPARE(self->tim_pwm, self->tim_channel_pwm,
			SPEED_TO_PWM_(self->target_speed, self->tim_pwm_period));
}
