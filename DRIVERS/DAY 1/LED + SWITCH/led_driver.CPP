#include "led_driver.h"
#include "gpio.h"

void LED_Init(volatile uint8_t *DDR, uint8_t pin)
{
    SETIO(DDR, pin, OUTPUT);
}

void LED_On(volatile uint8_t *PORT, uint8_t pin)
{
    OUT_WRITE(PORT, pin, HIGH);
}

void LED_Off(volatile uint8_t *PORT, uint8_t pin)
{
    OUT_WRITE(PORT, pin, LOW);
}

void LED_Toggle(volatile uint8_t *PORT, uint8_t pin)
{
    TOGGLE(PORT, pin);
}
