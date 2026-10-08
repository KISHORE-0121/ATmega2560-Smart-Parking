#include "gpio.h"

void SETIO(volatile uint8_t *DDR,
           uint8_t pin,
           uint8_t mode)
{
    if (mode == OUTPUT)
        *DDR |= (1 << pin);
    else
        *DDR &= ~(1 << pin);
}

void GPIO_Write(volatile uint8_t *PORT,
                uint8_t pin,
                uint8_t value)
{
    if (value == HIGH)
        *PORT |= (1 << pin);
    else
        *PORT &= ~(1 << pin);
}

uint8_t GPIO_Read(volatile uint8_t *PIN,
                  uint8_t pin)
{
    return ((*PIN >> pin) & 1);
}

void GPIO_Toggle(volatile uint8_t *PORT,
                 uint8_t pin)
{
    *PORT ^= (1 << pin);
}
