#include "main.h"

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_adc.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_ts.h"
#include "string.h"

#include "init_program.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "timers.h"

#include "program_gui.h"
#include "WM.h"
#include "GUI.h"
#include "DIALOG.h"

void tim2_init()
{
	__HAL_RCC_TIM2_CLK_ENABLE();

	uint32_t tim2_prescaler = 1;

	tim2_handle.Instance = TIM2;
	tim2_handle.State = HAL_TIM_STATE_RESET;
	tim2_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	tim2_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	tim2_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim2_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2_handle.Init.Prescaler = tim2_prescaler;
	tim2_handle.Init.Period = timebase;
	HAL_TIM_Base_Init(&tim2_handle);
	HAL_NVIC_SetPriority(TIM2_IRQn, 2, 2);
}

void tim3_init(void)
{
	uwPrescalerValue = 0;
	uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;

	TimHandle.Instance = TIM3;
	TimHandle.Init.Period = 500 - 1;
	TimHandle.Init.Prescaler = uwPrescalerValue;
	TimHandle.Init.ClockDivision = 0;
	TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_Base_Init(&TimHandle) != HAL_OK) {
		while(1)
		{
		}
	}
	if(HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)	{
		while(1)
		{
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//	if (htim->Instance == TIM2) {
//		adc_handle.Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
//		if (buffer_index < BUFFER_LEN - 1) {
//			buffer_index++;
//		} else {
//			buffer_index = 0;
//		}
//		if(buffer_index >= (BUFFER_LEN - 383))
//			memcpy((buffer_mem_address + buffer_index - BUFFER_LEN), (buffer_mem_address + buffer_index), sizeof(uint16_t));
//	} else {
//		BSP_Background();
//	}
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	HAL_NVIC_SetPriority(TIM3_IRQn, 10, 10);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}
