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

#define MOTOR_CONTROL_PANEL_X_ORIGO	136
#define MOTOR_CONTROL_PANEL_Y_ORIGO	136

/* Private macro -------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
extern TIM_HandleTypeDef tim2_pwm_handle;
extern TIM_HandleTypeDef tim3_pwm_handle;

uint8_t mac_addr[6];
uint8_t ip_addr[4];
uint8_t ip[] = {10, 27, 99, 161};
uint8_t firm_ip[] = {10, 27, 99, 161};
int32_t socket;
uint16_t datalen;
uint8_t connections = 0;
int command[4];

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
					if (command[0] == 0) {
						printf("command from PC: \n");
						switch(command[1]) {
						case 1:
							printf("%d - go forward\r\n", command[1]);
							go_forward();
							break;
						case 2:
							printf("%d - go backwards\r\n", command[1]);
							go_backward();
							break;
						case 3:
							printf("%d - go left\r\n", command[1]);
							go_left();
							break;
						case 4:
							printf("%d - go right\r\n", command[1]);
							go_right();
							break;
						case 5:
							printf("%d - stop\r\n", command[1]);
							ctrl_stop();
							break;
						default:
							printf("%d - command not recognized\r\n", command[1]);
							break;
						}
					} else if (command[0] == 1) {
						uint32_t pwm_pulse_1;
						uint32_t pwm_pulse_2;
						printf("Coordinates from F7: ");
						printf("x: %d\t", command[1]);
						printf("y: %d\n", command[2]);

						uint8_t side_b;
						uint8_t side_c;

						side_c = command[3];

						if (command[2] > MOTOR_CONTROL_PANEL_Y_ORIGO) { //forward or backward
							printf("go forward\n");
							side_b = command[2] - MOTOR_CONTROL_PANEL_Y_ORIGO;
							dir_forward();

							pwm_pulse_1 = (uint32_t)side_b >> 1;
							pwm_pulse_2 = (uint32_t)side_c >> 1;

							printf("pulse width 1: %u + 50\n", pwm_pulse_1);
							printf("pulse width 2: %u + 50\n", pwm_pulse_2);

							if (command[1] > MOTOR_CONTROL_PANEL_X_ORIGO) {//left of right
								tim2_pwm_handle.Instance->CCR1 = (uint32_t)pwm_pulse_1 + MIN_PWM_PULSE_VALUE;
								tim3_pwm_handle.Instance->CCR1 = (uint32_t)pwm_pulse_2 + MIN_PWM_PULSE_VALUE;
							} else if (command[1] < MOTOR_CONTROL_PANEL_X_ORIGO) {
								tim2_pwm_handle.Instance->CCR1 = (uint32_t)pwm_pulse_2 + MIN_PWM_PULSE_VALUE;
								tim3_pwm_handle.Instance->CCR1 = (uint32_t)pwm_pulse_1 + MIN_PWM_PULSE_VALUE;
							}

						} else if (command[2] < MOTOR_CONTROL_PANEL_Y_ORIGO) { //forward or backward
							printf("go backward\n");
							side_b = MOTOR_CONTROL_PANEL_Y_ORIGO - command[2];
							dir_backward();

							pwm_pulse_1 = (uint32_t)side_b >> 1;
							pwm_pulse_2 = (uint32_t)side_c >> 1;

							printf("pulse width 1: %lu + 50\n", pwm_pulse_1);
							printf("pulse width 2: %lu + 50\n", pwm_pulse_2);

							if (command[1] > MOTOR_CONTROL_PANEL_X_ORIGO) {//left of right
								tim2_pwm_handle.Instance->CCR1 = (uint32_t)pwm_pulse_1 + MIN_PWM_PULSE_VALUE;
								tim3_pwm_handle.Instance->CCR1 = (uint32_t)pwm_pulse_2 + MIN_PWM_PULSE_VALUE;
							} else if (command[1] <= MOTOR_CONTROL_PANEL_X_ORIGO) {
								tim2_pwm_handle.Instance->CCR1 = (uint32_t)pwm_pulse_2 + MIN_PWM_PULSE_VALUE;
								tim3_pwm_handle.Instance->CCR1 = (uint32_t)pwm_pulse_1 + MIN_PWM_PULSE_VALUE;
							}

						} else if (command[2] == MOTOR_CONTROL_PANEL_Y_ORIGO) {
							pwm_pulse_2 = (uint32_t)side_c / 2;
							if (command[1] > MOTOR_CONTROL_PANEL_X_ORIGO) {//left of right
								gpio_m1_p1_on();
								gpio_m1_p2_off();

								gpio_m2_p1_on();
								gpio_m2_p2_on();

								tim3_pwm_handle.Instance->CCR1 = (uint32_t)pwm_pulse_2 + MIN_PWM_PULSE_VALUE;
							} else if (command[1] <= MOTOR_CONTROL_PANEL_X_ORIGO) {
								gpio_m1_p1_on();
								gpio_m1_p2_on();

								gpio_m2_p1_on();
								gpio_m2_p2_off();
								tim2_pwm_handle.Instance->CCR1 = (uint32_t)pwm_pulse_2 + MIN_PWM_PULSE_VALUE;
							}
						}



					} else if (command[0] == 'a') {
						printf("Message from Android\n");
					} else {
						printf("Controller not recognized!\n");
						printf("%d\n", command[0]);
					}
					datalen = 0;
				}
			} while (WIFI_ReceiveData(socket, (uint8_t *)command, sizeof(command), &datalen, 0) == WIFI_STATUS_OK);
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
