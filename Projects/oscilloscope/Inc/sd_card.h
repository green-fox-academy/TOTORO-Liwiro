#ifndef __SD_CARD_H_
#define __SD_CARD_H_

#include "stm32f7xx_hal.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_sd.h"
#include "stm32746g_discovery_lcd.h"
#include "cmsis_os.h"
#include "lcd_log.h"
#include <string.h>
#include <stdint.h>

FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL MyFile;     /* File object */
char SDPath[4]; /* SD card logical drive path */
FRESULT res;    /* FatFs function common result code */
uint8_t sd_card_in;

uint8_t fat_fs(void);
//void write_sd_card(char* file_name, char* line_to_write);
//void read_sd_card(char* file_name);

#endif /* __SD_CARD_H_ */
