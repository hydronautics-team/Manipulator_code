/*
 * hydroled.h
 *
 *  Created on: Jan 6, 2025
 *      Author: 1
 */

#ifndef INC_HYDROLED_H_
#define INC_HYDROLED_H_

#include "stm32f1xx_hal.h"
#include "main.h"
#include <stdint.h>

typedef struct
{
	GPIO_TypeDef *led_port;
	uint16_t led_pin;

	uint8_t blink_count;
	uint16_t blink_period_on;
	uint16_t blink_period_off;
}hydroLED;

void hydroled_Init(hydroLED *led_self, GPIO_TypeDef *led_port, uint16_t led_pin);
void hydroled_SetBlinkCount(hydroLED *led_self, uint8_t count);
void hydroled_SetBlinkPeriodON(hydroLED *led_self, uint16_t period);
void hydroled_SetBlinkPeriodOFF(hydroLED *led_self, uint16_t period);
uint16_t hydroled_GetPeriodOFF(hydroLED *led_self);
void hydroled_ON(hydroLED *led_self);
void hydroled_OFF(hydroLED *led_self);
void hydroled_Blink(hydroLED *led_self);

#endif /* INC_HYDROLED_H_ */
