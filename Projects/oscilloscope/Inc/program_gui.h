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
#define ID_WINDOW_0		(GUI_ID_USER + 0x00)
#define ID_BUTTON_0		(GUI_ID_USER + 0x01)
#define ID_TEXT_0		(GUI_ID_USER + 0x02)

#define ID_IMAGE_0_IMAGE_0   0x00
/* Exported variables --------------------------------------------------------*/
uint8_t GUI_Initialized;
int measurement_index;
char measurement[10];

TIM_HandleTypeDef TimHandle;
uint32_t uwPrescalerValue;

FRAMEWIN_Handle hWin;
LISTBOX_Handle mode_selection;
LISTBOX_Handle measure_modes;
GRAPH_Handle hGraph;
GRAPH_DATA_Handle triggerData;
GRAPH_DATA_Handle runningData;
GRAPH_SCALE_Handle hYScale;
GRAPH_SCALE_Handle hXScale;
RADIO_Handle rising_falling;
TEXT_Handle measure_text;
TEXT_Handle timebase_unit;
TEXT_Handle error_handling;
SLIDER_Handle slider;
DROPDOWN_Handle amplif_cnt;
DROPDOWN_Handle average_cnt;
TEXT_Handle y_axis;

uint8_t measurement_selected;
uint8_t mode_selected;
uint8_t fast_mode_flag; //flag to check whether the ADC is initialized in fast mode
char measured_value_text[50];
uint16_t last_x;
uint16_t last_y;
uint8_t zoomed;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[];

/* Exported functions ------------------------------------------------------- */
void program_gui_init(void);
void wincallback(WM_MESSAGE * pMsg);
void gui_show_graph(WM_MESSAGE * pMsg);
void gui_attach_data();
void gui_graph_set_scale(void);
void gui_graph_set_y_scale(void);
void gui_graph_set_x_scale(void);
void change_x_scale(uint32_t timebase, uint8_t mode_selected);
void set_timebase(void);
void quit_fast_mode(void);
int select_measurement(void);
void gui_task(void);
WM_HWIN CreateWindow(void);
const void * _GetImageById(U32 Id, U32 * pSize);
void user_draw(WM_HWIN hWin, int Stage);
void create_triggerline(void);

#endif /* __GUI_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
