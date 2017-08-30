/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INIT_H
#define __INIT_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stm32l4xx_hal_tim.h"

/* PWM variables */
TIM_HandleTypeDef tim2_pwm_handle;
TIM_HandleTypeDef tim3_pwm_handle;
TIM_OC_InitTypeDef tim2_pwm_conf;
TIM_OC_InitTypeDef tim3_pwm_conf;

void uart_init();
void tim3_pwm_init();
void tim2_pwm_init();

#endif /* __INIT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
