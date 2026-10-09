#include "timer.h"

/* Timer1: 1 ms time base
   CPU frequency = 16 MHz
   Prescaler     = 64
   Timer clock   = 250 kHz
   OCR1A         = 249

   Compare period = (249 + 1) / 250000
                  = 1 ms
*/

void TIMER_Init(void)
{
    /* Stop Timer1 */
    TCCR1A = 0;
    TCCR1B = 0;

    /* Set Timer1 to CTC mode */
    TCCR1B |= (1 << WGM12);

    /* Compare value for 1 ms */
    OCR1AH = 0;
    OCR1AL = 249;

    /* Reset counter */
    TCNT1H = 0;
    TCNT1L = 0;

    /* Clear Output Compare A flag */
    TIFR1 = (1 << OCF1A);

    /* Start Timer1 with prescaler 64 */
    TCCR1B |= (1 << CS11) | (1 << CS10);
}


/* Delay in milliseconds */
void TIMER_Delay_ms(uint16_t ms)
{
    while (ms > 0)
    {
        /* Reset counter */
        TCNT1H = 0;
        TCNT1L = 0;

        /* Clear compare-match flag */
        TIFR1 = (1 << OCF1A);

        /* Wait for 1 ms compare match */
        while (!(TIFR1 & (1 << OCF1A)))
        {
        }

        ms--;
    }
}
