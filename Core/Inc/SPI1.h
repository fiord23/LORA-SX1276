#ifndef __SPI1_H
#define __SPI1_H

#include "main.h"
#define cs_low()   (GPIOA->BSRR |= GPIO_BSRR_BR_4) /* PF12 = 0  CS LOW*/ 
#define cs_high() (GPIOA->BSRR |= GPIO_BSRR_BS_4) /* PF12 = 1 CS HIGH*/
void SPI_GPIO_config (void);
void SPI_config (void);
uint8_t SPI1_Read (uint8_t reg );
void SPI1_Write (uint8_t reg, uint8_t data);
#endif