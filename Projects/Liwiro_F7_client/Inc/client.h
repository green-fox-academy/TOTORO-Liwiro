/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SOCKET_CLIENT_H
#define __SOCKET_CLIENT_H

#define SERVER_IP           "10.27.99.111"
#define SERVER_PORT         13003
#define DATA_BUFFER_SIZE    1024

#define CIRCLE_X 			136
#define CIRCLE_Y			136
#define CIRCLE_R			100

/* Includes ------------------------------------------------------------------*/
#include "stm32746g_discovery_ts.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void connect_to_server();
int send_message(int32_t length);
int send_message_stop(void);
uint8_t clicked_circle(void);
int32_t sqrt_measure(int32_t x);

#endif /* __SOCKET_CLIENT_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
