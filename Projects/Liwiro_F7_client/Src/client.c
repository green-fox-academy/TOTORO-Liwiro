/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "client.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "lwip/sockets.h"
#include "stm32746g_discovery_ts.h"
#include "init_program.h"
#include "program_gui.h"
#include <string.h>
#include <stdint.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//int client_socket;
int connection_on;

void connect_to_server()
{
	  TEXT_SetText(logo_text, "01");
	  GUI_Delay(50);

	// Creating client socket
	client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
	  TEXT_SetText(logo_text, "02");
	  GUI_Delay(50);

	// Creating server address
	struct sockaddr_in addr_in;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(SERVER_PORT);
	addr_in.sin_addr.s_addr = inet_addr(SERVER_IP);
	  TEXT_SetText(logo_text, "03");
	  GUI_Delay(50);

	// Connecting the client socket to the server
	lwip_connect(client_socket, (struct sockaddr *)&addr_in, sizeof(addr_in));
	  TEXT_SetText(logo_text, "04");
	  GUI_Delay(50);

	connection_on = 1;
	  TEXT_SetText(logo_text, "05");
	  GUI_Delay(50);

	while(connection_on){
	  TEXT_SetText(logo_text, "06");
	  GUI_Delay(50);

//		GUI_Delay(5);
	}
	lwip_close(client_socket);
	  TEXT_SetText(logo_text, "07");
	  GUI_Delay(50);

}

int send_message(int32_t length)
{
	uint8_t msg[4];
        msg[0] = 1;
		msg[1] = (uint8_t)(ts.touchX[0] - CIRCLE_X); // -kor kozepe
		msg[2] = (uint8_t)(ts.touchY[0] - CIRCLE_Y);
		msg[3] = (uint8_t)length;
	// Send the message to the servers
	int sent_bytes = send(client_socket, msg, 4, 0);
	return sent_bytes;
}

int send_message_stop(void)
{
	uint8_t msg[4];
        msg[0] = 1;
		msg[1] = CIRCLE_X;	// kor kozepe vagy 0
		msg[2] = CIRCLE_Y;
		msg[3] = 0;
	// Send the message to the servers
	int sent_bytes = send(client_socket, msg, 3, 0);

	return sent_bytes;
}

uint8_t clicked_circle(void)
{
	int16_t x_len = CIRCLE_X - ts.touchX[0];
	int16_t y_len = CIRCLE_Y - ts.touchY[0];
	int32_t length = sqrt_measure(x_len*x_len) + (y_len*y_len);
	return length;
}

int32_t sqrt_measure(int32_t x)
{
	int32_t op, res, one;
	op = x;
	res = 0;
	one = 1 << 30;
	while (one > op) one >>= 2;
	while (one != 0) {
		if (op >= res + one) {
			op = op - (res + one);
			res = res +  2 * one;
		}
		res /= 2;
		one /= 4;
	}
	return(res);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
