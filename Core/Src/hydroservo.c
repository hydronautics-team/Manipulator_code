#include "hydroservo.h"
#include "limits.h"

#define SPEED_TO_PWM_(speed, pwm_period) (pwm_period - (speed >= 0 ? speed : -speed))
#define ANGLE_TO_DECIDEGREES_(angle, fb_impulse_per_rotate) ((angle * 3600) / fb_impulse_per_rotate)
#define SPEED_TO_MILLI_RPM_(speed, fb_impulse_per_rotate, timer_clock) ((1000 * timer_clock) / (speed * fb_impulse_per_rotate))
#define ABS_(number) (number >= 0 ? number : -number)

static void SetDirection_(HydroServo *self);
static void SetPWM_(HydroServo *self);
static void ReadAngle_(HydroServo *self);
static void ReadSpeed_(HydroServo *self);
static HYDROSERVO_STATUS SearchAngleLimit_(HydroServo *self);


void hydroservo_Init(HydroServo *self, hydroservoConfig config)
{
	self->config = config;

	self->target_angle = 0;
	self->current_angle = 0;
	self->fb_impulse_period = 0;
	self->max_angle = HYDROSERVO_NO_MAX_ANGLE;
	self->min_angle = HYDROSERVO_NO_MIN_ANGLE;
	self->last_captured_value = 0;
	self->fb_flag = 0;
	hydroservo_SetSpeed(self, 0);

	HAL_TIM_PWM_Start(self->config.tim_pwm, self->config.tim_channel_pwm);
	HAL_TIM_IC_Start_IT(self->config.tim_fb, self->config.tim_channel_fb);
	__HAL_TIM_ENABLE_IT(self->config.tim_fb, self->config.tim_channel_fb);
	self->status = HYDROSERVO_OK;
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
	else
	{
		return HYDROSERVO_ERROR_LIMITS;
	}
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

void hydroservo_SetLimitsOffset(HydroServo *self, uint16_t offset)
{
	hydroservo_SetAngleMax(self, hydroservo_GetAngleMax(self) - offset);
	hydroservo_SetAngleMin(self, hydroservo_GetAngleMin(self) + offset);
}

int32_t hydroservo_GetAngleRaw(HydroServo *self)
{
	return self->current_angle;
}

int32_t hydroservo_GetAngleMax(HydroServo *self)
{
	return self->max_angle;
}

int32_t hydroservo_GetAngleMin(HydroServo *self)
{
	return self->min_angle;
}

int32_t hydroservo_GetPeriodFeedback(HydroServo *self)
{
	return self->fb_impulse_period;
}

int32_t hydroservo_GetSpeedMilliRPM(HydroServo *self)
{
	if(self->fb_impulse_period == self->config.tim_fb_period || self->target_speed == 0)
	{
		return 0;
	}
	else if(self->fb_impulse_period != 0)
	{
		return self->target_speed > 0 ?
			SPEED_TO_MILLI_RPM_(self->fb_impulse_period, self->config.fb_impulse_per_rotate, self->config.tim_fb_clock) :
			-SPEED_TO_MILLI_RPM_(self->fb_impulse_period, self->config.fb_impulse_per_rotate, self->config.tim_fb_clock);
	}
	else
	{
		return self->target_speed > 0 ? INT_MAX : INT_MIN;
	}
}

int32_t hydroservo_AngleToDeciDegrees(HydroServo *self, int32_t angle)
{
	return ANGLE_TO_DECIDEGREES_(angle, self->config.fb_impulse_per_rotate);
}

HYDROSERVO_STATUS hydroservo_GetStatus(HydroServo *self)
{
	return self->status;
}

void hydroservo_CallbackByFeedback(HydroServo *self)
{
	if(hydroservo_CheckAngleLimits(self))
	{
		hydroservo_SetSpeed(self, 0);
	}
	ReadSpeed_(self);
	ReadAngle_(self);
}

void hydroservo_CallbackPeriodElapsed(HydroServo *self)
{
	if(self->fb_flag)
	{
		self->fb_impulse_period = self->config.tim_fb_period;
	}
	else
	{
		self->fb_flag = 1;
	}
}

HYDROSERVO_STATUS hydroservo_CheckAngleLimits(HydroServo *self)
{
	if((self->target_speed == 0) ||
			(self->target_speed > 0 && self->current_angle < self->max_angle) ||
			(self->target_speed < 0 && self->current_angle > self->min_angle))
	{
		return HYDROSERVO_OK;
	}
	else
	{
		return HYDROSERVO_ERROR_LIMITS;
	}
}

HYDROSERVO_STATUS hydroservo_Calibrate(HydroServo *self)
{
	self->status = HYDROSERVO_CALIBRATE;
	HYDROSERVO_STATUS status = SearchAngleLimit_(self);
	if(!status)
	{
		status = SearchAngleLimit_(self);
		if(!status)
		{
			int32_t angle = (hydroservo_GetAngleMax(self) - hydroservo_GetAngleMin(self)) / 2;
			hydroservo_SetSpeed(self, self->config.calibrating_speed);
			if(self->config.calibrating_speed > 0)
			{
				while(hydroservo_GetAngleRaw(self) < angle);
			}
			else
			{
				while(hydroservo_GetAngleRaw(self) > angle);
			}
			self->status = HYDROSERVO_OK;
			return HYDROSERVO_OK;
		}
		else
		{
			self->status = HYDROSERVO_OK;
			return status;
		}
	}
	else
	{
		self->status = HYDROSERVO_OK;
		return status;
	}
}

static void SetDirection_(HydroServo *self)
{
	if(self->target_speed >= 0) HAL_GPIO_WritePin(self->config.direction_port, self->config.direction_pin, SET);
	else HAL_GPIO_WritePin(self->config.direction_port, self->config.direction_pin, RESET);
}
static void SetPWM_(HydroServo *self)
{
	__HAL_TIM_SET_COMPARE(self->config.tim_pwm, self->config.tim_channel_pwm,
			SPEED_TO_PWM_(self->target_speed, self->config.tim_pwm_period));
}

static void ReadAngle_(HydroServo *self)
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

static void ReadSpeed_(HydroServo *self)
{
	uint16_t captured_value = HAL_TIM_ReadCapturedValue(self->config.tim_fb, self->config.tim_channel_fb);

	if(!self->fb_flag)
	{
		self->fb_impulse_period = captured_value - self->last_captured_value;
	}
	else if (captured_value <= self->last_captured_value)
	{
		self->fb_impulse_period = captured_value + self->config.tim_fb_period - self->last_captured_value;
	}
	else
	{
		self->fb_impulse_period = self->config.tim_fb_period;
	}

	self->last_captured_value = captured_value;
	self->fb_flag = 0;
}

static HYDROSERVO_STATUS SearchAngleLimit_(HydroServo *self)
{
	int32_t angle_previous;
	hydroservo_SetSpeed(self, self->config.calibrating_speed);

	for(int32_t i = 0; i < INT_MAX; i++)
	{
		angle_previous = hydroservo_GetAngleRaw(self);
		HAL_Delay(self->config.calibrating_delay);
		if(hydroservo_GetAngleRaw(self) == angle_previous)
		{
			hydroservo_SetSpeed(self, 0);
			if(self->config.calibrating_speed)
			{
				hydroservo_SetAngleMax(self, hydroservo_GetAngleRaw(self));
			}
			else
			{
				hydroservo_SetAngleMin(self, hydroservo_GetAngleRaw(self));
			}
			return HYDROSERVO_OK;
		}
	}
	return HYDROSERVO_ERROR_TIMEOUT;
}

/*HYDROSERVO_STATUS SearchAngleLimit_(HydroServo *self, int16_t speed, uint16_t min_speed_milli_rpm)
{
	if(speed)
	{
		hydroservo_SetSpeed(self, speed);
		HAL_Delay(SEARCH_ORIGIN_DELAY_MILLISECONDS_);
		for(int32_t i = 0; i < INT_MAX; i++)
		{
			if(ABS_(hydroservo_GetSpeedMilliRPM(self)) <= min_speed_milli_rpm)
			{
				hydroservo_SetSpeed(self, 0);
				if(speed > 0) hydroservo_SetAngleMax(self, hydroservo_GetAngleRaw(self));
				else hydroservo_SetAngleMin(self, hydroservo_GetAngleRaw(self));
				return HYDROSERVO_OK;
			}
		}
		return HYDROSERVO_ERROR_TIMEOUT;
	}else return HYDROSERVO_ERROR_INCOR_DATA;
}*/
