/*
 * hydroled.c
 *
 *  Created on: Jan 6, 2025
 *      Author: 1
 */

#include "hydroled.h"

static void Blink_(hydroLED *led_self)
{
	HAL_GPIO_WritePin(led_self->led_port, led_self->led_pin, SET);
	HAL_Delay(led_self->blink_period_on);
	HAL_GPIO_WritePin(led_self->led_port, led_self->led_pin, RESET);
}

static void Delay_(hydroLED *led_self)
{
	HAL_Delay(led_self->blink_period_off);
}

void hydroled_Init(hydroLED *led_self, GPIO_TypeDef *led_port, uint16_t led_pin)
{
	led_self->led_port = led_port;
	led_self->led_pin = led_pin;

	led_self->blink_count = 0;
	led_self->blink_period_on = 0;
	led_self->blink_period_off = 0;
}

void hydroled_SetBlinkCount(hydroLED *led_self, uint8_t count)
{
	led_self->blink_count = count;
}

void hydroled_SetBlinkPeriodON(hydroLED *led_self, uint16_t period)
{
	led_self->blink_period_on = period;
}

void hydroled_SetBlinkPeriodOFF(hydroLED *led_self, uint16_t period)
{
	led_self->blink_period_off = period;
}

uint16_t hydroled_GetPeriodOFF(hydroLED *led_self)
{
	return led_self->blink_period_off;
}

void hydroled_ON(hydroLED *led_self)
{
	HAL_GPIO_WritePin(led_self->led_port, led_self->led_pin, SET);
}

void hydroled_OFF(hydroLED *led_self)
{
	HAL_GPIO_WritePin(led_self->led_port, led_self->led_pin, RESET);
}

void hydroled_Blink(hydroLED *led_self)
{
	uint16_t period = hydroled_GetPeriodOFF(led_self);

	for(uint8_t i = 0; i < period; i++)
	{
		Blink_(led_self);
		if(i >= period) break;
		Delay_(led_self);
	}
}
