#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>

void LED_Init(volatile uint8_t *DDR, uint8_t pin);

void LED_On(volatile uint8_t *PORT, uint8_t pin);

void LED_Off(volatile uint8_t *PORT, uint8_t pin);

void LED_Toggle(volatile uint8_t *PORT, uint8_t pin);

#endif