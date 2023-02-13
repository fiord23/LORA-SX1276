#ifndef __COMMANDS_H
#define __COMMANDS_H
#include <stdbool.h>
#include "main.h"

bool Parser_Commands (void);
void Lora_Show_List_of_Commands (void);
void Command_FW(void);
void Command_Set(char *);
void Command_Read(char *);
void Show_Config (void);
void Uart_Data_Clear (void);
void Error (void);
bool ASCII_to_hex (uint8_t*, uint8_t*, uint8_t*);
void Hex_to_ASCII(uint8_t*, uint8_t*, uint8_t*);
void Command_auto(void);
void Command_Setloop(char *);
void setendstr(uint8_t *);
void CRC8_init (uint8_t);
void dec2hex(uint8_t, uint8_t *);
bool hex2dec (uint8_t*, uint8_t*);
char * toUpper(char*);
uint8_t dec2str(uint32_t, uint8_t *, uint8_t );
void writeToEEPROM (uint32_t, uint32_t);
uint32_t readFromEEPROM (uint32_t);
void Auto_Mode(void);

#endif