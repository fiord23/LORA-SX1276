#ifndef __COMMANDS_H
#define __COMMANDS_H
#include <stdbool.h>
bool Lora_Show_List_of_Commands (void);
bool Parser_Commands (void);
void Uart_Data_Clear (void);
#endif