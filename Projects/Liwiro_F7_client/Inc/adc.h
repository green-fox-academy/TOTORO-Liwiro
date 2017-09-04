/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Exported constants --------------------------------------------------------*/
#define BUFFER_ADDRESS	0xc0600000
#define BUFFER_LEN	3000

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Variables for ADC */
ADC_HandleTypeDef adc_handle;
ADC_HandleTypeDef fast_adc_handle;
ADC_ChannelConfTypeDef adc_ch_conf;
ADC_ChannelConfTypeDef fast_adc_ch_conf;
int16_t *buffer_mem_address;
int16_t buffer_index;

/* Exported functions ------------------------------------------------------- */
void adc_init();
void fast_adc_init();
void dma_init();
void tim2_init();
void tim2_set_autoreload_reg();
void tim2_set_prescaler();
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void TransferComplete(DMA_HandleTypeDef *DmaHandle);
void TransferError(DMA_HandleTypeDef *DmaHandle);


#endif /* __ADC_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
