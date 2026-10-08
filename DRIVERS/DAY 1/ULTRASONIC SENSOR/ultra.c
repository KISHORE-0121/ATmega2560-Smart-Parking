#include "ultra.h"


/* ============================================================
   ATmega2560 ULTRASONIC DRIVER

   SENSOR:
   HC-SR04 / compatible ultrasonic sensor

   TRIGGER:
   PB1

   ECHO:
   PB2


   TIMER:
   TIMER4

   TIMER MODE:
   Normal Mode

   CPU CLOCK:
   16 MHz

   PRESCALER:
   8

   TIMER CLOCK:

   16 MHz / 8
   = 2 MHz

   TIMER TICK:

   1 / 2 MHz
   = 0.5 us
   ============================================================ */


/* ============================================================
   PORT B REGISTERS
   ============================================================ */

#define PINB  (*(volatile uint8_t *)0x23)
#define DDRB  (*(volatile uint8_t *)0x24)
#define PORTB (*(volatile uint8_t *)0x25)


/* ============================================================
   TIMER4 REGISTERS
   ============================================================ */

#define TCCR4A (*(volatile uint8_t *)0xA0)
#define TCCR4B (*(volatile uint8_t *)0xA1)

#define TCNT4L (*(volatile uint8_t *)0xA4)
#define TCNT4H (*(volatile uint8_t *)0xA5)


/* ============================================================
   ULTRASONIC PINS
   ============================================================ */

#define TRIG_BIT 1       /* PB1 */
#define ECHO_BIT 2       /* PB2 */


/* ============================================================
   TIMER SETTINGS
   ============================================================ */

#define ULTRA_TIMEOUT_TICKS 50000U


/*
   50000 ticks × 0.5 us
   = 25000 us
   = 25 ms

   This gives a reasonable timeout for a
   parking-distance application.
*/


/* ============================================================
   ULTRA_TIMER_RESET
   ============================================================ */

static void ULTRA_TimerReset(void)
{
    TCNT4H = 0x00;
    TCNT4L = 0x00;
}


/* ============================================================
   ULTRA_TimerRead
   ============================================================ */

static uint16_t ULTRA_TimerRead(void)
{
    uint8_t low;
    uint8_t high;

    /*
       Read low byte first.
    */

    low = TCNT4L;

    /*
       Then read high byte.
    */

    high = TCNT4H;

    return ((uint16_t)high << 8) | low;
}


/* ============================================================
   ULTRA_Delay10us
   ============================================================ */

static void ULTRA_Delay10us(void)
{
    /*
       Timer4 tick = 0.5 us

       10 us / 0.5 us
       = 20 ticks
    */

    ULTRA_TimerReset();

    while (ULTRA_TimerRead() < 20U)
    {
        /*
           Wait approximately 10 us
        */
    }
}


/* ============================================================
   ULTRA_Init
   ============================================================ */

void ULTRA_Init(void)
{
    /* --------------------------------------------------------
       TRIG = OUTPUT
       -------------------------------------------------------- */

    DDRB |= (1 << TRIG_BIT);


    /* --------------------------------------------------------
       ECHO = INPUT
       -------------------------------------------------------- */

    DDRB &= ~(1 << ECHO_BIT);


    /* --------------------------------------------------------
       TRIG initially LOW
       -------------------------------------------------------- */

    PORTB &= ~(1 << TRIG_BIT);


    /* --------------------------------------------------------
       TIMER4 NORMAL MODE
       -------------------------------------------------------- */

    TCCR4A = 0x00;


    /*
       Timer4:

       CS42 = 0
       CS41 = 1
       CS40 = 0

       Prescaler = 8
    */

    TCCR4B = (1 << 1);


    /* --------------------------------------------------------
       Reset timer
       -------------------------------------------------------- */

    ULTRA_TimerReset();
}


/* ============================================================
   ULTRA_GetDistanceCm
   ============================================================ */

unsigned int ULTRA_GetDistanceCm(void)
{
    uint16_t start_time;
    uint16_t end_time;
    uint16_t echo_time;


    /* --------------------------------------------------------
       Make sure trigger is LOW
       -------------------------------------------------------- */

    PORTB &= ~(1 << TRIG_BIT);


    /*
       Small settling delay
    */

    ULTRA_Delay10us();


    /* --------------------------------------------------------
       Send trigger pulse
       -------------------------------------------------------- */

    PORTB |= (1 << TRIG_BIT);


    /*
       Trigger HIGH for approximately 10 us
    */

    ULTRA_Delay10us();


    /*
       End trigger pulse
    */

    PORTB &= ~(1 << TRIG_BIT);


    /* --------------------------------------------------------
       Reset Timer4
       -------------------------------------------------------- */

    ULTRA_TimerReset();


    /* --------------------------------------------------------
       Wait for ECHO to become HIGH
       -------------------------------------------------------- */

    while ((PINB & (1 << ECHO_BIT)) == 0)
    {
        /*
           If ECHO never becomes HIGH,
           return 0 instead of waiting forever.
        */

        if (ULTRA_TimerRead() > ULTRA_TIMEOUT_TICKS)
        {
            return 0;
        }
    }


    /* --------------------------------------------------------
       ECHO became HIGH

       Save starting time
       -------------------------------------------------------- */

    start_time = ULTRA_TimerRead();


    /* --------------------------------------------------------
       Wait for ECHO to become LOW
       -------------------------------------------------------- */

    while ((PINB & (1 << ECHO_BIT)) != 0)
    {
        /*
           Timeout protection.

           Unsigned subtraction also handles
           timer wrap-around correctly.
        */

        if ((uint16_t)(ULTRA_TimerRead() - start_time)
            > ULTRA_TIMEOUT_TICKS)
        {
            return 0;
        }
    }


    /* --------------------------------------------------------
       Save ending time
       -------------------------------------------------------- */

    end_time = ULTRA_TimerRead();


    /* --------------------------------------------------------
       Calculate ECHO pulse width
       -------------------------------------------------------- */

    echo_time = end_time - start_time;


    /* --------------------------------------------------------
       Convert timer ticks to distance

       Timer tick = 0.5 us

       echo_time × 0.5
       = time in microseconds

       Distance(cm)
       ≈ time(us) / 58

       Therefore:

       Distance
       ≈ echo_time / 116
       -------------------------------------------------------- */

    return (unsigned int)(echo_time / 116U);
}