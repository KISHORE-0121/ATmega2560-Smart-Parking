
#include "pwm.h"
#include "gpio.h"
#include "timer.h"

/*
 * PWM output:
 * OC3A = PE3 = Arduino Mega D5
 * Timer used: Timer3
 * Mode: 8-bit Fast PWM
 * Prescaler: 64
 */

/* Initialize PWM hardware */
void PWM_Init(void)
{
    /* Configure PE3 as output */
    SETIO(&DDRE, 3, OUTPUT);

    /* Select non-inverting Fast PWM mode */
    TCCR3A = (1 << COM3A1) | (1 << WGM30);
    TCCR3B = (1 << WGM32);

    /* Set initial duty cycle to 0% */
    OCR3AH = 0;
    OCR3AL = 0;
}

/* Set PWM duty cycle (0 to 100%) */
void PWM_SetDuty(uint8_t duty)
{
    uint16_t value;

    /* Limit duty cycle to 100% */
    if (duty > 100)
        duty = 100;

    /* Convert percentage to 8-bit compare value */
    value = ((uint16_t)duty * 255) / 100;

    /* Write the compare value to OCR3A */
    OCR3AH = 0;
    OCR3AL = (uint8_t)value;
}

/* Start PWM using prescaler 64 */
void PWM_Start(void)
{
    /* Clear the Timer3 clock-select bits */
    TCCR3B &= ~((1 << CS32) |
                (1 << CS31) |
                (1 << CS30));

    /* Set prescaler to 64 and start Timer3 */
    TCCR3B |= (1 << CS31) | (1 << CS30);
}

/* Stop PWM by stopping Timer3 */
void PWM_Stop(void)
{
    /* Clear all Timer3 clock-select bits */
    TCCR3B &= ~((1 << CS32) |
                (1 << CS31) |
                (1 << CS30));
}
