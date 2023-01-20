#include "SPI1.h"
#include "gpio.h"

void SPI_GPIO_config (void)
{ 
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN_Msk << RCC_AHBENR_GPIOAEN_Pos; 
  GPIOA->MODER &=~ (3 << GPIO_MODER_MODER5_Pos | 3 << GPIO_MODER_MODER6_Pos | 3 << GPIO_MODER_MODER7_Pos | 3 << GPIO_MODER_MODER15_Pos);
  GPIOA->MODER |= (2 << GPIO_MODER_MODER5_Pos | 2 << GPIO_MODER_MODER6_Pos | 2 << GPIO_MODER_MODER7_Pos); 
  GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFSEL5_Pos | 5 << GPIO_AFRL_AFSEL6_Pos |5 << GPIO_AFRL_AFSEL7_Pos ); 
  GPIOA->OSPEEDR &=~ (3 << GPIO_OSPEEDER_OSPEEDR5_Pos | 3 << GPIO_OSPEEDER_OSPEEDR6_Pos | 3 << GPIO_OSPEEDER_OSPEEDR7_Pos | 3 << GPIO_OSPEEDER_OSPEEDR8_Pos  );  
  GPIOA->OSPEEDR |= (3 << GPIO_OSPEEDER_OSPEEDR5_Pos | 3 << GPIO_OSPEEDER_OSPEEDR6_Pos | 3 << GPIO_OSPEEDER_OSPEEDR7_Pos | 3 << GPIO_OSPEEDER_OSPEEDR8_Pos ); 
  GPIOA->MODER &=~ (3 << GPIO_MODER_MODER4_Pos);
  GPIOA->MODER |=  (1 << GPIO_MODER_MODER4_Pos); 
  GPIOA->OSPEEDR &=~ (3 << GPIO_OSPEEDER_OSPEEDR4_Pos);
  GPIOA->OSPEEDR |= (3 << GPIO_OSPEEDER_OSPEEDR4_Pos ); 
  GPIOA->BSRR |= GPIO_BSRR_BS_4;    
}

void SPI_config (void)
{
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN_Msk; /* (1) */
  SPI1->CR1 = ( SPI_CR1_MSTR | SPI_CR1_BR_1 | SPI_CR1_BR_2   | SPI_CR1_SSM | SPI_CR1_SSI  ); /* (2) */
 // SPI1->CR2 = SPI_CR2_RXNEIE | SPI_CR2_TXEIE;  /* (3) */
  SPI1->CR1 |= 1<<SPI_CR1_SPE_Pos; /* (4) */   
  NVIC_SetPriority(SPI1_IRQn, 0); /* (5) */
  NVIC_EnableIRQ(SPI1_IRQn); /* (6) */   
}


uint8_t SPI1_Read (uint8_t reg )
{
  cs_low();
  Delay_a();
  while (!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = reg; 
  while (!(SPI1->SR & SPI_SR_TXE));
  while ((SPI1->SR & SPI_SR_BSY));  
  SPI1->DR = 0; 
  while(!(SPI1->SR & SPI_SR_RXNE));
   SPI1->DR; 
  while(!(SPI1->SR & SPI_SR_RXNE));
  uint8_t data = SPI1->DR; 
  Delay_a();
  cs_high();
  Delay_a();
  return data;
}

void SPI1_Write (uint8_t reg, uint8_t data)
{
  cs_low();
  Delay_a();
  while (!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = reg | 0x80; 
  while (!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = data; 
  while (!(SPI1->SR & SPI_SR_TXE));
  while ((SPI1->SR & SPI_SR_BSY)); 
  Delay_a();
  cs_high();
  Delay_a();
}
