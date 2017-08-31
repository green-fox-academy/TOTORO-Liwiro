/* Includes ------------------------------------------------------------------*/
#include "comm.h"
#include "stm32f7xx_hal.h"
#include "lcd_log.h"
#include "cmsis_os.h"

/* SPI handler declaration */
SPI_HandleTypeDef spi_handle;

/* Buffer used for transmission */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void init_spi(void)
{
	__HAL_RCC_SPI2_CLK_ENABLE();

	spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	spi_handle.Init.Direction         = SPI_DIRECTION_2LINES;
	spi_handle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	spi_handle.Init.CLKPolarity       = SPI_POLARITY_LOW;
	spi_handle.Init.DataSize          = SPI_DATASIZE_16BIT;
	spi_handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	spi_handle.Init.TIMode            = SPI_TIMODE_DISABLE;
	spi_handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	spi_handle.Init.CRCPolynomial     = 7;
	spi_handle.Init.NSS               = SPI_NSS_HARD_OUTPUT;
	spi_handle.Init.Mode 			  = SPI_MODE_MASTER;
	spi_handle.Instance               = SPI2;
	spi_handle.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	spi_handle.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;

	HAL_SPI_Init(&spi_handle);
}
void spi_transmit(void)
{
	uint8_t buffer_trans[2];
    switch(amplifier) {
    case(1):
		buffer_trans[0] = 0;
    	break;
    case(2):
		buffer_trans[0] = 1;
    	break;
    case(4):
		buffer_trans[0] = 2;
    	break;
    case(5):
		buffer_trans[0] = 3;
    	break;
    case(8):
		buffer_trans[0] = 4;
    	break;
    case(10):
		buffer_trans[0] = 5;
    	break;
    case(16):
		buffer_trans[0] = 6;
    	break;
    case(32):
		buffer_trans[0] = 7;
    	break;
    default:
		buffer_trans[0] = 0;
		break;
    }
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_RESET);
	buffer_trans[1] = 0x40;
	HAL_SPI_Transmit(&spi_handle, buffer_trans, 1, 1000);
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET);
}

