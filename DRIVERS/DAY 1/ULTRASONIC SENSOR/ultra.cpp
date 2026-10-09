#include "ultra.h"
#include "gpio.h"
#include "timer.h"
#include <stdint.h>

/*
 * Ultrasonic sensor connections:
 * TRIG -> PB1 (Arduino Mega D52)
 * ECHO -> PB2 (Arduino Mega D51)
 *
 * Timer4:
 * Prescaler = 8
 * Timer tick = 0.5 us at 16 MHz
 */

#define ULTRA_TRIG_PIN  1
#define ULTRA_ECHO_PIN  2

/* Maximum time allowed while waiting for an echo */
#define ULTRA_TIMEOUT_TICKS 50000UL

/* Reset Timer4 counter to zero */
static void ULTRA_TimerReset(void)
{
    TCNT4H = 0;
    TCNT4L = 0;
}

/* Read the current 16-bit Timer4 counter */
static uint16_t ULTRA_TimerRead(void)
{
    uint8_t low;
    uint8_t high;

    /* Read low byte first to latch the high byte */
    low = TCNT4L;
    high = TCNT4H;

    /* Combine high and low bytes */
    return ((uint16_t)high << 8) | low;
}

/* Generate an approximate 10 us delay using Timer4 */
static void ULTRA_Delay10us(void)
{
    ULTRA_TimerReset();

    /* 20 ticks x 0.5 us = 10 us */
    while (ULTRA_TimerRead() < 20)
    {
    }
}

/* Initialize GPIO pins and Timer4 */
void ULTRA_Init(void)
{
    /* Configure TRIG as output */
    SETIO(&DDRB, ULTRA_TRIG_PIN, OUTPUT);

    /* Configure ECHO as input */
    SETIO(&DDRB, ULTRA_ECHO_PIN, INPUT);

    /* Keep TRIG LOW initially */
    OUT_WRITE(&PORTB, ULTRA_TRIG_PIN, LOW);

    /* Select Timer4 normal mode */
    TCCR4A = 0;
    TCCR4B = 0;

    /* Clear Timer4 counter */
    ULTRA_TimerReset();

    /* Start Timer4 with prescaler 8 */
    TCCR4B = (1 << CS41);
}

/* Trigger the sensor and measure echo pulse duration */
unsigned int ULTRA_GetDistanceCm(void)
{
    uint16_t start;
    uint16_t echo_time;

    /* Ensure TRIG is LOW before the pulse */
    OUT_WRITE(&PORTB, ULTRA_TRIG_PIN, LOW);
    ULTRA_Delay10us();

    /* Send the trigger pulse */
    OUT_WRITE(&PORTB, ULTRA_TRIG_PIN, HIGH);
    ULTRA_Delay10us();
    OUT_WRITE(&PORTB, ULTRA_TRIG_PIN, LOW);

    /* Reset timer before waiting for ECHO */
    ULTRA_TimerReset();

    /* Wait until ECHO becomes HIGH */
    while (READ(&PINB, ULTRA_ECHO_PIN) == LOW)
    {
        /* Return 0 if the echo does not arrive */
        if (ULTRA_TimerRead() > ULTRA_TIMEOUT_TICKS)
            return 0;
    }

    /* Save the starting timer count */
    start = ULTRA_TimerRead();

    /* Wait until ECHO becomes LOW */
    while (READ(&PINB, ULTRA_ECHO_PIN) == HIGH)
    {
        /* Return 0 if the echo pulse takes too long */
        if ((uint16_t)(ULTRA_TimerRead() - start) >
            ULTRA_TIMEOUT_TICKS)
            return 0;
    }

    /* Calculate the echo pulse duration in timer ticks */
    echo_time = (uint16_t)(ULTRA_TimerRead() - start);

    /* Convert ticks to centimeters:
       1 tick = 0.5 us
       Distance in cm = echo_time / 116 */
    return (unsigned int)(echo_time / 116);
}
