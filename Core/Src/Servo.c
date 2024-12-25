/*
 * Servo.c
 *
 *  Created on: Dec 18, 2024
 *      Author: 1
 */


#include "Servo.h"

//ХУЙНЯ, ПЕРЕДЕЛАТЬ!!!
void InitServos(Servo *servo1, Servo *servo2, TIM_HandleTypeDef *htim_pwm, TIM_HandleTypeDef *htim_fb)
{
	servo1->tim_pwm = htim_pwm;
	servo2->tim_pwm = htim_pwm;
	servo1->tim_fb = htim_fb;
	servo2->tim_fb = htim_fb;

	servo1->tim_channel_pwm = SERVO1_PWM_TIM_CHANNEL;
	servo2->tim_channel_pwm = SERVO2_PWM_TIM_CHANNEL;

	servo1->tim_channel_fb = SERVO1_FB_TIM_CHANNEL;
	servo2->tim_channel_fb = SERVO2_FB_TIM_CHANNEL;
}

void SetSpeed(Servo *srv)
{
	__HAL_TIM_SET_COMPARE(srv->tim_pwm, srv->tim_channel_pwm, srv->speed);
}

/*void SetPwm(TIM_HandleTypeDef *htim, uint16_t tim_channel, uint16_t speed)
{
	/*TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = speed;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, tim_channel);
	HAL_TIM_PWM_Start(htim, tim_channel);

	__HAL_TIM_SET_COMPARE(htim, tim_channel, speed);
}
*/
