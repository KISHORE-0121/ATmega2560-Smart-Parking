#include "seg7.h"
#include "gpio.h"


/* ============================================================
   7-SEGMENT PIN CONFIGURATION

   PORTA:
   PA0 -> a
   PA1 -> b
   PA2 -> c
   PA3 -> d
   PA4 -> e
   PA5 -> f
   PA6 -> g
   PA7 -> dp

   PC0 -> Digit 1
   PC1 -> Digit 2

   Common Cathode
   ============================================================ */


/* ============================================================
   LUT
   ============================================================ */

/*
       a
      ---
   f |   | b
      -g-
   e |   | c
      ---
       d

   Bit:
   PA0 = a
   PA1 = b
   PA2 = c
   PA3 = d
   PA4 = e
   PA5 = f
   PA6 = g
   PA7 = dp
*/

static const uint8_t SEG7_LUT[10] =
{
    0b00111111,   /* 0 */
    0b00000110,   /* 1 */
    0b01011011,   /* 2 */
    0b01001111,   /* 3 */
    0b01100110,   /* 4 */
    0b01101101,   /* 5 */
    0b01111101,   /* 6 */
    0b00000111,   /* 7 */
    0b01111111,   /* 8 */
    0b01101111    /* 9 */
};


/* ============================================================
   SEG7_Init
   ============================================================ */

void SEG7_Init(void)
{
    /*
     * PORTA:
     * All 8 pins are outputs
     */

    SETIO(&DDRA, 0, OUTPUT);
    SETIO(&DDRA, 1, OUTPUT);
    SETIO(&DDRA, 2, OUTPUT);
    SETIO(&DDRA, 3, OUTPUT);
    SETIO(&DDRA, 4, OUTPUT);
    SETIO(&DDRA, 5, OUTPUT);
    SETIO(&DDRA, 6, OUTPUT);
    SETIO(&DDRA, 7, OUTPUT);


    /*
     * Digit select pins
     */

    SETIO(&DDRC, 0, OUTPUT);
    SETIO(&DDRC, 1, OUTPUT);


    /*
     * Initially turn everything OFF
     */

    PORTA = 0x00;

    OUT_WRITE(&PORTC, 0, LOW);
    OUT_WRITE(&PORTC, 1, LOW);
}


/* ============================================================
   SEG7_DisplayDigit
   ============================================================ */

void SEG7_DisplayDigit(uint8_t digit)
{
    /*
     * Only allow 0-9
     */

    if (digit > 9)
    {
        SEG7_Clear();
        return;
    }


    /*
     * Output segment pattern
     */

    PORTA = SEG7_LUT[digit];
}


/* ============================================================
   SEG7_DisplayNumber
   ============================================================ */

void SEG7_DisplayNumber(uint8_t number)
{
    uint8_t tens;
    uint8_t units;

    /*
     * Maximum supported number = 99
     */

    if (number > 99)
    {
        number = 99;
    }


    tens = number / 10;
    units = number % 10;


    /*
     * This function calculates the digits.
     *
     * For a two-digit multiplexed display,
     * the application/timer should alternate
     * between tens and units.
     *
     * For now, this function leaves the calculated
     * values available for the display logic.
     */

    (void)tens;
    (void)units;
}


/* ============================================================
   SEG7_Clear
   ============================================================ */

void SEG7_Clear(void)
{
    PORTA = 0x00;

    OUT_WRITE(&PORTC, 0, LOW);
    OUT_WRITE(&PORTC, 1, LOW);
}
