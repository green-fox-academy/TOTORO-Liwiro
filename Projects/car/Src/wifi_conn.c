/* Includes ------------------------------------------------------------------*/
#include "wifi_conn.h"
#include "motor_control.h"
#include "init.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SSID     				"A66 Guest"
#define PASSWORD 				"Hello123"
#define SERVER_PORT 			13003
#define WIFI_READ_TIMEOUT 		1000
#define CONNECTION_TRIAL_MAX    10
/* Private macro -------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
uint8_t mac_addr[6];
uint8_t ip_addr[4];
uint8_t ip[] = {10, 27, 99, 161};
uint8_t firm_ip[] = {10, 27, 99, 161};
int32_t socket;
uint16_t datalen;
uint8_t connections = 0;
uint8_t command;

/* Private function prototypes -----------------------------------------------*/
void error_handling(const char *error_string, uint8_t error_code);
/* Private functions ---------------------------------------------------------*/
void send_ps_command()
{
    /*Initialize  WIFI */
    wifi_init();
    connections = 0;
    while (1) {


    	/*Waiting for connection with WIFI AP */
    	printf("> Connecting to %s.\n", SSID);
        while (WIFI_Connect(SSID, PASSWORD, WIFI_ECN_WPA2_PSK) != WIFI_STATUS_OK);
		printf("> Connected to %s!\n", SSID);

    	/*Getting IP Address */
		if (WIFI_GetIP_Address(ip_addr) == WIFI_STATUS_OK) {
			printf("> es-wifi module got IP Address : %d.%d.%d.%d\n",
					ip_addr[0],
					ip_addr[1],
					ip_addr[2],
					ip_addr[3]);
			printf("\n\r\n IP address assigned \n\r\n");
		} else {
			error_handling("> ERROR : es-wifi module CANNOT get IP address\n", WIFI_STATUS_ERROR);
		}
    	/*do-while connected to WIFI AP(checking connection by pinging own IP Address) */
		do {
			int8_t socket = 0;
			printf("Start TCP Server...\n");
			while(WIFI_StartServer(socket, WIFI_TCP_PROTOCOL, "LIWIRO SERVER", SERVER_PORT) != WIFI_STATUS_OK);
			connections++;
			printf("----------------------------------------- \n");
			printf("TCP Server Started \n");
			printf("receiving data...\n");
			/*trying to connect to server and sending data when connected in every 10 seconds */
			do {
				if(datalen > 0) {
					printf("Received message from Controller\r\n");
					if (command == 'w') {
						go_forward();
						printf("going forward\r\n");
					} else if (command == 's') {
						go_backward();
						printf("going backwards\r\n");
					} else if (command == 'a') {
						go_left();
						printf("left\r\n");
					} else if (command == 'd') {
						go_right();
						printf("right\n");
					} else if (command == 'x') {
						ctrl_stop();
						printf("stop\n");
					} else {
						printf("wrong command!\n");
						printf("%d\n", command);
					}
					datalen = 0;
				}
			} while (WIFI_ReceiveData(socket, &command, sizeof(command), &datalen, 0) == WIFI_STATUS_OK);
			/*Closing socket when connection is lost or could'not connect */
			printf("Closing the socket...\n");
			WIFI_CloseClientConnection(socket);
			WIFI_StopServer(socket);
		} while (wifi_isconnected() == 1 && connections < 4); //do-while
		/*If there might be a problem with pinging, disconnect from WIFI AP anyway */
		connections = 0;
		printf("> Disconnected from WIFI!\n");
		WIFI_Disconnect();
    }	//while (1)
}	//main

void wifi_init()
{
    /*Initialize  WIFI module */
	if(WIFI_Init() ==  WIFI_STATUS_OK) {
        printf("> WIFI Module Initialized.\n");
	} else {
	    error_handling("> ERROR : WIFI Module cannot be initialized.\n", WIFI_STATUS_ERROR);
	}
	/*Getting MAC address */
    if(WIFI_GetMAC_Address(mac_addr) == WIFI_STATUS_OK) {
        printf("> es-wifi module MAC Address : %X:%X:%X:%X:%X:%X\n",
               mac_addr[0],
               mac_addr[1],
               mac_addr[2],
               mac_addr[3],
               mac_addr[4],
               mac_addr[5]);
    } else {
        error_handling("> ERROR : CANNOT get MAC address\n", WIFI_STATUS_ERROR);
    }
}
