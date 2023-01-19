/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Pinout Configuration
*/
void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 2 */
void LED_config (void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN; /* PORTB CLOCK ENABLE */
  GPIOB->MODER &=~ (3 << GPIO_MODER_MODER0); 
  GPIOB->MODER |=  (1 << GPIO_MODER_MODER0); 
  GPIOB->MODER &=~ (3 << GPIO_MODER_MODER4_Pos);     //RED   
  GPIOB->MODER |=  (1 << GPIO_MODER_MODER4_Pos); 
  GPIOB->BSRR |= GPIO_BSRR_BS_4; 
  GPIOB->MODER &=~ (3 << GPIO_MODER_MODER1_Pos);      //GREEN
  GPIOB->MODER |=  (1 << GPIO_MODER_MODER1_Pos); 
  GPIOB->BSRR |= GPIO_BSRR_BS_1;
  GPIOB->MODER &=~ (3 << GPIO_MODER_MODER0_Pos);      //BLUE 
  GPIOB->MODER |=  (1 << GPIO_MODER_MODER0_Pos);  
  GPIOB->BSRR |= GPIO_BSRR_BS_0;
  GPIOB->MODER &=~ (3 << GPIO_MODER_MODER5_Pos);      //RED MAIN
  GPIOB->MODER |=  (1 << GPIO_MODER_MODER5_Pos);  
  GPIOB->BSRR |= GPIO_BSRR_BR_5;
}

void exti_config (void)
{
       //PA0, PA15 input interrupt
    RCC->APB2ENR |=RCC_APB2ENR_SYSCFGEN;
    GPIOA->MODER &=(~GPIO_MODER_MODER0);	
    GPIOA->MODER &=(~GPIO_MODER_MODER15);  
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR1_EXTI0_PA;
    SYSCFG->EXTICR[2] |= SYSCFG_EXTICR2_EXTI5;
    EXTI->PR |= EXTI_PR_PR0 | EXTI_PR_PR15; 
    EXTI->RTSR |= EXTI_RTSR_RT0 | EXTI_RTSR_RT15;  ; //rising
    EXTI->IMR |= EXTI_IMR_MR0 | EXTI_IMR_MR15;
    NVIC_SetPriority(EXTI0_IRQn, 0);
    NVIC_SetPriority(EXTI15_10_IRQn, 0);
    NVIC_EnableIRQ(EXTI0_IRQn); 
    NVIC_EnableIRQ(EXTI15_10_IRQn); 
    __enable_irq ();
}

void MCO_config (void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 
  GPIOA->MODER &=~ (3 <<GPIO_MODER_MODER8_Pos); 
  GPIOA->MODER |=  (2 << GPIO_MODER_MODER8_Pos); 
  GPIOA->OSPEEDR &=~(3 << GPIO_OSPEEDER_OSPEEDR8_Pos);
  GPIOA->OSPEEDR |= (3 << GPIO_OSPEEDER_OSPEEDR8_Pos);
  GPIOA->AFR[1] &=~(15 << GPIO_AFRH_AFSEL8_Pos);
  GPIOA->AFR[1] |= (0 << GPIO_AFRH_AFSEL8_Pos);
  RCC->CFGR &=~(7 <<RCC_CFGR_MCOPRE_Pos);
  RCC->CFGR &=~(7 <<RCC_CFGR_MCOSEL_Pos);
  RCC->CFGR |= (1 <<RCC_CFGR_MCOSEL_Pos);
}
void Delay_a (void)
{
  volatile uint32_t i;
  for (i=0; i != 0x100; i++);  
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
