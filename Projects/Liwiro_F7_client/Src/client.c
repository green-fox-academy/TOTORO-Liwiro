/* Includes ------------------------------------------------------------------*/
#include "client.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "lwip/sockets.h"
#include "stm32746g_discovery_ts.h"
#include <string.h>
#include <stdint.h>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TS_StateTypeDef TS_State;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void connect_to_server(int *client_sock, unsigned int server_port, char *server_ip)
{
	// Creating client socket
	(*client_sock) = lwip_socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
	//if (client_sock < 0)
		//printf("error creating client socket\n");
	//printf("client socket created successfully\n");

	// Creating server address
	struct sockaddr_in addr_in;
	addr_in.sin_family = PF_INET;
	addr_in.sin_port = htons(server_port);
	addr_in.sin_addr.s_addr = inet_addr(server_ip);

	// Connecting the client socket to the server
	//int connect_retval = lwip_connect(*client_sock, (struct sockaddr *)&addr_in, sizeof(addr_in));
	lwip_connect(*client_sock, (struct sockaddr *)&addr_in, sizeof(addr_in));
	//if (connect_retval < 0)
		//printf("error connecting the client socket to the server\n");
	//printf("client socket connected to the server successfully\n");
}

// Implement this function!
void socket_client_thread(void const *argument)
{
	while(!is_ip_ok())
		osDelay(1);
	//TS_StateTypeDef TS_State;
	// Connect to server
	int client_socket;
	connect_to_server(&client_socket, 54545, "10.27.6.40");
	// Local variables used in the do-while loop
	int sent_bytes;
	//set drawing board
	BSP_LCD_Clear(LCD_COLOR_LIGHTYELLOW);
	BSP_LCD_SetTextColor(LCD_COLOR_DARKMAGENTA);
	char msg[255];
	uint16_t x_coord;
	uint16_t y_coord;
	char x_coord_char[10];
	char y_coord_char[10];
	while (1) {
		//get touch state
		//char msg[255];
		memset(msg, 0, 255);
		memset(x_coord_char, 0, 10);
		memset(y_coord_char, 0, 10);
		TS_StateTypeDef TS_State;
		BSP_TS_GetState(&TS_State);

		// check if touch detected
		if (TS_State.touchDetected > 0 ) {
			BSP_LCD_FillCircle(TS_State.touchX[0],TS_State.touchY[0], 5);
			x_coord = TS_State.touchX[0];
			y_coord = TS_State.touchY[0];
			sprintf(x_coord_char, "%d", x_coord);
			sprintf(y_coord_char, "%d", y_coord);
			strcpy(msg, x_coord_char);
			strcat(msg, " ");
			strcat(msg, y_coord_char);
			strcat(msg, "\n");
			//puts(msg);
			osDelay(23);
			//send touch state
			lwip_send(client_socket, msg, strlen(msg), 0);
			//BSP_TS_GetState(&TS_State);
		}

		osDelay(10);


	}
	//lwip_close(client_socket);
	return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
