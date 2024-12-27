/*
 * Servo.c
 *
 *  Created on: Dec 18, 2024
 *      Author: 1
 */


#include "Servo.h"

void InitServo(Servo *srv, TIM_HandleTypeDef *htim_pwm, TIM_HandleTypeDef *htim_fb, uint16_t channel_pwm, uint16_t channel_fb, GPIO_TypeDef *GPIOx, uint16_t *GPIO_Pin)
{
	srv->tim_pwm = htim_pwm;
	srv->tim_fb = htim_fb;

	srv->tim_channel_pwm = channel_pwm;
	srv->tim_channel_fb = channel_fb;

	srv->GPIOx = GPIOx;
	srv->GPIO_Pin = GPIO_Pin;

	srv->direction = 1;
	srv->speed = 399;
	srv->angle = 0;
	ResetSpeed(srv);


	/*
	servo1->tim_pwm = htim_pwm;
	servo2->tim_pwm = htim_pwm;
	servo1->tim_fb = htim_fb;
	servo2->tim_fb = htim_fb;

	servo1->tim_channel_pwm = SERVO1_PWM_TIM_CHANNEL;
	servo2->tim_channel_pwm = SERVO2_PWM_TIM_CHANNEL;

	servo1->tim_channel_fb = SERVO1_FB_TIM_CHANNEL;
	servo2->tim_channel_fb = SERVO2_FB_TIM_CHANNEL;

	servo1->GPIOx = SRV1_FB_GPIO_Port;
	servo2->GPIOx = SRV2_FB_GPIO_Port;
	servo1->GPIO_Pin = SRV1_DIR_Pin;
	servo2->GPIO_Pin = SRV2_DIR_Pin;
	*/
}

void SetDirection(Servo *srv)
{
	if(srv->direction == 1)
	{
		HAL_GPIO_WritePin(srv->GPIOx, srv->GPIO_Pin, SET);
	}
	if(srv->direction == -1)
	{
		HAL_GPIO_WritePin(srv->GPIOx, srv->GPIO_Pin, RESET);
	}
}
void SetSpeed(Servo *srv)
{
	__HAL_TIM_SET_COMPARE(srv->tim_pwm, srv->tim_channel_pwm, srv->speed);
}
void ResetSpeed(Servo *srv)
{
	__HAL_TIM_SET_COMPARE(srv->tim_pwm, srv->tim_channel_pwm, TIM_PWM_COUNTER);
}

void Rotate(Servo *srv)
{
	SetDirection(srv);
	SetSpeed(srv);
}

/*void RotateByAngle(Servo *srv)
{
	SetDirection(srv);
	SetSpeed(srv);
	while(srv->fb_angle < srv->angle) HAL_Delay(10);
	ResetSpeed(srv);
}*/

void RotateByAngle(Servo *srv)
{
	if(srv->angle - srv->fb_angle > ServoGapK)
	{
		srv->direction = 1;//Может сделать отдельную функцию для установки направления??
		Rotate(srv);
	}
	if(srv->angle - srv->fb_angle < -ServoGapK)
	{
		srv->direction = -1;
		Rotate(srv);
	}
	if(srv->angle - srv->fb_angle < ServoGapK && srv->angle - srv->fb_angle > -ServoGapK)
	{
		ResetSpeed(srv);
	}
}
//дописать!!!
/*void CalibrateServo(Servo *srv)
{
	srv->direction = 1;
	srv->speed = 250; //Может тоже отдельную функцию сделать чтобы в структуру не лезть??
	Rotate(srv);
}*/

/*void ErrorHandlerServo(Servo *srv)
{

}*/

/*void SetPwm(TIM_HandleTypeDef *htim, uint16_t tim_channel, uint16_t speed)
{
	TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = speed;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, tim_channel);
	HAL_TIM_PWM_Start(htim, tim_channel);

	__HAL_TIM_SET_COMPARE(htim, tim_channel, speed);
}
*/
