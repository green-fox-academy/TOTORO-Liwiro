/* Includes ------------------------------------------------------------------*/
#include "adc.h"
#include "stm32f7xx_hal.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "WM.h"
#include "GUI.h"
#include "program_gui.h"
#include "timers.h"
#include "data.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void adc_init()
{
	buffer_mem_address = (int16_t *)BUFFER_ADDRESS;
	buffer_index = 0;
	/* Init ADC3*/
	__HAL_RCC_ADC3_CLK_ENABLE();
	/* ADC speed is determined by the ADC resolution (12, 10, 8 or 6 bit), the sampling time, and prescaler.
	 * Max speed with 8 bits and the minimum (3 cycles) sampling time and the smallest prescaler: 8 cycles + 3 cycles = 11 cycles.
	 */
	adc_handle.State = HAL_ADC_STATE_RESET;
	adc_handle.Instance = ADC3;
	adc_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	adc_handle.Init.Resolution = ADC_RESOLUTION_8B;
	adc_handle.Init.DMAContinuousRequests = ENABLE;
	adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc_handle.Init.DiscontinuousConvMode = DISABLE;
	adc_handle.Init.ContinuousConvMode = DISABLE;
	adc_handle.Init.ScanConvMode = DISABLE;
	adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	adc_handle.Init.NbrOfDiscConversion = 0;
	adc_handle.Init.NbrOfConversion = 1;
	adc_handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	HAL_ADC_Init(&adc_handle);

	adc_ch_conf.Channel = ADC_CHANNEL_7;
	adc_ch_conf.Offset = 0;
	adc_ch_conf.Rank = 1;
	adc_ch_conf.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);

}

void fast_adc_init()
{
	HAL_ADC_Stop_DMA(&adc_handle);
	HAL_ADC_DeInit(&adc_handle);
	buffer_index = 0;
	/* Init ADC3 in fast mode*/
	/* ADC speed is determined by the ADC resolution (12, 10, 8 or 6 bit), the sampling time, and prescaler.
	 * Max speed with 8 bits and the minimum (3 cycles) sampling time and the smallest prescaler: 8 cycles + 3 cycles = 11 cycles.
	 */
	fast_adc_handle.State = HAL_ADC_STATE_RESET;
	fast_adc_handle.Instance = ADC3;
	fast_adc_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	fast_adc_handle.Init.Resolution = ADC_RESOLUTION_8B;
	fast_adc_handle.Init.DMAContinuousRequests = ENABLE;
	fast_adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	fast_adc_handle.Init.DiscontinuousConvMode = DISABLE;
	fast_adc_handle.Init.ContinuousConvMode = ENABLE;
	fast_adc_handle.Init.ScanConvMode = DISABLE;
	fast_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	fast_adc_handle.Init.NbrOfDiscConversion = 0;
	fast_adc_handle.Init.NbrOfConversion = 1;
	fast_adc_handle.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	HAL_ADC_Init(&fast_adc_handle);

	fast_adc_ch_conf.Channel = ADC_CHANNEL_7;
	fast_adc_ch_conf.Offset = 0;
	fast_adc_ch_conf.Rank = 1;
	fast_adc_ch_conf.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	HAL_ADC_ConfigChannel(&fast_adc_handle, &fast_adc_ch_conf);
}

void dma_init(void)
{
	__HAL_RCC_DMA2_CLK_ENABLE();
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (mode_selected == 1) {
		HAL_ADC_Start_DMA(&adc_handle, (uint32_t*)buffer_mem_address, BUFFER_LEN);
	} else {
		NVIC_DisableIRQ(TIM2_IRQn);
		drawing_ready = 1;
	}
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
}

void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
}

