/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
void LED_config (void);
void exti_config (void);
void Delay_a (void);
void MCO_config (void);
#define led_redmain_high()   (GPIOB->BSRR |= GPIO_BSRR_BS_5) 
#define led_redmain_low() (GPIOB->BSRR |= GPIO_BSRR_BR_5) 
#define led_green_high()   (GPIOB->BSRR |= GPIO_BSRR_BR_1)  
#define led_green_low() (GPIOB->BSRR |= GPIO_BSRR_BS_1) 
#define led_red_high()   (GPIOB->BSRR |= GPIO_BSRR_BR_4) 
#define led_red_low() (GPIOB->BSRR |= GPIO_BSRR_BS_4) 
#define led_blue_high()   (GPIOB->BSRR |= GPIO_BSRR_BR_0) 
#define led_blue_low() (GPIOB->BSRR |= GPIO_BSRR_BS_0)

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
