#include "DIALOG.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "projector_client.h"
#include "WindowDLG.h"
#include "cmsis_os.h"
#include "GUI.h"
#include "WM.h"

#define ID_WINDOW_0 (GUI_ID_USER + 0x03)
#define ID_BUTTON_0 (GUI_ID_USER + 0x04)
#define PROJECTOR_UP		1
#define PROJECTOR_STOP		2
#define PROJECTOR_DOWN		3

osThreadDef(PROJECTOR, projector_client_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);

WM_HWIN main_window;
WM_HWIN hItem;
WM_HWIN swing_button;
WM_HWIN AC_control;
WM_HWIN AC_on_off;
WM_HWIN AC_L_control;

uint8_t ac_state = 0;
uint8_t ac_swing_state = 0;
uint8_t ac_lever_state = 0;
int ac_temperature;
uint8_t ac_controls[5] = {6, 1, 0, 3, 0};
uint8_t proj_control;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 480, 272, 1, 0x0, 0 },
  { BUTTON_CreateIndirect, "UP", ID_BUTTON_0, 410, 27, 50, 50, 0, 0x0, 0 },
};

void _cbDialog(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		hItem = pMsg->hWin;
	    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x77004F5F));
		break;
	case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
		case ID_BUTTON_0:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				proj_control = PROJECTOR_UP;
				osThreadCreate (osThread(PROJECTOR), &proj_control);
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			}
			break;
		}
	}
}

WM_HWIN CreateWindow(void)
{
  main_window = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return main_window;
}

void MainTask(void)
{
   CreateWindow() ;
}
