#ifndef OLED_H
#define OLED_H

#include <stdint.h>

void OLED_Init(void);
void OLED_Clear(void);
void OLED_PrintDistance(unsigned int distance);

#endif