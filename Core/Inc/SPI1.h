#ifndef __SPI1_H
#define __SPI1_H

#include "main.h"



void SPI_GPIO_config (void);
void SPI_config (void);
void LED_config (void);
void SPI1_Write (uint8_t data);
uint8_t  SPI1_Read();
void SPI_Read_a (uint8_t reg, uint8_t* data );
void SPI_Write_a (uint8_t reg, uint8_t data);
uint8_t SPI_Read_b (uint8_t reg );
void Delay_a (void);
void txdone_exti (void);


#define cs_low()   (GPIOA->BSRR |= GPIO_BSRR_BR_4) /* PF12 = 0  CS LOW*/ 
#define cs_high() (GPIOA->BSRR |= GPIO_BSRR_BS_4) /* PF12 = 1 CS HIGH*/

#define led_redmain_high()   (GPIOB->BSRR |= GPIO_BSRR_BS_5) 
#define led_redmain_low() (GPIOB->BSRR |= GPIO_BSRR_BR_5) 

#define led_blue_high()   (GPIOB->BSRR |= GPIO_BSRR_BR_4) 
#define led_blue_low() (GPIOB->BSRR |= GPIO_BSRR_BS_4)

#define led_green_high()   (GPIOB->BSRR |= GPIO_BSRR_BR_1)  
#define led_green_low() (GPIOB->BSRR |= GPIO_BSRR_BS_1) 

#define led_red_high()   (GPIOB->BSRR |= GPIO_BSRR_BR_0) 
#define led_red_low() (GPIOB->BSRR |= GPIO_BSRR_BS_0) 


#endif