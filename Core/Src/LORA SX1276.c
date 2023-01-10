#include "main.h"
#include "LORA SX1276.h"
#include "SPI1.h"
#include <stdbool.h>
//#define RECIEVER
//#define TRANSMITTER
/* Lora Config
* (1) - Reset
* (2) - Sleep mode 
*
*/
uint8_t str[] = "Hello!";
uint8_t str_r[16] = {0};

void Lora_init (void)
{  
   Lora_reset();

   if (SPI_Read_b(REG_VERSION) == 0x12)
   {
     led_red_high();       
    Delay_a();
     led_red_low(); 
   }
   else 
   {
     while(1)
     {
        led_blue_high();
        HAL_Delay(200);
        led_blue_low();
        HAL_Delay(200);
     }

   }
   //--------------------------------LORA CONFIGURATION------------------------
   // 868MHz, SF12, 125kHz, 300bps, MaxPower, OcpOn, 9Byte info 	
   SPI_Write_a(REG_OP_MODE, 0x80|MODE_SLEEP); //Lora mode, HF, Sleep //0x81 0x80
   /*--------------Sleep Mode -------------------------*/
   SPI_Write_a(REG_FIFO_TX_BASE_ADDR, 0x00); //new
   SPI_Write_a(REG_FIFO_RX_BASE_ADDR, 0x00); //new
   SPI_Write_a(REG_LNA, 0x23); //new
   SPI_Write_a(REG_PA_DAC, 0x84); //new
   SPI_Write_a(REG_OCP, 0x2b); //new
   SPI_Write_a(REG_PA_CONFIG, 0x8F); //new
   SPI_Write_a(REG_FRF_MSB, 0xD9); //0x86 0xD9
   SPI_Write_a(REG_FRF_MID, 0x00); //0x87 0x00
   SPI_Write_a(REG_FRF_LSB, 0x00); //0x88 0x00
   SPI_Write_a(REG_OP_MODE, 0x81); //new
   /* ------------- Stnadby Mode -----------------*/
   SPI_Write_a(REG_PA_DAC, 0x87); //new
   SPI_Write_a(REG_OCP, 0x31); //new
   SPI_Write_a(REG_PA_CONFIG, 0x8F); //new
   SPI_Write_a(REG_MODEM_CONFIG_1, 0x98); //500 kHz BW, Coding Rate 4/8, Explicit Header Mode
   SPI_Write_a(REG_MODEM_CONFIG_2, 0xC0);  // SF12 , Normal Mode single packet, no CRC
   SPI_Write_a(REG_MODEM_CONFIG_3, 0x04); //LNA gain set by the internal AGC loop
   SPI_Write_a(REG_OP_MODE, 0x81); //new
   SPI_Write_a(REG_FIFO_ADDR_PTR, 0x00); //new
  // SPI_Write_a(REG_PA_CONFIG, 0xFF); //Max power 0x89 0x00
  // SPI_Write_a(REG_OCP, 0x1F);  //OCP-on, Current 130 mA 0x8B 0x1F

 //  SPI_Write_a(REG_LR_PAYLOADMAXLENGTH, 0x10); //16 bytes  0xA3 0x10
   
   // Standart -1
//   SPI_Write_a(REG_LR_IRQFLAGSMASK, 0x48); //0x91 0x48            //Tx_Complete IRQ, RX_Complete IRQ	
 //  SPI_Write_a(REG_LR_PARAMP, 0x09);	 //0x8A 0x09               //Standart 40us
 //  SPI_Write_a(REG_FIFO_TX_BASE_ADDR, 0x80);	// 0x8E 0x80       //Standart
   
 //  SPI_Write_a(REG_FIFO_RX_BASE_ADDR, 0x00);  //  0x8F 0x00       //Standart
 //  SPI_Write_a(REG_LNA, 0x20);	 //0x8C 0x20		        //Standart
   SPI_Write_a(REG_LR_HOPPERIOD, 0x00);	//0xA4 0x00	//Standart  
   SPI_Write_a(REG_OP_MODE, MODE_SLEEP|0x80); // 0x81 0x80
   
   

   SPI_Write_a(REG_DIO_MAPPING_1, RFLR_DIOMAPPING1_DIO0_00);
   SPI_Write_a(REG_SYNC_WORD,0x12);	
   SPI_Write_a(REG_OP_MODE, MODE_STDBY|0x80);
    /* ------------- Stnadby Mode -----------------*/
   SPI_Write_a(REG_FIFO_ADDR_PTR,0x00);
   SPI_Write_a(REG_OP_MODE, MODE_RX_CONTINUOUS|0x80);	 
   /*---------------Receive continuous ---------- */

   
}


void Lora_transmit (void)
{

  SPI_Write_a(REG_DIO_MAPPING_1,	RFLR_DIOMAPPING1_DIO0_01); //0xC0 0x40  
  SPI_Write_a(REG_SYNC_WORD,0x12);	//0xB9 0x12
  SPI_Write_a(REG_OP_MODE,MODE_STDBY|0x80); // 0x81 0x81
      /* ------------- Stnadby Mode -----------------*/
    SPI_Write_a(REG_SYNC_WORD,0x12);	//0xB9 0x12
  SPI_Write_a(REG_FIFO_ADDR_PTR,0x00); //0x8D 0x80
  
  SPI_Write_a(REG_PAYLOAD_LENGTH, sizeof(str)/ sizeof(str[0]) );
  for ( uint8_t i = 0; i < (sizeof(str)/ sizeof(str[0])); i++ )
    SPI_Write_a(REG_FIFO, str[i]);
  SPI_Write_a(REG_PAYLOAD_LENGTH, sizeof(str)/ sizeof(str[0]));
  SPI_Write_a(REG_OP_MODE,MODE_TX|0x80); //0x81 0x83
  /*------------------Transmit--------*/
  while( SPI_Read_b(REG_IRQ_FLAGS) != 0x08)
  led_green_high();
  SPI_Write_a(REG_IRQ_FLAGS, 0x08); //0x92 -> 0x08
  HAL_Delay(300);
  led_green_low();     
  SPI_Write_a(REG_OP_MODE,MODE_SLEEP|0x80);	
  HAL_Delay(3000); 



}

uint8_t Lora_recieve(void)
{
      for (volatile uint8_t a = 0; a< 16; a ++)
    str_r[a] = 0;
  SPI_Write_a(REG_OP_MODE,MODE_STDBY|0x80); // 0x81 0x81
   SPI_Write_a(REG_DIO_MAPPING_1, RFLR_DIOMAPPING1_DIO0_00);
   SPI_Write_a(REG_SYNC_WORD,0x12);	
    /* ------------- Stnadby Mode -----------------*/
   SPI_Write_a(REG_FIFO_ADDR_PTR,0x00);
   SPI_Write_a(REG_OP_MODE, MODE_RX_CONTINUOUS|0x80);
   /*---------------Receive continuous ---------- */

       // while( SPI_Read_b(REG_IRQ_FLAGS) != 0x50)
          HAL_Delay(5000);
        if( (GPIOA->IDR & GPIO_IDR_IDR_15))
        {
          HAL_Delay(200);
       SPI_Write_a(REG_IRQ_FLAGS,0x50);
        SPI_Write_a(REG_SYNC_WORD,0x12);	
          led_redmain_high();
        HAL_Delay(300);
          led_redmain_low(); 
	SPI_Read_b(REG_RX_NB_BYTES);
	SPI_Read_b(REG_PKT_RSSI_VALUE);
	SPI_Read_b(REG_PKT_SNR_VALUE);
        
        for (uint8_t i = 0; i< SPI_Read_b(REG_RX_NB_BYTES); i++)
         str_r[i] = SPI_Read_b(REG_FIFO);	
	SPI_Write_a(REG_OP_MODE,MODE_STDBY|0x80);
        return str_r[0];
        }
        else 
          return (0x40);
                

        /*--------------Sleep Mode -------------------------*/
	//SPI_Write_a(REG_FIFO_ADDR_PTR,0x00);
	//SPI_Write_a(REG_OP_MODE,MODE_RX_CONTINUOUS|0x80);
        /*---------------Receive continuous ---------- */
}


void Lora_reset (void)
{
  
 // RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 
  GPIOA->MODER &=~ (3 << GPIO_MODER_MODER1_Pos);
  GPIOA->MODER |=  (1 << GPIO_MODER_MODER1_Pos); 
  Lora_reset_low();
  HAL_Delay(1);
  Lora_reset_high();
  HAL_Delay(10);  
  
}
