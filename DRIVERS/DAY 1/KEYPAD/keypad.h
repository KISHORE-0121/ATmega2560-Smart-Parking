
#ifndef KEYPAD_H
#define KEYPAD_H

/* Initialize the 4x4 keypad GPIO pins */
void KEYPAD_Init(void);

/* Read the pressed key
   Returns 0-9, A-D, * or #
   Returns '\0' when no key is pressed */
char KEYPAD_Read(void);

#endif
