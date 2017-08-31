#include "print_screen.h"
#include "program_gui.h"
#include "stm32f7xx_hal.h"

#include "main.h"
#include "WM.h"
#include "GUI.h"
#include "DIALOG.h"
#include "stm32746g_discovery_ts.h"
#include "adc.h"
#include "data.h"
#include "comm.h"
#include "sd_card.h"
#include "ffconf.h"
#include "sd_diskio.h"

void write_to_file(uint8_t data, void * p)
{
	uint byteswritten;
	f_write(((FIL *)p), &data, 1, &byteswritten);
}

void save_screen(void)
{
	fat_fs();
	if (sd_card_in == 0){
		TEXT_SetText(error_handling, "No sd card has found");
		GUI_Delay(3000);
		TEXT_SetText(error_handling, "");
		return;
	}
	char file_name[15];
	sprintf(file_name, "scr_%04d.bmp", picture_cntr);
	FILINFO* fno;
	while(f_stat(file_name, fno) != FR_NO_FILE){
		picture_cntr++;
		sprintf(file_name, "scr_%04d.bmp", picture_cntr);
	}
	f_open(&MyFile, file_name, FA_CREATE_ALWAYS | FA_WRITE);
	GUI_BMP_Serialize(write_to_file, &MyFile);
	f_close(&MyFile);
	picture_cntr++;
	f_mount(NULL, (TCHAR const*)SDPath, 1);
	FATFS_UnLinkDriver(SDPath);
}
