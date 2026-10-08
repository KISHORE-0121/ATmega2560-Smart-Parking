#ifndef SEG7_H
#define SEG7_H

#include <stdint.h>

/* Initialize 7-segment display */
void SEG7_Init(void);

/* Display one digit: 0-9 */
void SEG7_DisplayDigit(uint8_t digit);

/* Display two-digit number: 0-99 */
void SEG7_DisplayNumber(uint8_t number);

/* Clear display */
void SEG7_Clear(void);

#endif