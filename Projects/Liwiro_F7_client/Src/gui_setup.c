/* Includes ------------------------------------------------------------------*/
#include "socket_client.h"
#include "socket_server.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "app_ethernet.h"
#include "lwip/sockets.h"
#include "stm32746g_discovery_ts.h"
#include <string.h>
#include "gui_setup.h"

 #include "GUI.h"
 #include "DIALOG.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void GUI_Startup()
{
//    GUI_Init();
//	WM_MULTIBUF_Enable(1);
//	GUI_SetLayerVisEx (1, 0);
//	GUI_SelectLayer(0);

	//split screen for weather station and projector canvas control
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	GUI_SetColor(GUI_DARKBLUE);
	GUI_FillRect(0, 0, 100, 272);		//controls
	GUI_FillRect(105, 0, 275, 170);		//temperature
	GUI_FillRect(280, 0, 365, 85);		//humidity
	GUI_FillRect(280, 90, 365, 170);	//air pressure
	GUI_FillRect(105, 175, 365, 272);	//AC control
	GUI_FillRect(370, 0, 480, 217);		//projector canvas control
	GUI_FillRect(370, 222, 480, 272);	//empty
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
