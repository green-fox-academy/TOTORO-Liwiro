/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H
/* Includes ------------------------------------------------------------------*/
/* Defines -------------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void go_forward();
void go_backward();
void go_left();
void go_right();
void ctrl_stop();

void gpio_m1_p1_on();
void gpio_m1_p1_off();
void gpio_m1_p2_on();
void gpio_m1_p2_off();

void gpio_m2_p1_on();
void gpio_m2_p1_off();
void gpio_m2_p2_on();
void gpio_m2_p2_off();

#endif /* __MOTOR_CONTROL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
