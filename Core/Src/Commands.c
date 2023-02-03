#include "Commands.h"
#include <string.h>
#include "SPI1.h"
#include "LORA SX1276.h"
#include "gpio.h"
#include "usart.h"
#define RX_BUFFER_SIZE 100
extern uint8_t str_uart[RX_BUFFER_SIZE];


bool Parser_Commands (void)
{
  uint8_t help[] = "/Help\n";
  uint8_t firmware_version[] = "/FW\n";
  uint8_t set_command[] = "/Set 0x";
  uint8_t read_command[] = "/Read 0x";
  uint8_t config_command[] = "/Config\n";
  if(strncmp((char*)str_uart, (char*)help, sizeof(help) - 1) == 0)
  {
    Lora_Show_List_of_Commands();
    return true;
  }
  else if (strncmp((char*)str_uart, (char*)firmware_version, sizeof(firmware_version) - 1 ) == 0)
  {
    Command_FW();
    return true;
  }    
  else if ((strncmp((char*)str_uart, (char*)set_command, sizeof(set_command) - 1)) == 0)
  {
    Command_Set();
    return true;
  }      
  else if ((strncmp((char*)str_uart, (char*)read_command, sizeof(read_command) - 1)) == 0)
  {
    Command_Read();
    return true;
  }
  else if ((strncmp((char*)str_uart, (char*)config_command, sizeof(config_command) - 1)) == 0)
  {
    Show_Config();
    return true;
  }
  else return false;
}

void Lora_Show_List_of_Commands (void)
{  
  uint8_t list_of_commands[] = "\r\n--------------------\r\nLIST OF COMMANDS\r\n";
  uint8_t set_data_to_reg[] = "/Set - Set in reg(0xAB) data (0x55). Example:/Set 0xAB 0x55\r\n";
  uint8_t read_data_from_reg[] = "/Read - Read from reg(0xAB) data (0x55). Example:/Read 0xAB 0x55\r\n";
  uint8_t lora_config_data[] = "/Config - show current lora config\r\n";
  uint8_t get_FW[] = "/FW - Show firmware version\r\n";
  uint8_t who[] = "/Who - to get list  ID available lora online \r\n";
  uint8_t send_target_message[] = "Msg ID text - send message target ID lora. Example: msg 0x18 Hello!\r\n";
  uint8_t send_all_lora_message[] = "Msg 0 text - send message all available lora. Example: msg 0x00 Hello!\r\n";
  HAL_UART_Transmit(&huart2, list_of_commands, sizeof(list_of_commands), 100);
  HAL_UART_Transmit(&huart2, set_data_to_reg, sizeof(set_data_to_reg), 100);
  HAL_UART_Transmit(&huart2, read_data_from_reg, sizeof(read_data_from_reg), 100);
  HAL_UART_Transmit(&huart2, lora_config_data, sizeof(lora_config_data), 100);
  HAL_UART_Transmit(&huart2, get_FW, sizeof(get_FW), 100);
  HAL_UART_Transmit(&huart2, who, sizeof(who), 100);
  HAL_UART_Transmit(&huart2, send_target_message, sizeof(send_target_message), 100);
  HAL_UART_Transmit(&huart2, send_all_lora_message, sizeof(send_all_lora_message), 100);
  Uart_Data_Clear();
}
void Command_FW(void)
{
    Lora_Show_Firmware_Version();  
    Uart_Data_Clear();
}

void Command_Set(void)
{
    uint8_t reg = 0;
    uint8_t data = 0;
    if ((ASCII_to_hex(&str_uart[7], &str_uart[8], &reg)) & (ASCII_to_hex(&str_uart[12], &str_uart[13], &data)))
    {
        if (reg == REG_OP_MODE)
            SPI1_Write(reg, data);
        else  
        {
            uint8_t temp_reg = SPI1_Read(REG_OP_MODE);
            SPI1_Write(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
            SPI1_Write(reg, data);
            SPI1_Write(REG_OP_MODE, temp_reg);
        }
     uint8_t result_write[] = "\r\nIn reg 0xXX has been written 0xXX";
     result_write[11] = str_uart[7];
     result_write[12] = str_uart[8];
     result_write[33] = str_uart[12];
     result_write[34] = str_uart[13];
     HAL_UART_Transmit(&huart2, result_write, sizeof(result_write), 100);  
     Uart_Data_Clear();
    }
}

void Command_Read(void)
{
    uint8_t reg = 0;
    uint8_t data = 0;
    if (ASCII_to_hex(&str_uart[8], &str_uart[9], &reg))
    {
        data = SPI1_Read(reg);
        uint8_t result_read[] = "\r\nResult from reg 0xXX is 0xXX";
        result_read[20] = str_uart[8];
        result_read[21] = str_uart[9];
        Hex_to_ASCII(&data, &result_read[28], &result_read[29]);
        HAL_UART_Transmit(&huart2, result_read, sizeof(result_read), 100);  
        Uart_Data_Clear();
    }
}

void Show_Config (void)
{
  uint8_t result_data[] = "\r\nData from reg 0xXX is 0xXX";
  for (uint8_t reg = 0; reg <= 0x3F; reg++)
  {
    uint8_t data = SPI1_Read(reg);
    Hex_to_ASCII(&reg, &result_data[18], &result_data[19]);
    Hex_to_ASCII(&data, &result_data[26], &result_data[27]);
    HAL_UART_Transmit(&huart2, result_data, sizeof(result_data), 100);      
  }
    Uart_Data_Clear();
}
void Uart_Data_Clear (void)
{
    for (volatile uint8_t a = 0; a< RX_BUFFER_SIZE; a ++)
        *(str_uart + a) = 0;
}

bool ASCII_to_hex (uint8_t* first_symbol, uint8_t* second_symbol, uint8_t* hexdata)
{
    *hexdata = 0;
    if(*first_symbol >= '0' & *first_symbol <= '9') 
        *hexdata = (*first_symbol - '0') << 4;
    else if (*first_symbol >= 'A' & *first_symbol <= 'F')
    {
        *hexdata = (((*first_symbol - 'A')<<4) + 0xA0);      
    }
    else  
    {
      Error();
      return false;
    }   
    if(*second_symbol >= '0' & *second_symbol <= '9') 
        *hexdata |= (*second_symbol - '0');
    else if (*second_symbol >= 'A' & *second_symbol <= 'F')
        *hexdata |= (*second_symbol - 'A' + 0x0A);
    else  
    {
      Error();
      return false;
    }
    return true;
}

void Hex_to_ASCII(uint8_t* hex, uint8_t* first_symbol, uint8_t* second_symbol)
{
  *first_symbol = *hex >> 4;
  uint8_t zero = 0x00;
  if(*first_symbol >= zero & *first_symbol <=0x09)
    *first_symbol+= '0';
  if(*first_symbol >= 0x0A & *first_symbol <=0x0F)
    *first_symbol =  *first_symbol - 0x0A + 'A';
  *second_symbol = 0x0F & (*hex);
  if(*second_symbol >= zero & *second_symbol <=0x09)
    *second_symbol+= '0';
  if(*second_symbol >= 0x0A & *second_symbol <=0x0F)
    *second_symbol =  *second_symbol - 0x0A + 'A';
}

void Error (void)
{
  uint8_t error[] = "\r\nError!!!\r\n";
  HAL_UART_Transmit(&huart2, error, sizeof(error), 100);
}