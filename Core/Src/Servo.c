/*
 * Servo.c
 *
 *  Created on: Dec 18, 2024
 *      Author: 1
 */


#include "Servo.h"

TIM_HandleTypeDef htim3;

typedef struct
{
	uint8_t DIR;
	uint8_t SPD;
	uint16_t ANG;
	uint16_t FB;

}Servo;

void set_speed(uint16_t speed, uint8_t servo_num)
{

		TIM_OC_InitTypeDef sConfigOC;

		sConfigOC.OCMode = TIM_OCMODE_PWM1;
		sConfigOC.Pulse = speed;
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

		if(servo_num == 1)
		{
			HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
		}

		if(servo_num == 2)
		{
			HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);
			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
		}

}
