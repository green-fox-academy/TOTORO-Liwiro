/* Includes ------------------------------------------------------------------*/
#include "motor_control.h"
#include "stm32l4xx_hal.h"
#include "init.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define COMMAND_SIZE	24
/* Private macro -------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Command arrays */
const uint8_t ctrl_up_arr[COMMAND_SIZE] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0,
									 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0};

const uint8_t ctrl_down_arr[COMMAND_SIZE]= {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0,
									  1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1};

const uint8_t ctrl_stop_arr[COMMAND_SIZE]= {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0,
		   	   	   	   	   	   	   	  1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1};

const uint8_t ctrl_verification_arr[COMMAND_SIZE]= {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0,
		   	   	   	   	   	   	   	  	  	  1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1};
/* PWM variables */
extern TIM_HandleTypeDef tim_pwm_handle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void end_bit()
{
	HAL_TIM_PWM_Start(&tim_pwm_handle, TIM_CHANNEL_1);
	delay(1);
	HAL_TIM_PWM_Stop(&tim_pwm_handle, TIM_CHANNEL_1);
}

void bit_one()
{
	HAL_TIM_PWM_Start(&tim_pwm_handle, TIM_CHANNEL_1);
	delay(1);
	HAL_TIM_PWM_Stop(&tim_pwm_handle, TIM_CHANNEL_1);
	delay(3);
}

void bit_zero()
{
	HAL_TIM_PWM_Start(&tim_pwm_handle, TIM_CHANNEL_1);
	delay(3);
	HAL_TIM_PWM_Stop(&tim_pwm_handle, TIM_CHANNEL_1);
	delay(1);
}

void go_forward()
{
	HAL_TIM_PWM_Start(&tim2_pwm_handle, TIM_CHANNEL_1);
	printf("PWM started\r\n");
	while(1){
		HAL_Delay(500);
		tim2_pwm_handle.Instance->CCR1 = 10;
		HAL_Delay(500);
		tim2_pwm_handle.Instance->CCR1 = 80;
	}
	HAL_TIM_PWM_Stop(&tim2_pwm_handle, TIM_CHANNEL_1);
}

void go_backward()
{
	HAL_TIM_PWM_Start(&tim3_pwm_handle, TIM_CHANNEL_1);
	while(1){

	}
	HAL_TIM_PWM_Stop(&tim3_pwm_handle, TIM_CHANNEL_1);
}

void go_left()
{
	HAL_TIM_PWM_Start(&tim3_pwm_handle, TIM_CHANNEL_1);
	while(1){

	}
	HAL_TIM_PWM_Stop(&tim3_pwm_handle, TIM_CHANNEL_1);
}

void go_right()
{
	HAL_TIM_PWM_Start(&tim3_pwm_handle, TIM_CHANNEL_1);
	while(1){

	}
	HAL_TIM_PWM_Stop(&tim3_pwm_handle, TIM_CHANNEL_1);
}

void ctrl_stop()
{

}
