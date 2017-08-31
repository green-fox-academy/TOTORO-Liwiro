/* Includes ------------------------------------------------------------------*/
#include "program_gui.h"
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include "init_program.h"

#include "main.h"
#include "WM.h"
#include "GUI.h"
#include "DIALOG.h"
#include "stm32746g_discovery_ts.h"
#include "adc.h"
#include "data.h"
#include "comm.h"
#include "timers.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t last_x;
uint16_t last_y;


/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },			//GUI window
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 410, 48, 70, 18, 0, 0x64, 0 },				//Logo Crispo
  { BUTTON_CreateIndirect, "Stop", ID_BUTTON_0, 350, 100, 70, 70, 0, 0x0, 0 },
};



/*********************************************************************
*
*       program_gui_init
*/
void program_gui_init(void)
{
	/* Initialize TIMER 3 and the CRC CLOCK for the GUI */
	tim3_init();
	__HAL_RCC_CRC_CLK_ENABLE(); /* Enable the CRC Module */

	/* Initialize GUI */
	GUI_Init();

	GUI_Initialized = 1;

	/* Activate the use of memory device feature */
	WM_SetCreateFlags(WM_CF_MEMDEV);
	WM_MULTIBUF_Enable(1);
}

/*********************************************************************
*
*       SHOW GUI
*/
void gui_task(void) {
	CreateWindow();
}

/*********************************************************************
*
*       GUI CALLBACK
*/
void wincallback(WM_MESSAGE * pMsg) {
  const void * pData;
  WM_HWIN      hItem;
  U32          FileSize;
  int          NCode;
  int          Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Window' - GUI window
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));

    //
    // Initialization of 'Text' - Logo Crispo
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "Liwiro");
    TEXT_SetFont(hItem, GUI_FONT_16B_1);

    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
	case ID_BUTTON_0:
		switch(NCode) {
		case WM_NOTIFICATION_CLICKED:
			break;
		case WM_NOTIFICATION_RELEASED:
			break;
		}
      }
      break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

WM_HWIN CreateWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), wincallback, WM_HBKWIN, 0, 0);
  return hWin;
}



void user_draw(WM_HWIN hWin, int Stage)
{
	switch (Stage) {
	case GRAPH_DRAW_FIRST:
		//Grid drawer
		GRAPH_SetGridVis(hGraph, 1);
		break;
	case GRAPH_DRAW_LAST:
		if(26 < ts.touchX[0] && ts.touchX[0] < 410 && ts.touchY[0] < 244) {
			trigger_position = ts.touchX[0] - 26;
			trigger_value = 256 - ts.touchY[0];
			//Trigger level line drawer
			last_x = ts.touchX[0];
			last_y = ts.touchY[0];
			if((mode_selected == 0) || (mode_selected == 2 || (mode_selected == 3))){
				GUI_SetColor(GUI_WHITE);
				GUI_DrawVLine(last_x, 0,  256);
				GUI_SetColor(GUI_RED);
				GUI_DrawHLine(last_y, 27,  410);
			}
			if(mode_selected == 0)
			break;
		} else {
			if((mode_selected == 0) || (mode_selected == 2 || (mode_selected == 3))){
				GUI_SetColor(GUI_WHITE);
			GUI_DrawVLine(last_x, 0,  256);  		//Vertical
				GUI_SetColor(GUI_RED);
			GUI_DrawHLine(last_y, 27,  410);		//Horizontal
			}
			break;
		}
	}
}
