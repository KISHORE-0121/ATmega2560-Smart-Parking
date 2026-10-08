#ifndef IR_H
#define IR_H

#include <stdint.h>

/* IR sensor status */
#define IR_AVAILABLE  0
#define IR_OCCUPIED   1

/* IR Driver APIs */
void IR_Init(void);
uint8_t IR_Read(void);

#endif