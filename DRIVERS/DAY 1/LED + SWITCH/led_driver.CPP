#include "led_driver.h"
#include "gpio.h"

void LED_Init(volatile uint8_t *DDR, uint8_t pin)
{
    GPIO_SetIO(DDR, pin, OUTPUT);
}

void LED_On(volatile uint8_t *PORT, uint8_t pin)
{
    GPIO_Write(PORT, pin, HIGH);
}

void LED_Off(volatile uint8_t *PORT, uint8_t pin)
{
    GPIO_Write(PORT, pin, LOW);
}

void LED_Toggle(volatile uint8_t *PORT, uint8_t pin)
{
    GPIO_Toggle(PORT, pin);
}