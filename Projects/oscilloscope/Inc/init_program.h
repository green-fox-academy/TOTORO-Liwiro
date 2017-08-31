/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INIT_H
#define __INIT_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Exported constants --------------------------------------------------------*/
/* Size of buffer */
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
TS_StateTypeDef  ts;
/* Exported functions ------------------------------------------------------- */
void system_init(void);
void BSP_Background(void);
void BSP_Pointer_Update(void);
void BSP_Config(void);
void MPU_Config(void);
void SystemClock_Config(void);
void CPU_CACHE_Enable(void);

#endif /* __INIT_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
