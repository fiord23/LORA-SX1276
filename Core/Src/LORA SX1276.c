
#include "SPI1.h"
#include "LORA SX1276.h"
#include "gpio.h"
#include "usart.h"

#define RX_BUFFER_SIZE 100
uint8_t RSSI_value = 0;
uint8_t SNR_value = 0;


extern UART_HandleTypeDef huart2;
extern uint8_t str_uart[RX_BUFFER_SIZE];
extern bool flag_uart_receiver;
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
   SPI1_Write(REG_OP_MODE, MODE_LONG_RANGE_MODE|MODE_SLEEP); 
   /*--------------Sleep Mode -------------------------*/
   SPI1_Write(REG_FIFO_TX_BASE_ADDR, 0x00); 
   SPI1_Write(REG_FIFO_RX_BASE_ADDR, 0x00); 
   SPI1_Write(REG_LNA, LNA_GAIN_G1 | LNA_BOOST_LF | BOOST_ON_150_LNA_CURRENT);
   SPI1_Write(REG_PA_DAC, PA_HF_LF_OR_17_dBm);
   SPI1_Write(REG_OCP, OCP_ENABLED | OCP_240mA);
   SPI1_Write(REG_PA_CONFIG, PA_BOOST_TO_20dBm | POWER_15dBm | POUT_PA_17dBm); 
   SPI1_Write(REG_FRF_MSB, FREQUENCY_868_MSB);
   SPI1_Write(REG_FRF_MID, FREQUENCY_868_MID);
   SPI1_Write(REG_FRF_LSB, FREQUENCY_868_LSB);
   SPI1_Write(REG_OP_MODE, MODE_STDBY|MODE_LONG_RANGE_MODE); 
   /* ------------- Standby Mode -----------------*/
   SPI1_Write(REG_PA_DAC, PA_HIGH_POWER);
   SPI1_Write(REG_OCP, OCP_ENABLED | OCP_240mA);
   SPI1_Write(REG_PA_CONFIG, PA_BOOST_TO_20dBm | POUT_PA_17dBm);
   SPI1_Write(REG_MODEM_CONFIG_1, BANDWIDTH_250_kHz | CODING_RATE_4_8 | EXPLICIT_HEADER_MODE);
   SPI1_Write(REG_MODEM_CONFIG_2, SPREADING_FACTOR_12 | TX_NORMAL_SINGLE_MODE | CRC_DISABLE); 
   SPI1_Write(REG_MODEM_CONFIG_3, GAIN_INTERANL_AGC_LOOP); 
   SPI1_Write(REG_DIO_MAPPING_1, RFLR_DIOMAPPING1_RX_READY);
   SPI1_Write(REG_SYNC_WORD,0x12);	
   SPI1_Write(REG_OP_MODE, MODE_STDBY|MODE_LONG_RANGE_MODE);
    /* ------------- Stnadby Mode -----------------*/
   SPI1_Write(REG_FIFO_ADDR_PTR,0x00);   
   SPI1_Write(REG_OP_MODE, MODE_RX_CONTINUOUS|MODE_LONG_RANGE_MODE);	 
   /*---------------Receive continuous ---------- */
}


void Lora_transmit (uint8_t *strdata, uint8_t number_of_data)
{
    
   SPI1_Write(REG_OP_MODE,MODE_STDBY|MODE_LONG_RANGE_MODE); // 0x81 0x81
      /* ------------- Standby Mode Mode -----------------*/
 // SPI1_Write(REG_MODEM_CONFIG_2, 0xC4);  // SF12 , Normal Mode single packet, CRC
   SPI1_Write(REG_SYNC_WORD,0x12);
   SPI1_Write(REG_FIFO_ADDR_PTR,0x00); 
   SPI1_Write(REG_PAYLOAD_LENGTH, number_of_data );
   for ( uint8_t i = 0; i < number_of_data; i++ )
     SPI1_Write(REG_FIFO, *(strdata+i));
   SPI1_Write(REG_PAYLOAD_LENGTH, number_of_data);
   SPI1_Write(REG_OP_MODE,MODE_TX|MODE_LONG_RANGE_MODE); // MODE TX
  /*------------------Transmit--------*/
  HAL_Delay(300);
   while( SPI1_Read(REG_IRQ_FLAGS) != IRQ_TX_DONE_MASK)
   {
     led_green_high();
   }       
   SPI1_Write(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
   HAL_Delay(300);
   led_green_low();     
   SPI1_Write(REG_OP_MODE,MODE_STDBY|MODE_LONG_RANGE_MODE);
   SPI1_Write(REG_FIFO_ADDR_PTR,0x00);
   SPI1_Write(REG_OP_MODE, MODE_RX_CONTINUOUS|MODE_LONG_RANGE_MODE);
   SPI1_Write(REG_FIFO_ADDR_PTR,0x00);
}

void Lora_recieve(uint8_t *str_r, uint8_t *num_of_bytes)
{
   for (volatile uint8_t a = 0; a< RX_BUFFER_SIZE; a ++)
    *(str_r + a) = 0;
   SPI1_Write(REG_OP_MODE,MODE_STDBY|MODE_LONG_RANGE_MODE); 
   /* ------------- Standby Mode -----------------*/
   SPI1_Write(REG_SYNC_WORD,0x12);	
   SPI1_Write(REG_FIFO_ADDR_PTR,0x00);
   SPI1_Write(REG_OP_MODE, MODE_RX_CONTINUOUS|MODE_LONG_RANGE_MODE);
   /*---------------Receive continuous ---------- */
    HAL_Delay(200);
    SPI1_Read(REG_IRQ_FLAGS);
    SPI1_Write(REG_IRQ_FLAGS,0x50);
    SPI1_Write(REG_SYNC_WORD,0x12);	
    led_redmain_high();
    HAL_Delay(300);
    led_redmain_low(); 
    *num_of_bytes =  SPI1_Read(REG_RX_NB_BYTES);
    SPI1_Read(REG_PKT_SNR_VALUE);       
    for (uint8_t i = 0; i< SPI1_Read(REG_RX_NB_BYTES); i++)
        *(str_r+i) = SPI1_Read(REG_FIFO);
    RSSI_value =  157 - SPI1_Read(REG_PKT_RSSI_VALUE);
    SNR_value =  SPI1_Read(REG_PKT_SNR_VALUE) >> 2;
    SPI1_Write(REG_OP_MODE,MODE_STDBY|MODE_LONG_RANGE_MODE);
    SPI1_Write(REG_FIFO_ADDR_PTR,0x00);
    SPI1_Write(REG_OP_MODE, MODE_RX_CONTINUOUS|MODE_LONG_RANGE_MODE);
    SPI1_Write(REG_DIO_MAPPING_1, RFLR_DIOMAPPING1_RX_READY);
    SPI1_Write(REG_FIFO_ADDR_PTR,0x00);
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

void Show_RSSI (void)
{
    uint8_t RSSI[] = "RSSI = -XXXdBm\r\n";
    *(RSSI + 8) = (RSSI_value/100) + 0x30;
    *(RSSI + 9) = (RSSI_value - 100*(*(RSSI + 8) - 0x30));
    *(RSSI + 9) = (*(RSSI + 9)/10) + 0x30;
    *(RSSI + 10) = ((RSSI_value - 100*(*(RSSI + 8) - 0x30) - 10*(*(RSSI + 9) - 0x30))+ 0x30);
    HAL_UART_Transmit(&huart2, RSSI, sizeof(RSSI), 100);
}

void Show_SNR (void)
{
    uint8_t SNR[] = "SNR = XXXdB\r\n";
    *(SNR + 6) = (SNR_value/100) + 0x30;
    *(SNR + 7) = (SNR_value - 100*(*(SNR + 6) - 0x30));
    *(SNR + 7) = (*(SNR + 7)/10) + 0x30;
    *(SNR + 8) = ((SNR_value - 100*(*(SNR + 6) - 0x30) - 10*(*(SNR + 7) - 0x30))+ 0x30);
    HAL_UART_Transmit(&huart2, SNR, sizeof(SNR), 100);
}

void Lora_Show_Firmware_Version (void)
{
  uint8_t str_FW_Config[] = "Firmware ver. 0.1.1 (27-01-2023) by D.Zaretski\r\n";
  HAL_UART_Transmit(&huart2, str_FW_Config, sizeof(str_FW_Config), 100);
}

void Lora_Show_Help (void)
{
  uint8_t help[] = "Write help for list of commands \r\n";
  HAL_UART_Transmit(&huart2, help, sizeof(help), 100);
}

bool Lora_Show_List_of_Commands (void)
{  
  uint8_t help_compare[] = "help\r\n"; 
  uint8_t list_of_commands[] = "LIST OF COMMANDS\r\n";
  uint8_t compare = 0;
  for(uint8_t counter = 0; counter <=sizeof(help_compare); counter++)
  {
    if(*(str_uart+counter) == *(help_compare+counter))
      compare++;
  }
    if(compare == sizeof(help_compare))
    {
      HAL_UART_Transmit(&huart2, list_of_commands, sizeof(list_of_commands), 100);
      return true;
    }
    else 
      return false;
  
}
