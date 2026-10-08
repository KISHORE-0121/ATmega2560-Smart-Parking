#include "oled.h"

/* ================================
   ATmega2560 TWI REGISTERS
   ================================ */

#define TWBR   (*(volatile uint8_t *)0xB8)
#define TWSR   (*(volatile uint8_t *)0xB9)
#define TWAR   (*(volatile uint8_t *)0xBA)
#define TWDR   (*(volatile uint8_t *)0xBB)
#define TWCR   (*(volatile uint8_t *)0xBC)

/* TWI Control bits */
#define TWINT  7
#define TWEA   6
#define TWSTA  5
#define TWSTO  4
#define TWEN   2

#define F_CPU 16000000UL
#define SCL_FREQ 100000UL

#define OLED_ADDR 0x3C

/* ================================
   TWI INITIALIZATION
   ================================ */

static void TWI_Init(void)
{
    /*
       SCL = F_CPU / (16 + 2*TWBR*Prescaler)

       Prescaler = 1
       TWBR = 72

       Gives approximately 100 kHz
    */

    TWSR = 0x00;

    TWBR = 72;

    TWCR = (1 << TWEN);
}

/* ================================
   TWI START
   ================================ */

static void TWI_Start(void)
{
    TWCR =
        (1 << TWINT) |
        (1 << TWSTA) |
        (1 << TWEN);

    while (!(TWCR & (1 << TWINT)));
}

/* ================================
   TWI WRITE
   ================================ */

static void TWI_Write(uint8_t data)
{
    TWDR = data;

    TWCR =
        (1 << TWINT) |
        (1 << TWEN);

    while (!(TWCR & (1 << TWINT)));
}

/* ================================
   TWI STOP
   ================================ */

static void TWI_Stop(void)
{
    TWCR =
        (1 << TWINT) |
        (1 << TWEN) |
        (1 << TWSTO);
}

/* ================================
   OLED COMMAND
   ================================ */

static void OLED_Command(uint8_t command)
{
    TWI_Start();

    TWI_Write((OLED_ADDR << 1) | 0);

    TWI_Write(0x00);

    TWI_Write(command);

    TWI_Stop();
}

/* ================================
   OLED DATA
   ================================ */

static void OLED_Data(uint8_t data)
{
    TWI_Start();

    TWI_Write((OLED_ADDR << 1) | 0);

    TWI_Write(0x40);

    TWI_Write(data);

    TWI_Stop();
}

/* ================================
   OLED INIT
   ================================ */

void OLED_Init(void)
{
    TWI_Init();

    OLED_Command(0xAE);   // Display OFF

    OLED_Command(0xD5);
    OLED_Command(0x80);

    OLED_Command(0xA8);
    OLED_Command(0x3F);

    OLED_Command(0xD3);
    OLED_Command(0x00);

    OLED_Command(0x40);

    OLED_Command(0x8D);
    OLED_Command(0x14);

    OLED_Command(0x20);
    OLED_Command(0x00);

    OLED_Command(0xA1);

    OLED_Command(0xC8);

    OLED_Command(0xDA);
    OLED_Command(0x12);

    OLED_Command(0x81);
    OLED_Command(0x8F);

    OLED_Command(0xD9);
    OLED_Command(0xF1);

    OLED_Command(0xDB);
    OLED_Command(0x40);

    OLED_Command(0xA4);

    OLED_Command(0xA6);

    OLED_Command(0xAF);   // Display ON
}

/* ================================
   OLED CLEAR
   ================================ */

void OLED_Clear(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        OLED_Command(0xB0 + page);

        OLED_Command(0x00);
        OLED_Command(0x10);

        for (uint8_t column = 0; column < 128; column++)
        {
            OLED_Data(0x00);
        }
    }
}