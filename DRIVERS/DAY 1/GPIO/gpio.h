#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/* ---------- PORT A ---------- */
#define PINA   (*(volatile uint8_t *)0x20)
#define DDRA   (*(volatile uint8_t *)0x21)
#define PORTA  (*(volatile uint8_t *)0x22)

/* ---------- PORT B ---------- */
#define PINB   (*(volatile uint8_t *)0x23)
#define DDRB   (*(volatile uint8_t *)0x24)
#define PORTB  (*(volatile uint8_t *)0x25)

/* ---------- PORT C ---------- */
#define PINC   (*(volatile uint8_t *)0x26)
#define DDRC   (*(volatile uint8_t *)0x27)
#define PORTC  (*(volatile uint8_t *)0x28)

/* ---------- PORT D ---------- */
#define PIND   (*(volatile uint8_t *)0x29)
#define DDRD   (*(volatile uint8_t *)0x2A)
#define PORTD  (*(volatile uint8_t *)0x2B)

/* ---------- PORT E ---------- */
#define PINE   (*(volatile uint8_t *)0x2C)
#define DDRE   (*(volatile uint8_t *)0x2D)
#define PORTE  (*(volatile uint8_t *)0x2E)

/* ---------- PORT F ---------- */
#define PINF   (*(volatile uint8_t *)0x2F)
#define DDRF   (*(volatile uint8_t *)0x30)
#define PORTF  (*(volatile uint8_t *)0x31)

/* ---------- PORT G ---------- */
#define PING   (*(volatile uint8_t *)0x32)
#define DDRG   (*(volatile uint8_t *)0x33)
#define PORTG  (*(volatile uint8_t *)0x34)

/* ---------- PORT H ---------- */
#define PINH   (*(volatile uint8_t *)0x100)
#define DDRH   (*(volatile uint8_t *)0x101)
#define PORTH  (*(volatile uint8_t *)0x102)

/* ---------- PORT J ---------- */
#define PINJ   (*(volatile uint8_t *)0x103)
#define DDRJ   (*(volatile uint8_t *)0x104)
#define PORTJ  (*(volatile uint8_t *)0x105)

/* ---------- PORT K ---------- */
#define PINK   (*(volatile uint8_t *)0x106)
#define DDRK   (*(volatile uint8_t *)0x107)
#define PORTK  (*(volatile uint8_t *)0x108)

/* ---------- PORT L ---------- */
#define PINL   (*(volatile uint8_t *)0x109)
#define DDRL   (*(volatile uint8_t *)0x10A)
#define PORTL  (*(volatile uint8_t *)0x10B)

#define INPUT   0
#define OUTPUT  1
#define LOW     0
#define HIGH    1

/* Function declarations */

void SETIO(volatile uint8_t *DDR, uint8_t pin, uint8_t mode);

void OUT_WRITE(volatile uint8_t *PORT, uint8_t pin, uint8_t value);

uint8_t GPIO_Read(volatile uint8_t *PIN, uint8_t pin);

void GPIO_Toggle(volatile uint8_t *PORT, uint8_t pin);

#endif
