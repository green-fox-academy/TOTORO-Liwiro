/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_adc.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_ts.h"
#include "stm32746g_discovery_lcd.h"

#include "init_program.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "adc.h"
#include "comm.h"
#include "data.h"
#include "ffconf.h"
#include "timers.h"
#include "sd_diskio.h"

#include "program_gui.h"
#include "WM.h"
#include "GUI.h"
#include "DIALOG.h"

/* Private variables ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
TS_StateTypeDef  ts;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MPU_Config(void);
void CPU_CACHE_Enable(void);
/* Private functions ---------------------------------------------------------*/


void system_init(void)
{
	MPU_Config();

	/* Enable the CPU Cache */
	CPU_CACHE_Enable();

	HAL_Init();

	/* Configure the system clock to 200 MHz */
	SystemClock_Config();

	/* Configure BSP */
	BSP_Config();

	dma_init();
	adc_init();
	init_spi();
	trigger_value = 1000 / 12.89;
	trigger_position = 100;
	mode_selected = 4;
	edge_direction = RISING_EDGE;
	amplifier = 1;
	average_num = 2;
	zoomed = 1;
	timebase = TRIGGER_MODE_PERIOD;
	timebase_constant = TIMEBASE_CONSTANT * 0.9;
	tim2_init();
	GUI_Initialized = 0;
	program_gui_init();
	spi_transmit();
}


/**
  * @brief  Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
  * @param  None
  * @retval None
  */
void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;

	/* Disable the MPU */
	HAL_MPU_Disable();

	/* Configure the MPU attributes as WT for SRAM */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x20010000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Enable the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  Initializes the STM327546G-Discovery's LCD  resources.
  * @param  None
  * @retval None
  */
 void BSP_Config(void)
{
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
	/* Initialize LED1 (green led) */
	BSP_LED_Init(LED1);

	/* Initialize TS */
	BSP_TS_Init(LCD_GetXSize(), LCD_GetYSize());

	/* Initializes the SDRAM device */
	BSP_SDRAM_Init();
}

void CPU_CACHE_Enable(void)
 {
	/* Enable branch prediction */
	SCB->CCR |= (1 <<18);
	__DSB();

	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_EnableDCache();
 }

/**
* @brief  BSP_Background.
* @param  None
* @retval None
*/
void BSP_Background(void)
{
	if(GUI_Initialized == 1)
		BSP_Pointer_Update();
}

void BSP_Pointer_Update(void)
{
	GUI_PID_STATE TS_State;
	static TS_StateTypeDef prev_state;
	uint16_t xDiff, yDiff;
	BSP_TS_GetState(&ts);
	TS_State.Pressed = ts.touchDetected;
	xDiff = (prev_state.touchX[0] > ts.touchX[0]) ? (prev_state.touchX[0] - ts.touchX[0]) : (ts.touchX[0] - prev_state.touchX[0]);
	yDiff = (prev_state.touchY[0] > ts.touchY[0]) ? (prev_state.touchY[0] - ts.touchY[0]) : (ts.touchY[0] - prev_state.touchY[0]);

	if ((prev_state.touchDetected != ts.touchDetected) || (xDiff > 1) || (yDiff > 1)) {
		prev_state.touchDetected = ts.touchDetected;
		if ((ts.touchX[0] != 0) &&  (ts.touchY[0] != 0)) {
			prev_state.touchX[0] = ts.touchX[0];
			prev_state.touchY[0] = ts.touchY[0];
		}
		TS_State.Layer = 0;
		TS_State.x = prev_state.touchX[0];
		TS_State.y = prev_state.touchY[0];
		GUI_TOUCH_StoreStateEx(&TS_State);
	}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 200000000
  *            HCLK(Hz)                       = 200000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 400
  *            PLL_P                          = 2
  *            PLL_Q                          = 8
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 6
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 400;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 8;

	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(ret != HAL_OK){
		while(1) { ; }
	}

	/* Activate the OverDrive to reach the 200 MHz Frequency */
	ret = HAL_PWREx_EnableOverDrive();
	if(ret != HAL_OK) {
		while(1) { ; }
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
	if(ret != HAL_OK) {
		while(1) { ; }
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

