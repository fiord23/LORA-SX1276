#include "Commands.h"
#include <string.h>
#include "SPI1.h"
#include "LORA SX1276.h"
#include "gpio.h"
#include "usart.h"
#define RX_BUFFER_SIZE 100
extern uint8_t str_uart[RX_BUFFER_SIZE];
uint8_t reg = 0;
bool Lora_Show_List_of_Commands (void)
{  
  uint8_t help_compare[] = "/help\n"; 
  uint8_t list_of_commands[] = "\r\n--------------------\r\nLIST OF COMMANDS\r\n";
  uint8_t set_data_to_reg[] = "/Set - Set in reg(0xAB) data (0x55). Example:/Set 0xAB 0x55\r\n";
  uint8_t read_data_from_reg[] = "/Read - Read from reg(0xAB) data (0x55). Example:/Read 0xAB 0x55\r\n";
  uint8_t lora_config_data[] = "/Config - show current lora config\r\n";
  uint8_t get_FW[] = "/FW - Show firmware version\r\n";
  uint8_t who[] = "/Who - to get list  ID available lora online \r\n";
  uint8_t send_target_message[] = "Msg ID text - send message target ID lora. Example: msg 0x18 Hello!\r\n";
  uint8_t send_all_lora_message[] = "Msg 0 text - send message all available lora. Example: msg 0x00 Hello!\r\n";
  if (!strcmp(str_uart, help_compare))
    {
      HAL_UART_Transmit(&huart2, list_of_commands, sizeof(list_of_commands), 100);
      HAL_UART_Transmit(&huart2, set_data_to_reg, sizeof(set_data_to_reg), 100);
      HAL_UART_Transmit(&huart2, read_data_from_reg, sizeof(read_data_from_reg), 100);
      HAL_UART_Transmit(&huart2, lora_config_data, sizeof(lora_config_data), 100);
      HAL_UART_Transmit(&huart2, get_FW, sizeof(get_FW), 100);
      HAL_UART_Transmit(&huart2, who, sizeof(who), 100);
      HAL_UART_Transmit(&huart2, send_target_message, sizeof(send_target_message), 100);
      HAL_UART_Transmit(&huart2, send_all_lora_message, sizeof(send_all_lora_message), 100);
      Uart_Data_Clear();
      return true;
    }
    else 
      return false; 
}

bool Parser_Commands (void)
{
  uint8_t firmware_version[] = "/FW\n";
  if (!strcmp(str_uart, firmware_version))
  {
    Lora_Show_Firmware_Version();  
    Uart_Data_Clear();
    return true;
  }  
  uint8_t set_command[] = "/Set 0x";
  if (!strncmp(str_uart, set_command, sizeof(set_command) - 1))
  {
    //uint8_t reg = 0;
    uint8_t error[] = "\r\nError!!!\r\n";
    if(str_uart[7] >= '0' & str_uart[7] <= '9') 
        reg = (str_uart[7] - '0') << 4;
    else if (str_uart[7] >= 'A' & str_uart[7] <= 'B')
        reg = (str_uart[7] - 'A') << 4;
    else  
    {
      HAL_UART_Transmit(&huart2, error, sizeof(error), 100);
      return false;
    }
    
    if(str_uart[8] >= '0' & str_uart[8] <= '9') 
        reg |= (str_uart[8] - '0');
    else if (str_uart[8] >= 'A' & str_uart[8] <= 'B')
        reg |= (str_uart[8] - 'A');
    else  
    {
      HAL_UART_Transmit(&huart2, error, sizeof(error), 100);
      return false;
    }
    
    HAL_UART_Transmit(&huart2, str_uart+7, 2, 100);
    //uint8_t reg = (str_uart[7] - 0x30)*10 + str_uart[8] - 0x30;
    Uart_Data_Clear();
    return true;
  }
  
    return false;
}
void Uart_Data_Clear (void)
{
      for (volatile uint8_t a = 0; a< RX_BUFFER_SIZE; a ++)
        *(str_uart + a) = 0;
}