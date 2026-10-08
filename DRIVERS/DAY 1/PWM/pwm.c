#include "pwm.h"
#include "gpio.h"

/* ============================================================
   ATmega2560 Timer3 - Channel A
   PWM Output Pin: OC3A = PE3
   ============================================================ */

/* Timer3 Control Register A */
#define TCCR3A (*(volatile uint8_t *)0x90)

/* Timer3 Control Register B */
#define TCCR3B (*(volatile uint8_t *)0x91)

/* Timer3 Output Compare Register A */
#define OCR3A  (*(volatile uint8_t *)0x97)


/* ============================================================
   PWM_Init
   ============================================================ */

void PWM_Init(void)
{
    /*
     * OC3A / PE3 as OUTPUT
     */

    GPIO_SetIO(&DDRE, 3, OUTPUT);


    /*
     * Timer3 Fast PWM, 8-bit
     *
     * WGM30 = 1
     * WGM31 = 1
     *
     * COM3A1 = 1
     *
     * Non-inverting PWM
     */

    TCCR3A = (1 << 7) |
             (1 << 0) |
             (1 << 1);


    /*
     * Timer stopped initially
     *
     * WGM32 = 0
     * WGM33 = 0
     *
     * Clock select = 0
     */

    TCCR3B = 0;


    /*
     * Initial duty cycle = 0%
     */

    OCR3A = 0;
}


/* ============================================================
   PWM_SetDuty
   ============================================================ */

void PWM_SetDuty(uint8_t duty)
{
    /*
     * Limit duty cycle to 0-100%
     */

    if (duty > 100)
    {
        duty = 100;
    }


    /*
     * Convert 0-100% to 0-255
     */

    OCR3A = (uint8_t)(((uint16_t)duty * 255) / 100);
}


/* ============================================================
   PWM_Start
   ============================================================ */

void PWM_Start(void)
{
    /*
     * Timer3 clock:
     *
     * F_CPU / 64
     *
     * CS31 = 1
     * CS30 = 1
     */

    TCCR3B |= (1 << 1) |
              (1 << 0);
}


/* ============================================================
   PWM_Stop
   ============================================================ */

void PWM_Stop(void)
{
    /*
     * Stop Timer3
     */

    TCCR3B &= ~((1 << 1) |
                (1 << 0));
}