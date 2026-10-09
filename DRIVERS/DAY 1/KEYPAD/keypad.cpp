#include "keypad.h"
#include "gpio.h"
#include <stdint.h>

/*
 * 4x4 keypad connections
 *
 * L1 -> PF1  (Arduino Mega A1)
 * L2 -> PF2  (Arduino Mega A2)
 * L3 -> PF3  (Arduino Mega A3)
 * L4 -> PF4  (Arduino Mega A4)
 *
 * R1 -> PF5  (Arduino Mega A5)
 * R2 -> PF6  (Arduino Mega A6)
 * R3 -> PF7  (Arduino Mega A7)
 * R4 -> PG0  (Arduino Mega D41)
 *
 * Port F: PF1-PF7
 * Port G: PG0
 */

/* Keypad pin numbers */
#define L1 1
#define L2 2
#define L3 3
#define L4 4

#define R1 5
#define R2 6
#define R3 7
#define R4 0

/* Select a column output LOW at a time */
static void KEYPAD_Select(uint8_t column)
{
    /* Set all column outputs HIGH */
    OUT_WRITE(&PORTF, R1, HIGH);
    OUT_WRITE(&PORTF, R2, HIGH);
    OUT_WRITE(&PORTF, R3, HIGH);
    OUT_WRITE(&PORTG, R4, HIGH);

    /* Set the selected column LOW */
    if (column == 0)
        OUT_WRITE(&PORTF, R1, LOW);
    else if (column == 1)
        OUT_WRITE(&PORTF, R2, LOW);
    else if (column == 2)
        OUT_WRITE(&PORTF, R3, LOW);
    else
        OUT_WRITE(&PORTG, R4, LOW);
}

/* Initialize keypad rows and columns */
void KEYPAD_Init(void)
{
    /* Configure rows as inputs */
    SETIO(&DDRF, L1, INPUT);
    SETIO(&DDRF, L2, INPUT);
    SETIO(&DDRF, L3, INPUT);
    SETIO(&DDRF, L4, INPUT);

    /* Enable internal pull-up resistors on rows */
    OUT_WRITE(&PORTF, L1, HIGH);
    OUT_WRITE(&PORTF, L2, HIGH);
    OUT_WRITE(&PORTF, L3, HIGH);
    OUT_WRITE(&PORTF, L4, HIGH);

    /* Configure columns as outputs */
    SETIO(&DDRF, R1, OUTPUT);
    SETIO(&DDRF, R2, OUTPUT);
    SETIO(&DDRF, R3, OUTPUT);
    SETIO(&DDRG, R4, OUTPUT);

    /* Set all columns HIGH initially */
    OUT_WRITE(&PORTF, R1, HIGH);
    OUT_WRITE(&PORTF, R2, HIGH);
    OUT_WRITE(&PORTF, R3, HIGH);
    OUT_WRITE(&PORTG, R4, HIGH);
}

/* Scan the keypad and return the pressed key */
char KEYPAD_Read(void)
{
    uint8_t row;
    uint8_t column;
    uint8_t pressed;

    /* Key layout */
    const char keys[4][4] =
    {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    /* Scan each column */
    for (column = 0; column < 4; column++)
    {
        /* Select one column */
        KEYPAD_Select(column);

        /* Check each row */
        for (row = 0; row < 4; row++)
        {
            if (row == 0)
                pressed = READ(&PINF, L1);
            else if (row == 1)
                pressed = READ(&PINF, L2);
            else if (row == 2)
                pressed = READ(&PINF, L3);
            else
                pressed = READ(&PINF, L4);

            /* LOW means a key is pressed */
            if (pressed == LOW)
            {
                /* Restore columns before returning */
                OUT_WRITE(&PORTF, R1, HIGH);
                OUT_WRITE(&PORTF, R2, HIGH);
                OUT_WRITE(&PORTF, R3, HIGH);
                OUT_WRITE(&PORTG, R4, HIGH);

                return keys[row][column];
            }
        }
    }

    /* No key pressed */
    return '\0';
}
