/* Includes ------------------------------------------------------------------*/
#include "data.h"
#include "adc.h"
#include "stm32f7xx_hal.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "main.h"
#include "adc.h"
#include "GUI.h"
#include "WM.h"
#include "program_gui.h"
#include "comm.h"
#include "timers.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void trigger_run(void)
{
	drawing_ready = 0;
	HAL_ADC_Start_DMA(&adc_handle, (uint32_t*)buffer_mem_address, BUFFER_LEN);
	tim2_handle.Instance->ARR = timebase;
	NVIC_EnableIRQ(TIM2_IRQn);
	BSP_LED_Toggle(LED1);
	while (drawing_ready == 0) {
		GUI_Exec();
	}
	graph_drawer();
}

void fast_trigger_run(void)
{
	drawing_ready = 0;
	HAL_ADC_Start_DMA(&fast_adc_handle, (uint32_t*)buffer_mem_address, BUFFER_LEN);
	HAL_Delay(30);
	if (drawing_ready == 1){
		graph_drawer();
	}
}

void running_mode(void)
{
	buffer_index = 0;
	HAL_ADC_Start_DMA(&adc_handle, (uint32_t*)buffer_mem_address, BUFFER_LEN);
	tim2_handle.Instance->ARR = timebase * 1000;
	NVIC_EnableIRQ(TIM2_IRQn);
	while(mode_selected == 1) {
		GRAPH_DATA_YT_Clear(triggerData);
		GRAPH_DATA_YT_Delete(triggerData);
		GRAPH_DetachData(hGraph, triggerData);
		triggerData = GRAPH_DATA_YT_Create(GUI_GREEN, 383, (buffer_mem_address + buffer_index - 383), 383);
		GRAPH_DATA_YT_SetAlign(triggerData, GRAPH_ALIGN_LEFT);
		GRAPH_AttachData(hGraph, triggerData);
		GUI_Delay(50);
	}
	NVIC_DisableIRQ(TIM2_IRQn);
}

uint16_t trigger_func(void)
{
	uint16_t i = edge_start();
	if (i == 400){
		for(; i < (BUFFER_LEN-400); i++){
			if((*(buffer_mem_address + i - 1)*edge_direction) < (trigger_value*edge_direction)){
				if((*(buffer_mem_address + i)*edge_direction) >= (trigger_value*edge_direction)){
					return i;
				}
			}
		}
	}
	for (; i < (BUFFER_LEN - 400); i++) {
		if ((*(buffer_mem_address + i)*edge_direction) >= (trigger_value*edge_direction)) {
			return i;
		}
	}
	return 400;
}

void graph_drawer(void)
{
	GRAPH_DATA_YT_Clear(triggerData);
	trigger_point = trigger_func();
	if (zoomed == 1){
		GRAPH_DATA_YT_Delete(triggerData);
		GRAPH_DetachData(hGraph, triggerData);
		triggerData = GRAPH_DATA_YT_Create(GUI_GREEN, 383, (buffer_mem_address + trigger_point - trigger_position), 383);
		GRAPH_DATA_YT_SetAlign(triggerData, GRAPH_ALIGN_LEFT);
		GRAPH_AttachData(hGraph, triggerData);
	} else if (zoomed == 2){
		zoom_graph();
	}
	GUI_Exec();
	measurement_start();
}

uint16_t find_edge(uint16_t i, int8_t edge)
{
	for(; i < (BUFFER_LEN-400); i++) {
		if((*(buffer_mem_address + i - 1)*edge) < (trigger_value*edge)) {
			if ((*(buffer_mem_address + i)*edge) > (*(buffer_mem_address + i - 1))*edge) {
				if((*(buffer_mem_address + i + 1))*edge >= (*(buffer_mem_address + i))*edge) {
					if((*(buffer_mem_address + i + 2))*edge >= (*(buffer_mem_address + i + 1)*edge)) {
						if((*(buffer_mem_address + i))*edge > (*(buffer_mem_address + i + 5)*edge)) {
						} else {
							break;
						}
					}
				}
			}
		}
	}
	if(i == (BUFFER_LEN-400)){
		for(; i < (BUFFER_LEN-400); i++){
			if((*(buffer_mem_address + i - 1)*edge) < (trigger_value*edge)){
				if((*(buffer_mem_address + i)*edge) >= (trigger_value*edge)){
					break;
				}
			}
		}
	}
	return i;
}

uint16_t edge_start()
{
	uint16_t i = trigger_position;
	i = find_edge(i, edge_direction*(-1));
	i = find_edge(i, edge_direction);
	if((*(buffer_mem_address + i)*edge_direction) > (trigger_value * edge_direction))
		return 400;
	return i;
}

uint8_t peaktopeak_voltage(void)
{
	uint8_t min = 255;
	uint8_t max = 0;
	for (int i = 0; i < 385; i++) {
		if (*(buffer_mem_address + trigger_position + i) < min)
			min = *(buffer_mem_address + trigger_position + i);
		if (*(buffer_mem_address + i) > max)
			max = *(buffer_mem_address + i);
	}
	uint16_t voltage;
	if (min > max){
		voltage = 0;
	} else {
		voltage = (max - min) * 13.2 * 4 / amplifier;
	}
	char str[50];
	sprintf(str, "%d mV", voltage);
	TEXT_SetText(measure_text, (const char*)str);
	return voltage;
}

uint16_t frequency_measure(void)
{
	uint16_t i = trigger_point - 5;
	int32_t freq;
	i = find_edge(i, (edge_direction * (-1)));
	if((i - trigger_point) > 385)
		return 0;
	uint16_t edge = i;
	i = find_edge(i, edge_direction);
	if((i - trigger_point) > 385)
		return 0;
	i = find_edge(i, (edge_direction * (-1)));
	if((i - trigger_point) > 385)
		return 0;
	freq = i - edge;
	return (uint16_t)freq;
}

uint32_t rms_measurement(void)
{
	uint32_t rms = 0;
	uint16_t freq = frequency_measure();
	uint32_t mean = 0;
	for (int i = 0; i < freq; i++){
		mean += (*(buffer_mem_address + trigger_point + i));
	}
	mean /= freq;
	uint32_t value;
	for (int i = 0; i < freq; i++){
		value = ((*(buffer_mem_address + trigger_point + i)) - mean);
		rms += value * value;
	}
	rms /= freq;
	return sqrt_measure(rms);
}

void measurement_start(void)
{
	switch(measurement_selected){
	case 0:
		peaktopeak_voltage();
	    break;
	case 1:
		print_rms();
	    break;
	case 2:
		print_frequency();
	    break;
	case 3:
		print_period();
	    break;
	}
}

void print_frequency(void)
{
	float freq = (float)frequency_measure();
	if(freq == 0){
		TEXT_SetText(measure_text, "-");
	} else {
		freq = (10000 / freq) / timebase_constant;
		char str[50];
		sprintf(str, "%4.2f kHz", freq);
		TEXT_SetText(measure_text, (const char*)str);
	}
}

void print_period(void)
{
	float period_time = (float)frequency_measure();
	if(period_time == 0){
		TEXT_SetText(measure_text, "-");
	} else {
		period_time *= timebase_constant / 10;
		char str[50];
		sprintf(str, "%.1f us", period_time);
		TEXT_SetText(measure_text, (const char*)str);
	}
}

void print_rms(void)
{
	uint16_t rms = rms_measurement() * 13.2 * 4 / amplifier;
	char str[50];
	sprintf(str, "%d mV", rms);
	TEXT_SetText(measure_text, (const char*)str);
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

void averaging(void)
{
	drawing_ready = 0;
	TEXT_SetText(measure_text, "");
	int16_t averaging_sum[383];
	for(int i = 0; i < average_num; i++){
		HAL_ADC_Start_DMA(&adc_handle, (uint32_t*)buffer_mem_address, BUFFER_LEN);
		if (fast_mode_flag != 1) {
			tim2_handle.Instance->ARR = timebase;
			NVIC_EnableIRQ(TIM2_IRQn);
		}
		HAL_Delay(50);
		while (drawing_ready == 0) {
			GUI_Delay(2);
		}
		trigger_point = trigger_func();
		for(int j = 0; j < 383; j++){
			if (i == 0)
				averaging_sum[j] = *(buffer_mem_address + trigger_point - trigger_position + j);
			else
				averaging_sum[j] += *(buffer_mem_address + trigger_point - trigger_position + j);
		}
	}
	uint8_t shift_value;
	switch(average_num){
	case(2):
		shift_value = 1;
		break;
	case(4):
		shift_value = 2;
		break;
	case(8):
		shift_value = 3;
		break;
	case(16):
		shift_value = 4;
		break;
	default:
		shift_value = 1;
		break;
	}
	for(int i = 0; i < 383; i++){
		averaging_sum [i] = averaging_sum[i] >> shift_value;
	}
	GRAPH_DATA_YT_Clear(triggerData);
	GRAPH_DATA_YT_Delete(triggerData);
	GRAPH_DetachData(hGraph, triggerData);
	triggerData = GRAPH_DATA_YT_Create(GUI_GREEN, 460, averaging_sum, 383);
	GRAPH_AttachData(hGraph, triggerData);
	GUI_Exec();
}

void zoom_graph(void)
{
	int16_t zoomed_buff[765];
	for(int i = 0; i < 383; i++){
		zoomed_buff[2*i] = (*(buffer_mem_address + trigger_point - trigger_position + i)) << 1;
	}
	for(int i = 1; i < 383; i++){
		zoomed_buff[2*i -1] = (zoomed_buff[2*i] + zoomed_buff[2*i -2]) >> 1;
	}
	GRAPH_DATA_YT_Clear(triggerData);
	GRAPH_DATA_YT_Delete(triggerData);
	GRAPH_DetachData(hGraph, triggerData);
	triggerData = GRAPH_DATA_YT_Create(GUI_GREEN, 460, zoomed_buff, 383);
	GRAPH_AttachData(hGraph, triggerData);
	GUI_Exec();
}
