/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INIT_H
#define __INIT_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stm32l4xx_hal_tim.h"

/* Private typedef -----------------------------------------------------------*/
#define PWM_PERIOD_VALUE				100
/* Private define ------------------------------------------------------------*/
#define MAX_PWM_PULSE_VALUE    			PWM_PERIOD_VALUE	/* Duty cycle 100%  */
#define MIN_PWM_PULSE_VALUE				60
#define TIM2_PRESCALER					800
#define TIM3_PRESCALER					800

void uart_init();
void tim3_pwm_init();
void tim2_pwm_init();
void direction_ctrl_pin_init();

#endif /* __INIT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
