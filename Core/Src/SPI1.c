#include "main.h"
#include "SPI1.h"


/*      SPI1 GPIO Config
*       CLK - PA5, MISO - PA6, MOSI - PA7, CS - PF12    
*(1) - Set CLK for GPIOA 
*(2) - Set PA5, PA6, PA7 Alternative function
*(3) - AF5 set for PA5, PA6, PA7
*(4) - HIGH SPEED PA5, PA6, PA7
*(5) - Enable Clock PF12
*(6) - Set PF12 OUTPUT
*(7) - Set PF12 High Speed
*/
void SPI_GPIO_config (void)
{
  
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN_Msk << RCC_AHBENR_GPIOAEN_Pos; /* (1) */
  
  GPIOA->MODER &=~ (3 << GPIO_MODER_MODER5_Pos | 3 << GPIO_MODER_MODER6_Pos | 3 << GPIO_MODER_MODER7_Pos);
  GPIOA->MODER |= (2 << GPIO_MODER_MODER5_Pos | 2 << GPIO_MODER_MODER6_Pos | 2 << GPIO_MODER_MODER7_Pos); /* (2) */
  GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFSEL5_Pos | 5 << GPIO_AFRL_AFSEL6_Pos |5 << GPIO_AFRL_AFSEL7_Pos ); /* (3) */
  GPIOA->OSPEEDR &=~ (3 << GPIO_OSPEEDER_OSPEEDR5_Pos | 3 << GPIO_OSPEEDER_OSPEEDR6_Pos | 3 << GPIO_OSPEEDER_OSPEEDR7_Pos | 3 << GPIO_OSPEEDER_OSPEEDR8_Pos  );
  
  GPIOA->OSPEEDR |= (3 << GPIO_OSPEEDER_OSPEEDR5_Pos | 3 << GPIO_OSPEEDER_OSPEEDR6_Pos | 3 << GPIO_OSPEEDER_OSPEEDR7_Pos | 3 << GPIO_OSPEEDER_OSPEEDR8_Pos ); /* (4) */
  
  GPIOA->MODER &=~ (3 << GPIO_MODER_MODER4_Pos);
  GPIOA->MODER |=  (1 << GPIO_MODER_MODER4_Pos); /* (6) */
  GPIOA->OSPEEDR &=~ (3 << GPIO_OSPEEDER_OSPEEDR4_Pos);
  GPIOA->OSPEEDR |= (3 << GPIO_OSPEEDER_OSPEEDR4_Pos ); /* (7) */
  GPIOA->BSRR |= GPIO_BSRR_BS_4; 
  
  
  //DIO0 PA15 INPUT
  GPIOA->MODER &=~ (3 << GPIO_MODER_MODER15_Pos);
  GPIOA->PUPDR &=~ (3 << GPIO_PUPDR_PUPDR15_Pos);
  GPIOA->MODER &=~ (3 << GPIO_MODER_MODER1_Pos);
  GPIOA->MODER |=  (1 << GPIO_MODER_MODER1_Pos); 

  
   //PA0 input interrupt
RCC->APB2ENR |=RCC_APB2ENR_SYSCFGEN;
GPIOA->MODER &=(~GPIO_MODER_MODER0);																			// PA0- IRQ from 	SX1276 (Rx- Completed, Tx - completed) 		
//GPIOA->PUPDR |=GPIO_PUPDR_PUPDR0_1;																				//Pull_down
SYSCFG->EXTICR[1] |= SYSCFG_EXTICR1_EXTI0_PA;
//EXTI->FTSR |= EXTI_FTSR_TR0_Pos; //Falling
EXTI->RTSR |= EXTI_RTSR_RT0; //rising //rising
	
  NVIC_SetPriority(EXTI0_IRQn, 1); 
  NVIC_EnableIRQ(EXTI0_IRQn); 
	__enable_irq ();
        
}

/*      SPI1  Config          
*(1) - Enable the peripheral clock SPI1 
*(2) - Configure SPI1 in master , fPCLK/16
*(3) - TX and RX Not Empty set interrupts
*(4) - Enable SPI1
*(5) Set priority for SPI1_IRQn 
*(6) Enable SPI1_IRQn 
*/
void SPI_config (void)
{
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN_Msk; /* (1) */
  SPI1->CR1 = ( SPI_CR1_MSTR | SPI_CR1_BR_1 | SPI_CR1_BR_2   | SPI_CR1_SSM | SPI_CR1_SSI  ); /* (2) */
 // SPI1->CR2 = SPI_CR2_RXNEIE | SPI_CR2_TXEIE;  /* (3) */
  SPI1->CR1 |= 1<<SPI_CR1_SPE_Pos; /* (4) */   
  NVIC_SetPriority(SPI1_IRQn, 0); /* (5) */
  NVIC_EnableIRQ(SPI1_IRQn); /* (6) */ 
 
  
}

void SPI1_Write (uint8_t data)
{
   
  while (!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = data; 
  while (!(SPI1->SR & SPI_SR_TXE));
  while ((SPI1->SR & SPI_SR_BSY));   
}

uint8_t SPI1_Read( void)
{
  SPI1->DR = 0; 
  while(!(SPI1->SR & SPI_SR_RXNE));
  uint8_t b = SPI1->DR; 
  while(!(SPI1->SR & SPI_SR_RXNE));
  return b;  
}


void SPI_Read_a (uint8_t reg, uint8_t* data )
{
  cs_low();
  Delay_a();
  while (!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = reg ; 
  while (!(SPI1->SR & SPI_SR_TXE));
  while ((SPI1->SR & SPI_SR_BSY));  

  SPI1->DR = 0; 
  while(!(SPI1->SR & SPI_SR_RXNE));
   SPI1->DR; 
  while(!(SPI1->SR & SPI_SR_RXNE));
  *data = SPI1->DR; 
  Delay_a();
  cs_high();
  Delay_a();
}

uint8_t SPI_Read_b (uint8_t reg )
{
  cs_low();
  Delay_a();
  while (!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = reg ; 
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


void SPI_Write_a (uint8_t reg, uint8_t data)
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



void LED_config (void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN; /* (1) */
  GPIOB->MODER &=~ (3 << GPIO_MODER_MODER0); 
  GPIOB->MODER |=  (1 << GPIO_MODER_MODER0); /* (2) */  
  
  
   //RED
  GPIOB->MODER &=~ (3 << GPIO_MODER_MODER4_Pos);
  GPIOB->MODER |=  (1 << GPIO_MODER_MODER4_Pos); 
  GPIOB->BSRR |= GPIO_BSRR_BS_4; 
  
  //GREEN
  GPIOB->MODER &=~ (3 << GPIO_MODER_MODER1_Pos);
  GPIOB->MODER |=  (1 << GPIO_MODER_MODER1_Pos); 
  GPIOB->BSRR |= GPIO_BSRR_BS_1;
  
  //BLUE
  GPIOB->MODER &=~ (3 << GPIO_MODER_MODER0_Pos);
  GPIOB->MODER |=  (1 << GPIO_MODER_MODER0_Pos);  
  GPIOB->BSRR |= GPIO_BSRR_BS_0;
  
  //RED MAIN
  GPIOB->MODER &=~ (3 << GPIO_MODER_MODER5_Pos);
  GPIOB->MODER |=  (1 << GPIO_MODER_MODER5_Pos);  
  GPIOB->BSRR |= GPIO_BSRR_BR_5;
  

}

void txdone_exti (void)
{

  SYSCFG->EXTICR[1] &=~ SYSCFG_EXTICR2_EXTI4;
  SYSCFG->EXTICR[1] |= 1 << SYSCFG_EXTICR2_EXTI4_Pos;
   
  EXTI->RTSR |= EXTI_RTSR_TR4;
  EXTI->PR |= EXTI_PR_PR4;
  EXTI->IMR |= EXTI_IMR_MR4; 
  NVIC_EnableIRQ(EXTI4_IRQn);
}



void Delay_a (void)
{
  volatile uint32_t i;
        for (i=0; i != 0x100; i++);
  
}