/* Includes ------------------------------------------------------------------*/
#include "motor_control.h"
#include "stm32l4xx_hal.h"
#include "init.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/

/* PWM variables */
extern TIM_HandleTypeDef tim2_pwm_handle;
extern TIM_HandleTypeDef tim3_pwm_handle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void go_forward()
{
	gpio_m1_p1_on();
	gpio_m1_p2_off();

	gpio_m2_p1_on();
	gpio_m2_p2_off();

	tim2_pwm_handle.Instance->CCR1 = 100;
	tim3_pwm_handle.Instance->CCR1 = 100;
	HAL_Delay(5000);
}

void go_backward()
{
	gpio_m1_p1_off();
	gpio_m1_p2_on();

	gpio_m2_p1_off();
	gpio_m2_p2_on();

	tim2_pwm_handle.Instance->CCR1 = 100;
	tim3_pwm_handle.Instance->CCR1 = 100;
	HAL_Delay(5000);
}

void go_left()
{
	gpio_m1_p1_on();
	gpio_m1_p2_off();

	gpio_m2_p1_on();
	gpio_m2_p2_off();

	tim2_pwm_handle.Instance->CCR1 = 100;
	tim3_pwm_handle.Instance->CCR1 = 80;
	HAL_Delay(5000);
}

void go_right()
{
	gpio_m1_p1_on();
	gpio_m1_p2_off();

	gpio_m2_p1_on();
	gpio_m2_p2_off();

	tim2_pwm_handle.Instance->CCR1 = 80;
	tim3_pwm_handle.Instance->CCR1 = 100;
	HAL_Delay(5000);
}

void ctrl_stop()
{
	gpio_m1_p1_off();
	gpio_m1_p2_off();

	gpio_m2_p1_off();
	gpio_m2_p2_off();
}

void dir_forward()
{
	gpio_m1_p1_on();
	gpio_m1_p2_off();

	gpio_m2_p1_on();
	gpio_m2_p2_off();
}

void dir_backward()
{
	gpio_m1_p1_off();
	gpio_m1_p2_on();

	gpio_m2_p1_off();
	gpio_m2_p2_on();
}

void gpio_m1_p1_on()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
}

void gpio_m1_p1_off()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
}

void gpio_m1_p2_on()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
}

void gpio_m1_p2_off()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
}

void gpio_m2_p1_on()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

void gpio_m2_p1_off()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

void gpio_m2_p2_on()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
}

void gpio_m2_p2_off()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
}
