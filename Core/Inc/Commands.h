#ifndef __COMMANDS_H
#define __COMMANDS_H
#include <stdbool.h>
#include "main.h"
bool Lora_Show_List_of_Commands (void);
bool Parser_Commands (void);
void Uart_Data_Clear (void);
void Error (void);
bool ASCII_to_hex (uint8_t*, uint8_t*, uint8_t*);
#endif