/*
 * Servo.c
 *
 *  Created on: Dec 18, 2024
 *      Author: 1
 */


#include "Servo.h"
//read about changing pulse
//__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, i)
void set_speed(uint16_t speed, TIM_HandleTypeDef *htim, uint16_t tim_channel)
{

	TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = speed;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, tim_channel);
	HAL_TIM_PWM_Start(htim, tim_channel);
}

void read_fb(uint16_t period, Servo *servo)
{
	servo->fb = period * 8;
}
