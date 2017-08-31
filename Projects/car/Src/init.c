/* Includes ------------------------------------------------------------------*/
#include "init.h"
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stm32l4xx_hal_tim.h"

/* Private typedef -----------------------------------------------------------*/
#define PWM_PERIOD_VALUE	100
/* Private define ------------------------------------------------------------*/
#define PWM_PULSE_VALUE    (PWM_PERIOD_VALUE/2)	/* Duty cycle 50%  */

/* Private macro -------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* UART variables */
UART_HandleTypeDef uart_handle;

/* PWM variables */
TIM_HandleTypeDef tim2_pwm_handle;
TIM_HandleTypeDef tim3_pwm_handle;
TIM_OC_InitTypeDef tim2_pwm_conf;
TIM_OC_InitTypeDef tim3_pwm_conf;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
void error_handling(const char *error_string, uint8_t error_code);

void uart_init()
{
	/* Initialize UART: COM1 port, Baudrate 115200, 8bit buffer length, 1 stop bit, no parity mode */
	uart_handle.Instance = DISCOVERY_COM1;
	uart_handle.Init.BaudRate = 115200;
	uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
	uart_handle.Init.StopBits = UART_STOPBITS_1;
	uart_handle.Init.Parity = UART_PARITY_NONE;
	uart_handle.Init.Mode = UART_MODE_TX_RX;
	uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;
	uart_handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	uart_handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	BSP_COM_Init(COM1, &uart_handle);
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	/* e.g. write a character to the USART1 and Loop until the end of transmission */
	HAL_UART_Transmit(&uart_handle, (uint8_t *)&ch, 1, 0xFFFF);

	return ch;
}

void tim2_pwm_init()
{
	uint32_t prescalervalue = 20;

	/* Set TIM2 instance */
	tim2_pwm_handle.Instance = TIM2;

	/* Initialize TIMx peripheral as follows:
	   + Prescaler = (SystemCoreClock / 380000) - 1
	   + Period = 100
	   + ClockDivision = 0
	   + Counter direction = Up
	*/
	tim2_pwm_handle.Init.Period            = PWM_PERIOD_VALUE;
	tim2_pwm_handle.Init.Prescaler         = prescalervalue;
	tim2_pwm_handle.Init.ClockDivision     = 0;
	tim2_pwm_handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	tim2_pwm_handle.Init.RepetitionCounter = 0;

	if (HAL_TIM_PWM_Init(&tim2_pwm_handle) != HAL_OK)
	{
		/* Initialization Error */
		error_handling("TIM Base initialization has failed!", HAL_ERROR);
	}

	tim2_pwm_conf.OCFastMode = TIM_OCFAST_DISABLE;
	tim2_pwm_conf.OCIdleState = TIM_OCIDLESTATE_RESET;
	tim2_pwm_conf.OCMode = TIM_OCMODE_PWM1;
	tim2_pwm_conf.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim2_pwm_conf.Pulse = PWM_PULSE_VALUE;

	/* Set the pulse value for channel 1 */
	if (HAL_TIM_PWM_ConfigChannel(&tim2_pwm_handle, &tim2_pwm_conf, TIM_CHANNEL_1) != HAL_OK)
	{
		/* Configuration Error */
		error_handling("TIM2 PWM channel configuration has failed!", HAL_ERROR);
	}
}

void tim3_pwm_init()
{
	/* Compute the prescaler value to have TIM3 counter clock equal to 38 kHz */
	uint8_t prescalervalue = 20;

	/* Set TIM3 instance */
	tim3_pwm_handle.Instance = TIM3;

	/* Initialize TIMx peripheral as follows:
	   + Prescaler = (SystemCoreClock / 380000) - 1
	   + Period = 100
	   + ClockDivision = 0
	   + Counter direction = Up
	*/
	tim3_pwm_handle.Init.Prescaler         = prescalervalue;
	tim3_pwm_handle.Init.Period            = PWM_PERIOD_VALUE;
	tim3_pwm_handle.Init.ClockDivision     = 0;
	tim3_pwm_handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	tim3_pwm_handle.Init.RepetitionCounter = 0;

	if (HAL_TIM_PWM_Init(&tim3_pwm_handle) != HAL_OK)
	{
		/* Initialization Error */
		error_handling("TIM PWM initialization has failed!", HAL_ERROR);
	}

	tim3_pwm_conf.OCFastMode = TIM_OCFAST_DISABLE;
	tim3_pwm_conf.OCIdleState = TIM_OCIDLESTATE_RESET;
	tim3_pwm_conf.OCMode = TIM_OCMODE_PWM1;
	tim3_pwm_conf.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim3_pwm_conf.Pulse = PWM_PULSE_VALUE;

	/* Set the pulse value for channel 1 */
	if (HAL_TIM_PWM_ConfigChannel(&tim3_pwm_handle, &tim3_pwm_conf, TIM_CHANNEL_1) != HAL_OK)
	{
		/* Configuration Error */
		error_handling("TIM PWM3 channel configuration has failed!", HAL_ERROR);
	}
}

void direction_ctrl_pin_init()
{
//	Clocks already enabled for PWM
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* CONFIGURE MOTOR1 PINS
	 * M1_P1: D12 = PA6
	 * M1_P2: D11 = PA7
	 */
	GPIO_InitTypeDef  gpio_init_m1_p1;
	GPIO_InitTypeDef  gpio_init_m1_p2;

	gpio_init_m1_p1.Pin = GPIO_PIN_6;
	gpio_init_m1_p1.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_m1_p1.Pull = GPIO_NOPULL;
	gpio_init_m1_p1.Speed = GPIO_SPEED_FREQ_HIGH;

	gpio_init_m1_p2.Pin = GPIO_PIN_7;
	gpio_init_m1_p2.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_m1_p2.Pull = GPIO_NOPULL;
	gpio_init_m1_p2.Speed = GPIO_SPEED_FREQ_HIGH;

	/* CONFIGURE MOTOR2 PINS
	 * D7 = PA4
	 * D8 = PB2
	 */
	GPIO_InitTypeDef  gpio_init_m2_p1;
	GPIO_InitTypeDef  gpio_init_m2_p2;

	gpio_init_m2_p1.Pin = GPIO_PIN_4;
	gpio_init_m2_p1.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_m2_p1.Pull = GPIO_NOPULL;
	gpio_init_m2_p1.Speed = GPIO_SPEED_FREQ_HIGH;

	gpio_init_m2_p2.Pin = GPIO_PIN_2;
	gpio_init_m2_p2.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_m2_p2.Pull = GPIO_NOPULL;
	gpio_init_m2_p2.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOA, &gpio_init_m1_p1);
	HAL_GPIO_Init(GPIOA, &gpio_init_m1_p2);
	HAL_GPIO_Init(GPIOA, &gpio_init_m2_p1);
	HAL_GPIO_Init(GPIOB, &gpio_init_m2_p2);
}

void error_handling(const char *error_string, uint8_t error_code)
{
	printf("Error: %s Error code: %d", error_string, error_code);
	BSP_LED_On(LED2);
}
