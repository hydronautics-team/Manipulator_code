//в set speed принмать скорость, посмотреть что возвращают hal функции и ставить ошибку когда у них ошибка

#include "hydroservo.h"
#include "limits.h"

#define SPEED_TO_PWM_(speed, pwm_period) (pwm_period - (speed >= 0 ? speed : -speed))
#define ANGLE_TO_DECIDEGREES_(angle, fb_impulse_per_rotate) ((angle * 3600) / fb_impulse_per_rotate)
#define SPEED_TO_MILLI_RPM_(speed, fb_impulse_per_rotate, timer_clock) ((1000 * timer_clock) / (speed * fb_impulse_per_rotate))
#define ABS_(number) (number >= 0 ? number : -number)

#define SEARCH_ORIGIN_DELAY_MILLISECONDS_ 7
#define SEARCH_ORIGIN_WAITING_COUNT_ 1000

static void SetDirection_(HydroServo *self);
static void SetPWM_(HydroServo *self);

void hydroservo_Init(HydroServo *self, TIM_HandleTypeDef *htim_pwm, TIM_HandleTypeDef *htim_fb,
		uint16_t channel_pwm, uint16_t channel_fb, uint16_t tim_pwm_period, uint16_t tim_fb_period, uint16_t fb_impulse_per_rotate,
		uint32_t fb_timer_clock, GPIO_TypeDef *direction_port, uint16_t direction_pin)
{
	self->tim_pwm = htim_pwm;
	self->tim_fb = htim_fb;
	self->tim_channel_pwm = channel_pwm;
	self->tim_channel_fb = channel_fb;
	self->tim_pwm_period = tim_pwm_period;
	self->tim_fb_period = tim_fb_period;
	self->fb_timer_clock = fb_timer_clock;

	self->direction_port = direction_port;
	self->direction_pin = direction_pin;

	self->target_angle = 0;
	self->current_angle = 0;
	self->current_speed = 0;
	self->max_angle = HYDROSERVO_NO_MAX_ANGLE;
	self->min_angle = HYDROSERVO_NO_MIN_ANGLE;
	self->fb_impulse_per_rotate = fb_impulse_per_rotate;
	hydroservo_SetSpeed(self, 0);
}

HYDROSERVO_STATUS hydroservo_SetSpeed(HydroServo *self, int16_t speed)
{
	if((speed == 0) ||
			(speed > 0 && self->current_angle < self->max_angle) ||
			(speed < 0 && self->current_angle > self->min_angle))
	{
		self->target_speed = speed;
		SetDirection_(self);
		SetPWM_(self);
		return HYDROSERVO_OK;
	}
	else return HYDROSERVO_ERROR_LIMITS;
}

int32_t hydroservo_GetSpeedRaw(HydroServo *self)
{
	return self->current_speed;
}

int32_t hydroservo_GetSpeedMilliRPM(HydroServo *self)
{
	if(self->current_speed == self->tim_fb_period || self->target_speed == 0) return 0;
	else if(self->current_speed != 0) return self->target_speed > 0 ?
			SPEED_TO_MILLI_RPM_(self->current_speed, self->fb_impulse_per_rotate, self->fb_timer_clock) :
			-SPEED_TO_MILLI_RPM_(self->current_speed, self->fb_impulse_per_rotate, self->fb_timer_clock);
	else return self->target_speed > 0 ? INT_MAX : INT_MIN;
}

int32_t hydroservo_GetAngleRaw(HydroServo *self)
{
	return self->current_angle;
}

int32_t hydroservo_GetAngleDeciDegrees(HydroServo *self)
{
	return ANGLE_TO_DECIDEGREES_(self->current_angle, self->fb_impulse_per_rotate);
}

void hydroservo_CallbackByFeedback(HydroServo *self)
{
	hydroservo_CheckAngleLimits(self);

	uint16_t captured_value = HAL_TIM_ReadCapturedValue(self->tim_fb, self->tim_channel_fb);

	if(!self->fb_flag) self->current_speed = captured_value - self->fb_buffer;
	else if (captured_value <= self->fb_buffer)
		self->current_speed = captured_value + self->tim_fb_period - self->fb_buffer;
	else self->current_speed = self->tim_fb_period;

	self->fb_buffer = captured_value;
	self->fb_flag = 0;

	if(self->target_speed >= 0) self->current_angle++;
	else self->current_angle--;
}

void hydroservo_CallbackPeriodElapsed(HydroServo *self)
{
	if(self->fb_flag) self->current_speed = self->tim_fb_period;
	else self->fb_flag = 1;
}

void hydroservo_SetOrigin(HydroServo *self, int32_t origin_angle)
{
	self->current_angle -= origin_angle;
	self->max_angle -= origin_angle;
	self->min_angle -= origin_angle;
}

void hydroservo_SetAngleMax(HydroServo *self, int32_t max_angle)
{
	self->max_angle = max_angle;
}

void hydroservo_SetAngleMin(HydroServo *self, int32_t min_angle)
{
	self->min_angle = min_angle;
}

int32_t hydroservo_GetAngleMax(HydroServo *self)
{
	return self->max_angle;
}

int32_t hydroservo_GetAngleMin(HydroServo *self)
{
	return self->min_angle;
}

HYDROSERVO_STATUS hydroservo_CheckAngleLimits(HydroServo *self)
{
	if((self->current_angle >= self->max_angle && self->target_speed > 0) ||
			(self->current_angle <= self->min_angle && self->target_speed < 0))
	{
		hydroservo_SetSpeed(self, 0);
		return HYDROSERVO_ERROR_LIMITS;
	}
	return HYDROSERVO_OK;
}

//сделать конечный автомат пометка в каком состоянии серва
//сделать неблокирующей

HYDROSERVO_STATUS hydroservo_SearchAngleLimit(HydroServo *self, int16_t speed, uint16_t min_speed_milli_rpm)
{
	hydroservo_SetSpeed(self, speed);

	for(int16_t i = 0; i < SEARCH_ORIGIN_WAITING_COUNT_; i++)
	{
		HAL_Delay(SEARCH_ORIGIN_DELAY_MILLISECONDS_);
		if(ABS_(hydroservo_GetSpeedMilliRPM(self)) <= min_speed_milli_rpm)
		{
			hydroservo_SetSpeed(self, 0);
			return HYDROSERVO_OK;
		}
	}
	return HYDROSERVO_ERROR_TIMEOUT;
}

static void SetDirection_(HydroServo *self)
{
	if(self->target_speed >= 0) HAL_GPIO_WritePin(self->direction_port, self->direction_pin, SET);
	else HAL_GPIO_WritePin(self->direction_port, self->direction_pin, RESET);
}
static void SetPWM_(HydroServo *self)
{
	__HAL_TIM_SET_COMPARE(self->tim_pwm, self->tim_channel_pwm,
			SPEED_TO_PWM_(self->target_speed, self->tim_pwm_period));
}
