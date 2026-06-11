/*
 * lcd.c
 *
 *  Created on: Jun 7, 2026
 */

/******************************************************************************
 *                             Include Files                                   *
 ******************************************************************************/

#include "lcd.h"
#include "common_macros.h"
#include "gpio.h"
#include <avr/delay.h>
#include <stdlib.h>

/* Sends a command byte to the LCD
 * Handles 4-bit mode by sending upper nibble first, then lower nibble,
 * or sends full byte in 8-bit mode.
 */
void LCD_SendCommand(uint8 command)
{
    // RS = 0 for command mode
    GPIO_writePin(LCD_REGISTER_SELECT_PORT, LCD_REGISTER_SELECT_PIN, LOGIC_LOW);
    _delay_ms(1);

    // Enable = 1 to latch data/command
    GPIO_writePin(LCD_ENABLE_PORT, LCD_ENABLE_PIN, LOGIC_HIGH);
    _delay_ms(1);

#if(4 == LCD_DATA_BITS_MODE)
    // Send upper nibble of command
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB4_PIN_ID, GET_BIT(command,4));
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB5_PIN_ID, GET_BIT(command,5));
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB6_PIN_ID, GET_BIT(command,6));
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB7_PIN_ID, GET_BIT(command,7));
    _delay_ms(1);

    // Enable = 0 to latch upper nibble
    GPIO_writePin(LCD_ENABLE_PORT, LCD_ENABLE_PIN, LOGIC_LOW);
    _delay_ms(1);

    // Enable = 1 for next nibble
    GPIO_writePin(LCD_ENABLE_PORT, LCD_ENABLE_PIN, LOGIC_HIGH);
    _delay_ms(1);

    // Send lower nibble of command
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB4_PIN_ID, GET_BIT(command,0));
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB5_PIN_ID, GET_BIT(command,1));
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB6_PIN_ID, GET_BIT(command,2));
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB7_PIN_ID, GET_BIT(command,3));
#elif(8 == LCD_DATA_BITS_MODE)
    // Send full byte in 8-bit mode
    GPIO_writePort(LCD_DISPLAY_DATA_PORT, command);
#endif

    _delay_ms(1);

    // Enable = 0 to latch low nibble or full byte
    GPIO_writePin(LCD_ENABLE_PORT, LCD_ENABLE_PIN, LOGIC_LOW);
    _delay_ms(1);
}

/* Initializes the LCD pins and mode (4 or 8 bits), and prepares the LCD */
void LCD_init(void)
{
    // Set RS and Enable pins as output
    GPIO_setupPinDirection(LCD_REGISTER_SELECT_PORT, LCD_REGISTER_SELECT_PIN, PIN_OUTPUT);
    GPIO_setupPinDirection(LCD_ENABLE_PORT, LCD_ENABLE_PIN, PIN_OUTPUT);

    _delay_ms(20); // LCD power up delay

#if(4 == LCD_DATA_BITS_MODE)
    // Setup data pins for 4-bit mode as output
    GPIO_setupPinDirection(LCD_DISPLAY_DATA_PORT, LCD_DB4_PIN_ID, PIN_OUTPUT);
    GPIO_setupPinDirection(LCD_DISPLAY_DATA_PORT, LCD_DB5_PIN_ID, PIN_OUTPUT);
    GPIO_setupPinDirection(LCD_DISPLAY_DATA_PORT, LCD_DB6_PIN_ID, PIN_OUTPUT);
    GPIO_setupPinDirection(LCD_DISPLAY_DATA_PORT, LCD_DB7_PIN_ID, PIN_OUTPUT);

    // Send 4-bit initialization sequence commands
    LCD_SendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
    LCD_SendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);
    LCD_SendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);
#elif(8 == LCD_DATA_BITS_MODE)
    // Setup all data pins as output in 8-bit mode
    GPIO_setupPortDirection(LCD_DISPLAY_DATA_PORT, PORT_OUTPUT);

    // Send 8-bit init command for 2-line display
    LCD_SendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);
#endif

    // Turn off cursor and clear screen
    LCD_SendCommand(LCD_CURSOR_OFF);
    LCD_SendCommand(LCD_CLEAR_COMMAND);
}

/* Displays a single character on the LCD */
void LCD_displayCharacter(uint8 character)
{
    // RS = 1 for data mode
    GPIO_writePin(LCD_REGISTER_SELECT_PORT, LCD_REGISTER_SELECT_PIN, LOGIC_HIGH);
    _delay_ms(1);

    // Enable = 1 to start sending data
    GPIO_writePin(LCD_ENABLE_PORT, LCD_ENABLE_PIN, LOGIC_HIGH);
    _delay_ms(1);

#if(4 == LCD_DATA_BITS_MODE)
    // Send upper nibble of character
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB4_PIN_ID, GET_BIT(character,4));
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB5_PIN_ID, GET_BIT(character,5));
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB6_PIN_ID, GET_BIT(character,6));
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB7_PIN_ID, GET_BIT(character,7));
    _delay_ms(1);

    // Latch upper nibble
    GPIO_writePin(LCD_ENABLE_PORT, LCD_ENABLE_PIN, LOGIC_LOW);
    _delay_ms(1);
    GPIO_writePin(LCD_ENABLE_PORT, LCD_ENABLE_PIN, LOGIC_HIGH);
    _delay_ms(1);

    // Send lower nibble of character
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB4_PIN_ID, GET_BIT(character,0));
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB5_PIN_ID, GET_BIT(character,1));
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB6_PIN_ID, GET_BIT(character,2));
    GPIO_writePin(LCD_DISPLAY_DATA_PORT, LCD_DB7_PIN_ID, GET_BIT(character,3));
#elif(8 == LCD_DATA_BITS_MODE)
    // Send full byte in 8-bit mode
    GPIO_writePort(LCD_DISPLAY_DATA_PORT, character);
#endif

    _delay_ms(1);

    // Enable = 0 to latch data
    GPIO_writePin(LCD_ENABLE_PORT, LCD_ENABLE_PIN, LOGIC_LOW);
    _delay_ms(1);
}

/* Displays a string of characters on LCD */
void LCD_displayString(uint8* str)
{
    uint8 i = 0;
    while (*(str + i))
    {
        LCD_displayCharacter(*(str + i));
        i++;
    }
}

/* Converts integer to string and displays it */
void LCD_intgerToString(int data)
{
    char buff[16];
    itoa(data, buff, 10); // Convert integer to ASCII string in decimal
    LCD_displayString(buff);
}

/* Clears the LCD display screen */
void LCD_clearScreen(void)
{
    LCD_SendCommand(LCD_CLEAR_COMMAND);
}

/* Moves LCD cursor to specified row and column */
void LCD_moveCursor(uint8 row, uint8 col)
{
    uint8 lcd_memory_address;

    // Calculate DDRAM address based on (row, col)
    switch(row)
    {
        case 0: lcd_memory_address = col; break;
        case 1: lcd_memory_address = col + 0x40; break;
        case 2: lcd_memory_address = col + 0x10; break;
        case 3: lcd_memory_address = col + 0x50; break;
        default: lcd_memory_address = col; // default first row
    }

    // Send command to move cursor to the DDRAM address
    LCD_SendCommand(lcd_memory_address | LCD_SET_CURSOR_LOCATION);
}

/* Displays a string on specified row and column */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *Str)
{
    // Position the cursor and print the string
    LCD_moveCursor(row, col);
    LCD_displayString((uint8*)Str);
}
