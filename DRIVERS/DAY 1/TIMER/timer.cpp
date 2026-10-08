#include "timer.h"

/* ---------- TIMER1 REGISTERS ---------- */

#define TCCR1A  (*(volatile uint8_t *)0x80)
#define TCCR1B  (*(volatile uint8_t *)0x81)

#define TCNT1L  (*(volatile uint8_t *)0x84)
#define TCNT1H  (*(volatile uint8_t *)0x85)

#define OCR1AL  (*(volatile uint8_t *)0x88)
#define OCR1AH  (*(volatile uint8_t *)0x89)

#define TIFR1   (*(volatile uint8_t *)0x36)

/* ---------- TIMER1 BITS ---------- */

#define WGM12   3
#define CS11    1
#define CS10    0
#define OCF1A   1


void TIMER_Init(void)
{
    /* CTC mode */
    TCCR1A = 0x00;

    /*
       WGM12 = 1
       Prescaler = 64
    */
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);

    /*
       16 MHz / 64 = 250 kHz

       1 ms:
       250000 × 0.001 = 250 counts

       OCR1A = 249
    */
    OCR1AH = 0x00;
    OCR1AL = 249;
}


void TIMER_Delay_ms(uint16_t ms)
{
    while (ms--)
    {
        /* Reset timer counter */
        TCNT1H = 0x00;
        TCNT1L = 0x00;

        /* Wait for compare match */
        while ((TIFR1 & (1 << OCF1A)) == 0)
        {
        }

        /* Clear compare flag */
        TIFR1 |= (1 << OCF1A);
    }
}