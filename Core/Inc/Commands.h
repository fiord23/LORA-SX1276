#ifndef __COMMANDS_H
#define __COMMANDS_H
#include <stdbool.h>
#include "main.h"

bool Parser_Commands (void);
void Lora_Show_List_of_Commands (void);
void Command_FW(void);
void Command_Set(void);
void Command_Read(void);
void Show_Config (void);
void Uart_Data_Clear (void);
void Error (void);
bool ASCII_to_hex (uint8_t*, uint8_t*, uint8_t*);
void Hex_to_ASCII(uint8_t*, uint8_t*, uint8_t*);
#endif