/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_H
#define __GUI_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "main.h"
#include "WM.h"
#include "GUI.h"
#include "DIALOG.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x01)
#define ID_IMAGE_0 (GUI_ID_USER + 0x07)
#define ID_IMAGE_1 (GUI_ID_USER + 0x08)
#define ID_TEXT_0 (GUI_ID_USER + 0x09)
#define ID_IMAGE_2 (GUI_ID_USER + 0x0A)

#define ID_IMAGE_0_IMAGE_0 0x00
#define ID_IMAGE_1_IMAGE_0 0x01
#define ID_IMAGE_2_IMAGE_0 0x02
/* Exported variables --------------------------------------------------------*/
uint8_t GUI_Initialized;

TIM_HandleTypeDef TimHandle;
uint32_t uwPrescalerValue;

FRAMEWIN_Handle hWin;
BUTTON_Handle stop_butt;
TEXT_Handle logo_text;

uint16_t last_x;
uint16_t last_y;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[];

/* Exported functions ------------------------------------------------------- */
const void * _GetImageById(U32 Id, U32 * pSize);
void program_gui_init(void);
void gui_task(void);
void wincallback(WM_MESSAGE * pMsg);
WM_HWIN CreateWindow(void);

#endif /* __GUI_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
