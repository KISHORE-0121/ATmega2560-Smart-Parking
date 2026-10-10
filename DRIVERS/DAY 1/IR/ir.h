
#ifndef IR_H
#define IR_H

#include <stdint.h>

/* IR sensor status */
#define IR_AVAILABLE  0
#define IR_OCCUPIED   1

/* IR driver APIs */
void IR_Init(void);
uint8_t IR_Read(void);
uint8_t IR_ReadSlot1(void);
uint8_t IR_ReadSlot2(void);

#endif
