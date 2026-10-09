
#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/* ================= TIMER 1 REGISTERS ================= */

#define TCCR1A  (*(volatile uint8_t *)0x80)
#define TCCR1B  (*(volatile uint8_t *)0x81)

#define TCNT1L  (*(volatile uint8_t *)0x84)
#define TCNT1H  (*(volatile uint8_t *)0x85)

#define OCR1AL  (*(volatile uint8_t *)0x88)
#define OCR1AH  (*(volatile uint8_t *)0x89)

#define TIFR1   (*(volatile uint8_t *)0x36)

/* Timer1 bits */
#define WGM12   3
#define CS11    1
#define CS10    0
#define OCF1A   1


/* ================= TIMER 3 REGISTERS ================= */

#define TCCR3A  (*(volatile uint8_t *)0x90)
#define TCCR3B  (*(volatile uint8_t *)0x91)

#define TCNT3L  (*(volatile uint8_t *)0x94)
#define TCNT3H  (*(volatile uint8_t *)0x95)

#define OCR3AL  (*(volatile uint8_t *)0x98)
#define OCR3AH  (*(volatile uint8_t *)0x99)

/* Timer3 bits */
#define COM3A1  7
#define WGM30   0
#define WGM32   3
#define CS30    0
#define CS31    1
#define CS32    2


/* ================= TIMER 4 REGISTERS ================= */

#define TCCR4A  (*(volatile uint8_t *)0xA0)
#define TCCR4B  (*(volatile uint8_t *)0xA1)

#define TCNT4L  (*(volatile uint8_t *)0xA4)
#define TCNT4H  (*(volatile uint8_t *)0xA5)

#define OCR4AL  (*(volatile uint8_t *)0xA8)
#define OCR4AH  (*(volatile uint8_t *)0xA9)

/* Timer4 bits */
#define CS40    0
#define CS41    1
#define CS42    2


/* ================= TIMER 1 FUNCTIONS ================= */

void TIMER_Init(void);
void TIMER_Delay_ms(uint16_t ms);

#endif
