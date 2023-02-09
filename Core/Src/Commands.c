#include "Commands.h"
#include <string.h>
#include <ctype.h>
#include "SPI1.h"
#include "LORA SX1276.h"
#include "gpio.h"
#include "usart.h"
#define RX_BUFFER_SIZE 100
extern uint8_t str_uart[RX_BUFFER_SIZE];

// CRC8 Table
uint8_t CRC8Table[256]={0};
void CRC8_init (uint8_t CRC8_Polynom) {
  uint8_t x,y,crc;
  x = 0;
  do 
  {
    crc = x;
    for (y = 0; y < 8; y++) 
    {
      crc = (crc & (1<<7)) ? ((crc << 1) ^ CRC8_Polynom) : (crc << 1);
    }
    CRC8Table[x] = crc;
    x++;
  } while (x > 0);
}    

bool Parser_Commands (void)
{
  uint8_t help[] = "/Help\n";
  uint8_t firmware_version[] = "/FW\n";
  uint8_t set_command[] = "/Set 0x";
  uint8_t read_command[] = "/Read 0x";
  uint8_t config_command[] = "/Config\n";
  if(strncmp((char*)str_uart, (char*)help, sizeof(help) - 1) == 0)
  {
    flash_ok = HAL_FLASHEx_DATAEEPROM_Unlock();
  }
  flash_ok = HAL_ERROR;
  while (flash_ok != HAL_OK)
  {
    flash_ok = HAL_FLASHEx_DATAEEPROM_Erase (FLASH_TYPEERASEDATA_WORD, address);
  }
  flash_ok = HAL_ERROR;
  while (flash_ok != HAL_OK)
  {
    flash_ok = HAL_FLASHEx_DATAEEPROM_Program (FLASH_TYPEPROGRAMDATA_WORD, address, value);
  }
  flash_ok = HAL_ERROR;
  while (flash_ok != HAL_OK)
  {
    flash_ok = HAL_FLASHEx_DATAEEPROM_Lock ();
  }
  else return false;
}



bool Parser_Commands (void)
{
  char help[] = "/HELP";
  char firmware_version[] = "/FW";
  char auto_command[] = "/AUTO";
  char set_loop[] = "/LOOP ";  
  char set_command[] = "/SET 0X";
  char read_command[] = "/READ 0X";
  char config_command[] = "/CONFIG";
  char resetlora_command[] = "/RESETLORA";
  char resetstm_command[] = "/RESETSTM";  
  char msg_comerror[]="\r\nUnkonw command\r\n";
  char command[20];

  setendstr(str_uart);
  strncpy(command, (char*)str_uart,20);
  toUpper(command);

  // Get command
   if ((strlen((char*)str_uart)>2) &&(str_uart[0]=='/')) {
      if(strncmp(command, help, sizeof(help)) == 0)
      {
        Lora_Show_List_of_Commands();
        return true;
      }
      else if (strncmp(command, firmware_version, sizeof(firmware_version)-1) == 0)
      {
        Command_FW();
        return true;
      }    
      else if (strncmp(command, auto_command, sizeof(auto_command)-1) == 0)
      {
        Command_auto();
        return true;
      }  
      else if ((strncmp(command, set_loop, sizeof(set_loop)-1 )) == 0)
      {
        Command_Setloop(command);
        return true;
      }
      else if ((strncmp(command, set_command, sizeof(set_command)-1 )) == 0)
      {
        Command_Set(command);
        return true;
      }
      else if ((strncmp(command, read_command, sizeof(read_command)-1 )) == 0)
      {
        Command_Read(command);
        return true;
      }
      else if ((strncmp(command, config_command, sizeof(config_command)-1)) == 0)
      {
        Show_Config();
        return true;
      }
      else if ((strncmp(command, resetstm_command, sizeof(resetstm_command)-1)) == 0)
      {
        HAL_NVIC_SystemReset();
        return true;
      }
      else if ((strncmp(command, resetlora_command, sizeof(resetlora_command)-1)) == 0)
      {
        Lora_reset();
        return true;
      }
      else {
        HAL_UART_Transmit(&huart2, (uint8_t*)msg_comerror, sizeof(msg_comerror)-1, 30);
        return false;
      }
   }
   return false;
}

void Lora_Show_List_of_Commands (void)
{  
  uint8_t list_of_commands[] = "\r\n--------------------\r\nLIST OF COMMANDS\r\n \
/auto - autosend code \r\n \
/loop X - set loop to autosend x=1..9\r\n \
/Set - Set in reg(0xAB) data (0x55). Example:/Set 0xAB 0x55\r\n \
/Read - Read from reg(0xAB) data (0x55). Example:/Read 0xAB 0x55\r\n \
/Config - show current lora config\r\n \
/ResetSTMr\n \
/ResetloRa - reset LoRa\r\n \
/FW - Show firmware version\r\n \
/Who - to get list  ID available lora online \r\n \
Msg ID text - send message target ID lora. Example: msg 0x18 Hello!\r\n \
Msg 0 text - send message all available lora. Example: msg 0x00 Hello!\r\n";
  HAL_UART_Transmit(&huart2, list_of_commands, sizeof(list_of_commands)-1, 100);
  Uart_Data_Clear();
}

void Command_FW(void)
{
  Lora_Show_Firmware_Version();
  uint8_t color[] = "Show light RGBM, Red, Green, Blue, Mainred.\r\n";
  HAL_UART_Transmit(&huart2, color, sizeof(color)-1, 100);  
  led_red_high();
  led_green_high();
  led_blue_high();
  led_redmain_high();
  HAL_Delay(500);

  led_green_low();
  led_blue_low();
  led_redmain_low();
  HAL_Delay(100);

  led_red_low();  
  led_green_high();
  HAL_Delay(100);

  led_green_low(); 
  led_blue_high();
  HAL_Delay(100);

  led_blue_low(); 
  led_redmain_high();
  HAL_Delay(100);
  led_blue_low();
  led_red_low();
  led_green_low();        
  led_redmain_low();
        
  Uart_Data_Clear();
}  

void Command_auto(void)
{
     auto_send=true;
}

void Command_Set(void)
{
    uint8_t reg = 0;
    uint8_t data = 0;
    uint8_t bit_hi = (uint8_t)command[7];
    uint8_t bit_lo = (uint8_t)command[8];
    uint8_t val_hi = (uint8_t)command[12];
    uint8_t val_lo = (uint8_t)command[13];    
    if ((ASCII_to_hex(&bit_hi,  &bit_lo, &reg)) & (ASCII_to_hex(&val_hi, &val_lo, &data)))
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
     HAL_UART_Transmit(&huart2, result_write, sizeof(result_write)-1, 100);  
     Uart_Data_Clear();
    }
}

void Command_Read(char * command)
{
    uint8_t reg = 0;
    uint8_t data = 0;
    uint8_t bit_hi = (uint8_t)command[8];
    uint8_t bit_lo = (uint8_t)command[9];
    if (ASCII_to_hex(&bit_hi,  &bit_lo, &reg))
    {
        data = SPI1_Read(reg);
        uint8_t result_read[] = "\r\nResult from reg 0xXX is 0xXX";
        result_read[20] = str_uart[8];
        result_read[21] = str_uart[9];
        Hex_to_ASCII(&data, &result_read[28], &result_read[29]);
        HAL_UART_Transmit(&huart2, result_read, sizeof(result_read)-1, 100);  
        Uart_Data_Clear();
    }
}

void Show_Config (void)
{
  HAL_UART_Transmit(&huart2, "{", 1, 30); 
  uint8_t json_data[] = "\"0x00\":\"0x00\"";
  uint8_t str[] = "\"0x00\":\"0x00\"";
  bool firstch=true;
  
  CRC8_init(0x31);
  uint8_t crc = 0xFF;
  for (uint8_t reg = 0; reg <= 0x3F; reg++)
  {
    if (firstch)
      firstch=false;
    else
      HAL_UART_Transmit(&huart2, ",", 1, 30);       
    uint8_t data = SPI1_Read(reg);
    crc = CRC8Table[crc ^ reg];
    crc = CRC8Table[crc ^ data];
    Hex_to_ASCII(&reg, &json_data[3], &json_data[4]);
    Hex_to_ASCII(&data, &json_data[10], &json_data[11]);
    HAL_UART_Transmit(&huart2, json_data, sizeof(json_data)-1, 30);   
  }

  //show CRC code
  json_data[0]=',';
  json_data[1]='"';
  json_data[2]='C';
  json_data[3]='R';
  json_data[4]='C';
  uint8_t crc2;
  dec2hex(crc,&json_data[10]);
  hex2dec(&json_data[10],&crc2);
  uint8_t x=dec2str(crc,str, 3);  
  HAL_UART_Transmit(&huart2, json_data, sizeof(json_data)-1, 30);   
  HAL_UART_Transmit(&huart2, "}\r\n", 3, 30); 
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
  if(*first_symbol <=0x09)
    *first_symbol+= '0';
  if(*first_symbol >= 0x0A & *first_symbol <=0x0F)
    *first_symbol =  *first_symbol - 0x0A + 'A';
  *second_symbol = 0x0F & (*hex);
  if(*second_symbol <=0x09)
    *second_symbol+= '0';
  if(*second_symbol >= 0x0A & *second_symbol <=0x0F)
    *second_symbol =  *second_symbol - 0x0A + 'A';
}

void Error (void)
{
  uint8_t error[] = "\r\nAscii 2 Hex Error!!!\r\n";
  HAL_UART_Transmit(&huart2, error, sizeof(error)-1, 100);
}



void Show_RSSI (void)
{
    uint8_t RSSI[] = "RSSI = -XXXdBm\r\n";
    *(RSSI + 8) = (RSSI_value/100) + 0x30;
    *(RSSI + 9) = (RSSI_value - 100*(*(RSSI + 8) - 0x30));
    *(RSSI + 9) = (*(RSSI + 9)/10) + 0x30;
    *(RSSI + 10) = ((RSSI_value - 100*(*(RSSI + 8) - 0x30) - 10*(*(RSSI + 9) - 0x30))+ 0x30);
    HAL_UART_Transmit(&huart2, RSSI, sizeof(RSSI)-1, 30);
}

void Show_SNR (void)
{
    uint8_t SNR[] = "SNR = XXXdB\r\n";
    *(SNR + 6) = (SNR_value/100) + 0x30;
    *(SNR + 7) = (SNR_value - 100*(*(SNR + 6) - 0x30));
    *(SNR + 7) = (*(SNR + 7)/10) + 0x30;
    *(SNR + 8) = ((SNR_value - 100*(*(SNR + 6) - 0x30) - 10*(*(SNR + 7) - 0x30))+ 0x30);
    HAL_UART_Transmit(&huart2, SNR, sizeof(SNR)-1, 30);
}

void Lora_Show_Firmware_Version (void)
{
  uint8_t str_FW_Config[] = FW;
  // read|set node N
  if (nodeN==0) {
    uint32_t x = readFromEEPROM(0x08080000);
    if (x==0)
      writeToEEPROM (0x08080000, 105);
    nodeN=x&0xFF;
  }
  HAL_UART_Transmit(&huart2, str_FW_Config, sizeof(str_FW_Config)-1, 30);
}

void Lora_Show_Help (void)
{
  uint8_t help[] = "Write \"/Help\" for list of commands \r\n";
  HAL_UART_Transmit(&huart2, help, sizeof(help)-1, 100);
}


