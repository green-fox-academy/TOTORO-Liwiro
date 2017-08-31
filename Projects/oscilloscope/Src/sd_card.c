#include "sd_card.h"
#include "program_gui.h"

char SDPath[4]; /* SD card logical drive path */
FRESULT res;    /* FatFs function common result code */
uint8_t fat_fs(void)
{
	FATFS_LinkDriver(&SD_Driver, SDPath);
	if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 1) == FR_OK){
		sd_card_in = 1;
	} else {
		sd_card_in = 0;
		f_mount(NULL, (TCHAR const*)SDPath, 1);
		FATFS_UnLinkDriver(SDPath);
	}
	return 0;
}
