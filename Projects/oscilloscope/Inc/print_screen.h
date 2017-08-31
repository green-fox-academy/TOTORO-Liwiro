 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PRINT_SCREEN_H
#define __PRINT_SCREEN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
uint16_t picture_cntr;
/* Exported functions ------------------------------------------------------- */
void write_to_file(unsigned char Data, void * p);
void save_screen(void);

#endif /* __PRINT_SCREEN_H */
