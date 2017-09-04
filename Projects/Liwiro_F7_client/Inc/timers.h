/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMERS_H
#define __TIMERS_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "WM.h"
#include "GUI.h"
#include "GRAPH.h"

/* Exported constants --------------------------------------------------------*/
#define RUNNING_MODE_PERIOD 2000 //Default value in the ARR register in running mode
#define TRIGGER_MODE_PERIOD 100 //Default value in the ARR register in trigger mode
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Variables for TIM2 */
TIM_HandleTypeDef tim2_handle;
uint32_t timebase;

/* Exported functions ------------------------------------------------------- */
void tim2_init();
void tim3_init(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);

#endif /* __DATA_H */
