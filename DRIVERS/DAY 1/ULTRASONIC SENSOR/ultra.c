#include "ultra.h"

/* ============================================================
   ATmega2560 PORT B Registers
   ============================================================ */

#define PINB    (*(volatile unsigned char *)0x23)
#define DDRB    (*(volatile unsigned char *)0x24)
#define PORTB   (*(volatile unsigned char *)0x25)


/* ============================================================
   ATmega2560 TIMER1 Registers
   ============================================================ */

#define TCCR1A  (*(volatile unsigned char *)0x80)
#define TCCR1B  (*(volatile unsigned char *)0x81)

#define TCNT1L  (*(volatile unsigned char *)0x84)
#define TCNT1H  (*(volatile unsigned char *)0x85)

#define TCNT1   (*(volatile unsigned int  *)0x84)


/* ============================================================
   Ultrasonic Pins
   ============================================================ */

#define TRIG_BIT    1       /* PB1 */
#define ECHO_BIT    2       /* PB2 */


/* ============================================================
   Timer settings
   ATmega2560 = 16 MHz
   Prescaler = 8

   Timer frequency:
   16 MHz / 8 = 2 MHz

   One timer tick:
   1 / 2 MHz = 0.5 us
   ============================================================ */


/* ============================================================
   ULTRA_Init
   ============================================================ */

void ULTRA_Init(void)
{
    /* TRIG = OUTPUT */
    DDRB |= (1 << TRIG_BIT);

    /* ECHO = INPUT */
    DDRB &= ~(1 << ECHO_BIT);

    /* TRIG initially LOW */
    PORTB &= ~(1 << TRIG_BIT);


    /* --------------------------------------------------------
       Timer1 Normal Mode
       -------------------------------------------------------- */

    TCCR1A = 0x00;

    /*
       CS11 = 1

       Timer clock:
       F_CPU / 8
    */
    TCCR1B = (1 << 1);

    /* Reset timer */
    TCNT1 = 0;
}


/* ============================================================
   Small delay using Timer1
   ============================================================ */

static void ULTRA_Delay10us(void)
{
    TCNT1 = 0;

    while (TCNT1 < 20)
    {
        /* Wait approximately 10 us
           20 ticks × 0.5 us = 10 us */
    }
}


/* ============================================================
   ULTRA_GetDistanceCm
   ============================================================ */

unsigned int ULTRA_GetDistanceCm(void)
{
    unsigned int start_time;
    unsigned int end_time;
    unsigned int echo_time;


    /* --------------------------------------------------------
       Make sure TRIG is LOW
       -------------------------------------------------------- */

    PORTB &= ~(1 << TRIG_BIT);

    ULTRA_Delay10us();


    /* --------------------------------------------------------
       Send ultrasonic trigger pulse
       -------------------------------------------------------- */

    PORTB |= (1 << TRIG_BIT);

    ULTRA_Delay10us();

    PORTB &= ~(1 << TRIG_BIT);


    /* --------------------------------------------------------
       Wait for ECHO to become HIGH
       -------------------------------------------------------- */

    TCNT1 = 0;

    while ((PINB & (1 << ECHO_BIT)) == 0)
    {
        /*
           Timeout protection.

           If ECHO does not become HIGH,
           don't wait forever.
        */

        if (TCNT1 > 60000)
        {
            return 0;
        }
    }


    /* --------------------------------------------------------
       ECHO became HIGH

       Save starting time
       -------------------------------------------------------- */

    start_time = TCNT1;


    /* --------------------------------------------------------
       Wait for ECHO to become LOW
       -------------------------------------------------------- */

    while ((PINB & (1 << ECHO_BIT)) != 0)
    {
        /*
           Timeout protection
        */

        if ((unsigned int)(TCNT1 - start_time) > 60000)
        {
            return 0;
        }
    }


    /* --------------------------------------------------------
       Save ending time
       -------------------------------------------------------- */

    end_time = TCNT1;


    /* --------------------------------------------------------
       Calculate ECHO pulse width
       -------------------------------------------------------- */

    echo_time = end_time - start_time;


    /* --------------------------------------------------------
       Convert timer ticks to distance

       Timer tick = 0.5 us

       Therefore:

       echo_time × 0.5 = microseconds

       Distance(cm) ≈ time(us) / 58

       Distance ≈ echo_time / 116
       -------------------------------------------------------- */

    return (echo_time / 116);
}