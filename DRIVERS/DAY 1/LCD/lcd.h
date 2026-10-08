#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t column);
void LCD_WriteChar(char data);
void LCD_WriteString(const char *str);
void LCD_PrintNumber(unsigned int number);
void LCD_PrintDistance(unsigned int distance);

#endif