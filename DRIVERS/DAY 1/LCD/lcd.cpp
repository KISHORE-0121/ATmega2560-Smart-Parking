#include "lcd.h"

/* =========================================
   ATmega2560 PORT A REGISTERS

   PA0 = RS
   PA1 = E
   PA2 = D4
   PA3 = D5
   PA4 = D6
   PA5 = D7
   ========================================= */

#define PINA  (*(volatile uint8_t *)0x20)
#define DDRA  (*(volatile uint8_t *)0x21)
#define PORTA (*(volatile uint8_t *)0x22)

/* LCD pins */

#define LCD_RS  0
#define LCD_E   1

#define LCD_D4  2
#define LCD_D5  3
#define LCD_D6  4
#define LCD_D7  5


/* =========================================
   SIMPLE DELAY
   ========================================= */

static void LCD_Delay(void)
{
    volatile unsigned long i;

    for (i = 0; i < 5000UL; i++)
    {
        /* delay */
    }
}


/* =========================================
   ENABLE PULSE
   ========================================= */

static void LCD_Enable(void)
{
    PORTA |= (1 << LCD_E);

    LCD_Delay();

    PORTA &= ~(1 << LCD_E);

    LCD_Delay();
}


/* =========================================
   SEND 4 BITS
   ========================================= */

static void LCD_Send4Bits(uint8_t data)
{
    /* Clear D4-D7 */

    PORTA &= ~(
        (1 << LCD_D4) |
        (1 << LCD_D5) |
        (1 << LCD_D6) |
        (1 << LCD_D7)
    );

    /* Set D4 */

    if (data & 0x01)
        PORTA |= (1 << LCD_D4);

    /* Set D5 */

    if (data & 0x02)
        PORTA |= (1 << LCD_D5);

    /* Set D6 */

    if (data & 0x04)
        PORTA |= (1 << LCD_D6);

    /* Set D7 */

    if (data & 0x08)
        PORTA |= (1 << LCD_D7);

    LCD_Enable();
}


/* =========================================
   SEND COMMAND
   ========================================= */

static void LCD_SendCommand(uint8_t command)
{
    /* RS = 0 */

    PORTA &= ~(1 << LCD_RS);

    /* Send upper nibble */

    LCD_Send4Bits(command >> 4);

    /* Send lower nibble */

    LCD_Send4Bits(command & 0x0F);

    LCD_Delay();
}


/* =========================================
   SEND DATA / CHARACTER
   ========================================= */

void LCD_WriteChar(char data)
{
    /* RS = 1 */

    PORTA |= (1 << LCD_RS);

    /* Upper nibble */

    LCD_Send4Bits(data >> 4);

    /* Lower nibble */

    LCD_Send4Bits(data & 0x0F);

    LCD_Delay();
}


/* =========================================
   LCD INITIALIZATION
   ========================================= */

void LCD_Init(void)
{
    /* PA0-PA5 as OUTPUT */

    DDRA |=
        (1 << LCD_RS) |
        (1 << LCD_E)  |
        (1 << LCD_D4) |
        (1 << LCD_D5) |
        (1 << LCD_D6) |
        (1 << LCD_D7);


    /* Initially LOW */

    PORTA &= ~(
        (1 << LCD_RS) |
        (1 << LCD_E)  |
        (1 << LCD_D4) |
        (1 << LCD_D5) |
        (1 << LCD_D6) |
        (1 << LCD_D7)
    );


    /*
       LCD power-up delay
    */

    for (volatile unsigned long i = 0;
         i < 30000UL;
         i++)
    {
    }


    /*
       Force 4-bit initialization
    */

    PORTA &= ~(1 << LCD_RS);

    LCD_Send4Bits(0x03);

    LCD_Delay();

    LCD_Send4Bits(0x03);

    LCD_Delay();

    LCD_Send4Bits(0x03);

    LCD_Delay();

    LCD_Send4Bits(0x02);

    LCD_Delay();


    /*
       Function set
       4-bit
       2-line
       5x8 font
    */

    LCD_SendCommand(0x28);


    /*
       Display OFF
    */

    LCD_SendCommand(0x08);


    /*
       Clear display
    */

    LCD_SendCommand(0x01);

    LCD_Delay();


    /*
       Entry mode
       Cursor moves right
    */

    LCD_SendCommand(0x06);


    /*
       Display ON
       Cursor OFF
       Blink OFF
    */

    LCD_SendCommand(0x0C);
}


/* =========================================
   CLEAR LCD
   ========================================= */

void LCD_Clear(void)
{
    LCD_SendCommand(0x01);

    LCD_Delay();
}


/* =========================================
   SET CURSOR
   =========================================

   row = 0 → first line
   row = 1 → second line

   column = 0 to 15
   ========================================= */

void LCD_SetCursor(uint8_t row, uint8_t column)
{
    uint8_t address;

    if (row == 0)
    {
        address = 0x00 + column;
    }
    else
    {
        address = 0x40 + column;
    }

    LCD_SendCommand(0x80 | address);
}


/* =========================================
   PRINT STRING
   ========================================= */

void LCD_WriteString(const char *str)
{
    while (*str)
    {
        LCD_WriteChar(*str);

        str++;
    }
}


/* =========================================
   PRINT NUMBER
   ========================================= */

void LCD_PrintNumber(unsigned int number)
{
    char buffer[6];

    uint8_t i = 0;

    if (number == 0)
    {
        LCD_WriteChar('0');
        return;
    }

    while (number > 0)
    {
        buffer[i] = (number % 10) + '0';

        number = number / 10;

        i++;
    }

    while (i > 0)
    {
        i--;

        LCD_WriteChar(buffer[i]);
    }
}


/* =========================================
   PRINT ULTRASONIC DISTANCE
   ========================================= */

void LCD_PrintDistance(unsigned int distance)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);

    LCD_WriteString("Distance:");

    LCD_SetCursor(1, 0);

    if (distance == 0)
    {
        LCD_WriteString("No Echo");
    }
    else
    {
        LCD_PrintNumber(distance);

        LCD_WriteString(" cm");
    }
}