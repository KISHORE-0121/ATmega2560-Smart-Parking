#include "adc.h"

/* ============================================================
   ATmega2560 ADC Registers
   ============================================================ */

#define ADCL   (*(volatile uint8_t *)0x78)
#define ADCH   (*(volatile uint8_t *)0x79)

#define ADCSRA (*(volatile uint8_t *)0x7A)
#define ADCSRB (*(volatile uint8_t *)0x7B)

#define ADMUX  (*(volatile uint8_t *)0x7C)

#define DIDR0  (*(volatile uint8_t *)0x7E)


/* ============================================================
   ADC_Init
   ============================================================ */

void ADC_Init(void)
{
    /*
     * ADMUX:
     *
     * REFS1:0 = 01
     * AVCC reference voltage
     *
     * ADLAR = 0
     * Right adjusted result
     *
     * MUX4:0 = 00000
     * ADC0 selected initially
     */

    ADMUX = (1 << 6);


    /*
     * ADCSRA:
     *
     * ADEN  = 1
     * Enable ADC
     *
     * ADPS2:0 = 111
     * Prescaler = 128
     *
     * For 16 MHz:
     *
     * ADC clock = 16 MHz / 128
     *            = 125 kHz
     */

    ADCSRA = (1 << 7) |
             (1 << 2) |
             (1 << 1) |
             (1 << 0);


    /*
     * ADCSRB
     *
     * Free running mode / normal single conversion
     */

    ADCSRB = 0;


    /*
     * Disable digital input on ADC0-ADC7
     */

    DIDR0 = 0x00;
}


/* ============================================================
   ADC_Read
   ============================================================ */

uint16_t ADC_Read(uint8_t channel)
{
    uint16_t result;


    /*
     * Select ADC channel
     *
     * Keep only channel 0-7
     */

    channel &= 0x07;

    /*
     * Keep AVCC reference and select channel
     */

    ADMUX = (1 << 6) | channel;


    /*
     * Start ADC conversion
     */

    ADCSRA |= (1 << 6);


    /*
     * Wait until conversion is complete
     *
     * ADIF = bit 4
     */

    while ((ADCSRA & (1 << 4)) == 0)
    {
        /* Wait */
    }


    /*
     * Clear ADIF
     */

    ADCSRA |= (1 << 4);


    /*
     * Read ADC result
     *
     * IMPORTANT:
     * Read ADCL first, then ADCH
     */

    result = ADCL;

    result |= ((uint16_t)ADCH << 8);


    return result;
}