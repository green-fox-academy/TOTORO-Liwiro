/**
  ******************************************************************************
  * @file    Templates/Src/stm32l4xx_hal_msp.c
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    21-April-2017
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/** @addtogroup STM32L4xx_HAL_Driver
  * @{
  */

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */
/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
	/*##-1- Enable peripheral clock #################################*/
	/* TIM2 Peripheral clock enable */
	__HAL_RCC_TIM2_CLK_ENABLE();
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM3) {

		/* GPIO Init structure*/
		GPIO_InitTypeDef gpio_tim3;
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* TIM3 Peripheral clock enable */
		__HAL_RCC_TIM3_CLK_ENABLE();
		/* Enable GPIO Channels Clock */
		__HAL_RCC_GPIOB_CLK_ENABLE();

		/* Configure PB4 (connected to D5) (TIM3_Channel1)*/
		gpio_tim3.Mode = GPIO_MODE_AF_PP;
		gpio_tim3.Pull = GPIO_NOPULL;
		gpio_tim3.Speed = GPIO_SPEED_FREQ_LOW;

		gpio_tim3.Alternate = GPIO_AF2_TIM3;
		gpio_tim3.Pin = GPIO_PIN_4;
		HAL_GPIO_Init(GPIOB, &gpio_tim3);

	} else if (htim->Instance == TIM2) {

		/* GPIO Init structure*/
		GPIO_InitTypeDef gpio_tim2;
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* TIM3 Peripheral clock enable */
		__HAL_RCC_TIM2_CLK_ENABLE();
		/* Enable GPIO Channels Clock */
		__HAL_RCC_GPIOA_CLK_ENABLE();

		/* Configure PA.15 (connected to D9) (TIM2_Channel1)*/
		gpio_tim2.Mode = GPIO_MODE_AF_PP;
		gpio_tim2.Pull = GPIO_NOPULL;
		gpio_tim2.Speed = GPIO_SPEED_FREQ_LOW;

		gpio_tim2.Alternate = GPIO_AF1_TIM2;
		gpio_tim2.Pin = GPIO_PIN_15;
		HAL_GPIO_Init(GPIOA, &gpio_tim2);
	}
}

