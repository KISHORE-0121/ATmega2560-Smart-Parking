#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include <stdint.h>

/* Initialize Timer1 */
void TIMER_Init(void);

/* Blocking delay in milliseconds */
void TIMER_Delay_ms(uint16_t ms);

#endif