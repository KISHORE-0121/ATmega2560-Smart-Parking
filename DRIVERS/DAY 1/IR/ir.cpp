
#include "ir.h"

/* ATmega2560 Port K registers */
#define PINK_REG  (*(volatile uint8_t *)0x106)
#define DDRK_REG  (*(volatile uint8_t *)0x107)
#define PORTK_REG (*(volatile uint8_t *)0x108)

/* Sensor pin assignments */
#define IR_SLOT1_BIT  0  /* PK0 = Arduino Mega A8 */
#define IR_SLOT2_BIT  1  /* PK1 = Arduino Mega A9 */

void IR_Init(void)
{
    /* Configure PK0 and PK1 as inputs */
    DDRK_REG &= ~((1 << IR_SLOT1_BIT) |
                  (1 << IR_SLOT2_BIT));

    /* Enable internal pull-ups */
    PORTK_REG |= (1 << IR_SLOT1_BIT) |
                 (1 << IR_SLOT2_BIT);
}

uint8_t IR_ReadSlot1(void)
{
    /* Active LOW: LOW = occupied */
    if ((PINK_REG & (1 << IR_SLOT1_BIT)) == 0)
        return IR_OCCUPIED;

    return IR_AVAILABLE;
}

uint8_t IR_ReadSlot2(void)
{
    /* Active LOW: LOW = occupied */
    if ((PINK_REG & (1 << IR_SLOT2_BIT)) == 0)
        return IR_OCCUPIED;

    return IR_AVAILABLE;
}

/* Preserve compatibility with your original main code */
uint8_t IR_Read(void)
{
    return IR_ReadSlot1();
}
