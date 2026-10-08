#include "timer.h"


/* ============================================================
   ATmega2560 TIMER1

   PURPOSE:
   System delay

   MODE:
   CTC

   CPU CLOCK:
   16 MHz

   PRESCALER:
   64

   TIMER CLOCK:

   16 MHz / 64
   = 250 kHz

   TIMER TICK:

   1 / 250000
   = 4 us

   For 1 ms:

   1000 us / 4 us
   = 250 counts

   OCR1A = 249

   Therefore:

   One compare match = approximately 1 ms
   ============================================================ */


/* ---------- TIMER1 REGISTERS ---------- */

#define TCCR1A (*(volatile uint8_t *)0x80)
#define TCCR1B (*(volatile uint8_t *)0x81)

#define TCNT1L (*(volatile uint8_t *)0x84)
#define TCNT1H (*(volatile uint8_t *)0x85)

#define OCR1AL (*(volatile uint8_t *)0x88)
#define OCR1AH (*(volatile uint8_t *)0x89)

#define TIFR1  (*(volatile uint8_t *)0x36)


/* ---------- TIMER1 BITS ---------- */

#define WGM12  3

#define CS11   1
#define CS10   0

#define OCF1A  1


/* ============================================================
   TIMER_Init
   ============================================================ */

void TIMER_Init(void)
{
    /*
       CTC MODE

       WGM13 = 0
       WGM12 = 1
       WGM11 = 0
       WGM10 = 0

       Mode = CTC
    */

    TCCR1A = 0x00;


    /*
       WGM12 = 1

       Prescaler = 64

       CS12 = 0
       CS11 = 1
       CS10 = 1
    */

    TCCR1B = (1 << WGM12) |
             (1 << CS11) |
             (1 << CS10);


    /*
       OCR1A = 249

       Gives approximately 1 ms
       compare interval at 16 MHz.
    */

    OCR1AH = 0x00;
    OCR1AL = 249;


    /*
       Reset timer counter
    */

    TCNT1H = 0x00;
    TCNT1L = 0x00;


    /*
       Clear any pending compare flag.

       Timer flags are cleared by writing 1.
    */

    TIFR1 = (1 << OCF1A);
}


/* ============================================================
   TIMER_Delay_ms
   ============================================================ */

void TIMER_Delay_ms(uint16_t ms)
{
    while (ms--)
    {
        /*
           Reset Timer1 counter
        */

        TCNT1H = 0x00;
        TCNT1L = 0x00;


        /*
           Clear old compare flag
        */

        TIFR1 = (1 << OCF1A);


        /*
           Wait until Timer1 reaches OCR1A
        */

        while ((TIFR1 & (1 << OCF1A)) == 0)
        {
            /*
               Wait
            */
        }


        /*
           Clear compare flag.

           Writing 1 clears the flag.
        */

        TIFR1 = (1 << OCF1A);
    }
}