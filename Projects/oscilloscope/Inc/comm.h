/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMM_H
#define __COMM_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Exported constants --------------------------------------------------------*/
#define BUFFERSIZE                       100
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
uint8_t amplifier;
/* Exported functions ------------------------------------------------------- */
void init_spi(void);
void spi_transmit(void);

#endif /* __COMM_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
