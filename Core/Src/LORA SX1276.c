#include "main.h"
#include "LORA SX1276.h"
#include "SPI1.h"
#include <stdbool.h>
#define RX_BUFFER_SIZE 100
//#define RECIEVER
//#define TRANSMITTER
/* Lora Config
* (1) - Reset
* (2) - Sleep mode 
*
*/
uint8_t str[] = "Hello!";
uint8_t RSSI[] = "RSSI = -";
//uint8_t str_r[16] = {0};

void Lora_init (void)
{  
   Lora_reset();

   if (SPI1_Read(REG_VERSION) == 0x12)
   {
     led_blue_high();       
    Delay_a();
     led_blue_low(); 
   }
   else 
   {
     while(1)
     {

        led_red_high();
        HAL_Delay(200);
        led_red_low();
        HAL_Delay(200);
     }

   }
   //--------------------------------LORA CONFIGURATION------------------------
   // 868MHz, SF12, 125kHz, 300bps, MaxPower, OcpOn, 9Byte info 	
   SPI1_Write(REG_OP_MODE, MODE_LONG_RANGE_MODE|MODE_SLEEP); //Lora mode, HF, Sleep //0x81 0x80
   /*--------------Sleep Mode -------------------------*/
   SPI1_Write(REG_FIFO_TX_BASE_ADDR, 0x00); 
   SPI1_Write(REG_FIFO_RX_BASE_ADDR, 0x00); 
   SPI1_Write(REG_LNA, 0x23); //G1 = maximum gain, Boost om, 150% LNA current
   SPI1_Write(REG_PA_DAC, 0x84); //Default value PA_HF/LF or +17dBm
   SPI1_Write(REG_OCP, 0x2b); //OCP enabled, OcpTrim = 100 mA
   SPI1_Write(REG_PA_CONFIG, 0xFF); //PA_BOOST, Pout = 17 dBm 
   SPI1_Write(REG_FRF_MSB, 0xD9); //0x86 0xD9 868 MHz
   SPI1_Write(REG_FRF_MID, 0x00); //0x87 0x00
   SPI1_Write(REG_FRF_LSB, 0x00); //0x88 0x00
   SPI1_Write(REG_OP_MODE, MODE_STDBY|0x80); //new
   /* ------------- Standby Mode -----------------*/
   SPI1_Write(REG_PA_DAC, 0x87); //Set Pmax to +20dBm for PA_HP
   SPI1_Write(REG_OCP, 0x3b); //OCP enabled,  OcpTrim = 27 Imax = 240 mA
   SPI1_Write(REG_PA_CONFIG, 0x8F); //PA_BOOST, Pout = 17 dBm 
   SPI1_Write(REG_MODEM_CONFIG_1, 0x98); //500 kHz BW, Coding Rate 4/8, Explicit Header Mode
   SPI1_Write(REG_MODEM_CONFIG_2, 0xC0);  // SF12 , Normal Mode single packet, no CRC
   SPI1_Write(REG_MODEM_CONFIG_3, 0x04); //LNA gain set by the internal AGC loop
   SPI1_Write(REG_DIO_MAPPING_1, RFLR_DIOMAPPING1_DIO0_00); //RX READY
   SPI1_Write(REG_SYNC_WORD,0x12);	
   SPI1_Write(REG_OP_MODE, MODE_STDBY|0x80);
    /* ------------- Stnadby Mode -----------------*/
   SPI1_Write(REG_FIFO_ADDR_PTR,0x00);
   
   SPI1_Write(REG_OP_MODE, MODE_RX_CONTINUOUS|0x80);	 
   /*---------------Receive continuous ---------- */

}


void Lora_transmit (uint8_t *strdata, uint8_t number_of_data)
{
    
 // SPI_Write_a(REG_DIO_MAPPING_1, RFLR_DIOMAPPING1_DIO0_01); //0xC0 0x40  TX COMPLETE
  SPI1_Write(REG_OP_MODE,MODE_STDBY|0x80); // 0x81 0x81
      /* ------------- Standby Mode Mode -----------------*/
 // SPI1_Write(REG_MODEM_CONFIG_2, 0xC4);  // SF12 , Normal Mode single packet, CRC
  SPI1_Write(REG_SYNC_WORD,0x12);	//0xB9 0x12
  SPI1_Write(REG_FIFO_ADDR_PTR,0x00); //0x8D 0x80 
  SPI1_Write(REG_PAYLOAD_LENGTH, number_of_data );
  for ( uint8_t i = 0; i < number_of_data; i++ )
    SPI1_Write(REG_FIFO, *(strdata+i));
  SPI1_Write(REG_PAYLOAD_LENGTH, number_of_data);
  SPI1_Write(REG_OP_MODE,MODE_TX|0x80); // MODE TX
  /*------------------Transmit--------*/
 // HAL_Delay(3500);
  while( SPI1_Read(REG_IRQ_FLAGS) == !0x08)  
  led_green_high();
  SPI1_Write(REG_IRQ_FLAGS, 0x08); //0x92 -> 0x08
  HAL_Delay(300);
  led_green_low();     
  SPI1_Write(REG_OP_MODE,MODE_STDBY|0x80); // 0x81 0x81
  SPI1_Write(REG_FIFO_ADDR_PTR,0x00);
  SPI1_Write(REG_OP_MODE, MODE_RX_CONTINUOUS|0x80);       
}

void Lora_recieve(uint8_t *str_r, uint8_t *num_of_bytes)
{
      for (volatile uint8_t a = 0; a< RX_BUFFER_SIZE; a ++)
    *(str_r + a) = 0;
  SPI1_Write(REG_OP_MODE,MODE_STDBY|0x80); // 0x81 0x81
   /* ------------- Standby Mode -----------------*/
 //  SPI1_Write(REG_MODEM_CONFIG_2, 0xC0);  // SF12 , Normal Mode single packet, no CRC
   SPI1_Write(REG_DIO_MAPPING_1, RFLR_DIOMAPPING1_DIO0_00); // RX READY
   SPI1_Write(REG_SYNC_WORD,0x12);	
   SPI1_Write(REG_FIFO_ADDR_PTR,0x00);
   SPI1_Write(REG_OP_MODE, MODE_RX_CONTINUOUS|0x80);
   /*---------------Receive continuous ---------- */

      //  while( SPI1_Read(REG_IRQ_FLAGS) != 0x50)
         // HAL_Delay(200);
       // if( (GPIOA->IDR & GPIO_IDR_IDR_15))
       // {
    HAL_Delay(200);
    SPI1_Read(REG_IRQ_FLAGS);
    SPI1_Write(REG_IRQ_FLAGS,0x50);
    SPI1_Write(REG_SYNC_WORD,0x12);	
    led_redmain_high();
    HAL_Delay(300);
    led_redmain_low(); 
    *num_of_bytes =  SPI1_Read(REG_RX_NB_BYTES);
    SPI1_Read(REG_PKT_RSSI_VALUE);
    SPI1_Read(REG_PKT_SNR_VALUE);       
    for (uint8_t i = 0; i< SPI1_Read(REG_RX_NB_BYTES); i++)
    *(str_r+i) = SPI1_Read(REG_FIFO);
    for (uint8_t r = 0; r < sizeof(RSSI); r++)
    {
      *(str_r+ *num_of_bytes + r) = *(RSSI+r);
    }
    uint8_t rssi_tmp = 157 - SPI1_Read(REG_PKT_RSSI_VALUE);
    uint8_t rssi_tmp_first = (rssi_tmp / 100) + 0x30;
    uint8_t rssi_tmp_sec = (rssi_tmp - 100*(rssi_tmp_first- 0x30));
    rssi_tmp_sec = (rssi_tmp_sec/10) + 0x30;
    uint8_t rssi_tmp_thi = ((rssi_tmp - 100*(rssi_tmp_first- 0x30) - 10*(rssi_tmp_sec - 0x30))+ 0x30);
    
    *(str_r+ *num_of_bytes + sizeof(RSSI) - 1) = rssi_tmp_first;
    *(str_r+ *num_of_bytes + sizeof(RSSI)) = rssi_tmp_sec;
    *(str_r+ *num_of_bytes + sizeof(RSSI) + 1) = rssi_tmp_thi;
    *(str_r+ *num_of_bytes + sizeof(RSSI) + 2) = '\r';
    *(str_r+ *num_of_bytes + sizeof(RSSI) + 3 ) = '\n';
    SPI1_Write(REG_OP_MODE,MODE_STDBY|0x80); // 0x81 0x81
    SPI1_Write(REG_FIFO_ADDR_PTR,0x00);
    SPI1_Write(REG_OP_MODE, MODE_RX_CONTINUOUS|0x80);         

}


void Lora_reset (void)
{
  
  GPIOA->MODER &=~ (3 << GPIO_MODER_MODER1_Pos);
  GPIOA->MODER |=  (1 << GPIO_MODER_MODER1_Pos); 
  Lora_reset_low();
  HAL_Delay(1);
  Lora_reset_high();
  HAL_Delay(10);  
  
}
