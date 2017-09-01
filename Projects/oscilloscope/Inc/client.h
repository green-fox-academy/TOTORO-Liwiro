/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CLIENT_H
#define __CLIENT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32746g_discovery_ts.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define CLIENT_BUFF_LEN				10
#define CLIENT_SEND_INTERVAL		10
#define CLIENT_RECONNECT_INTERVAL	1000
#define CLIENT_SERVER_IP			"10.27.6.66"
#define SERVER_PORT					"10.27.99.212"

/* Exported functions ------------------------------------------------------- */
void socket_client_thread(void const *argument);

#endif /* __CLIENT_H */
 
