#include "ir.h"

/* ATmega2560 Port B register addresses */
#define DDRB   (*(volatile unsigned char *)0x24)
#define PORTB  (*(volatile unsigned char *)0x25)
#define PINB   (*(volatile unsigned char *)0x23)

/* IR sensor pin */
#define IR_BIT 0

void IR_Init(void)
{
    /* PB0 as INPUT */
    DDRB &= ~(1 << IR_BIT);

    /* Pull-up disabled */
    PORTB &= ~(1 << IR_BIT);
}

unsigned char IR_Read(void)
{
    /*
     * Active LOW IR sensor:
     *
     * LOW  = Vehicle detected
     * HIGH = Slot available
     */

    if ((PINB & (1 << IR_BIT)) == 0)
    {
        return IR_OCCUPIED;
    }
    else
    {
        return IR_AVAILABLE;
    }
}