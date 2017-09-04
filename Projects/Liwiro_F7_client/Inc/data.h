/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DATA_H
#define __DATA_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "WM.h"
#include "GUI.h"
#include "GRAPH.h"

/* Exported constants --------------------------------------------------------*/
#define RISING_EDGE  1
#define FALLING_EDGE -1
#define TIMEBASE_CONSTANT 10
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
GRAPH_DATA_Handle htrigData;
uint8_t trigger_value;
uint16_t trigger_point;
uint16_t trigger_position;
int8_t edge_direction;
uint8_t average_num;
uint8_t drawing_ready;
float timebase_constant;
/* Exported functions ------------------------------------------------------- */
void trigger_run(void);
void fast_trigger_run(void);
uint16_t trigger_func(void);
void running_mode(void);
void graph_drawer(void);
uint16_t find_edge(uint16_t i, int8_t edge);
uint16_t edge_start(void);
uint8_t peaktopeak_voltage(void);
uint16_t frequency_measure (void);
uint32_t rms_measurement(void);
void measurement_start(void);
void print_period(void);
void print_frequency(void);
int32_t sqrt_measure(int32_t x);
void print_rms(void);
void averaging(void);
void zoom_graph(void);

#endif /* __DATA_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
