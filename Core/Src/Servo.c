/*
 * Servo.c
 *
 *  Created on: Dec 18, 2024
 *      Author: 1
 */


#include "Servo.h"

//подумать как сделать лучше!!
void InitServos(Servo *servo1, Servo *servo2)
{
	servo1->tim_channel_pwm = SERVO1_PWM_TIM_CHANNEL;
	servo2->tim_channel_pwm = SERVO2_PWM_TIM_CHANNEL;

	servo1->tim_channel_fb = SERVO1_FB_TIM_CHANNEL;
	servo2->tim_channel_fb = SERVO2_FB_TIM_CHANNEL;
}

void SetSpeed(Servo *srv)
{

	/*TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = speed;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, tim_channel);
	HAL_TIM_PWM_Start(htim, tim_channel);
	*/
	__HAL_TIM_SET_COMPARE(srv->htim_pwm, srv->tim_channel_pwm, srv->speed);
}

void ReadFb(Servo *srv)
{

}
