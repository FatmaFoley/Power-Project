/*
 * lcd.h
 *
 *  Created on: Jun 7, 2026
 */


#ifndef LCD_H_
#define LCD_H_

/******************************************************************************
 *                             Include Files                                   *
 ******************************************************************************/

#include "std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Define LCD data mode: must be either 4 or 8 bits */
#define LCD_DATA_BITS_MODE           4

/* Compile-time check for valid LCD data mode */
#if((LCD_DATA_BITS_MODE != 4) && (LCD_DATA_BITS_MODE != 8))
#error "Number of Data bits should be equal to 4 or 8"
#endif

/* Define ports and pin IDs for LCD control signals */
#define LCD_REGISTER_SELECT_PORT     PORTA_ID
#define LCD_REGISTER_SELECT_PIN      PIN2_ID
#define LCD_ENABLE_PORT              PORTA_ID
#define LCD_ENABLE_PIN               PIN7_ID

/* Define the data port for LCD data pins */
#define LCD_DISPLAY_DATA_PORT        PORTA_ID

/* Define individual data pins when using 4-bit mode */
#if (LCD_DATA_BITS_MODE == 4)
#define LCD_DB4_PIN_ID               PIN3_ID
#define LCD_DB5_PIN_ID               PIN4_ID
#define LCD_DB6_PIN_ID               PIN5_ID
#define LCD_DB7_PIN_ID               PIN6_ID
#endif

/* LCD command codes for control operations */
#define LCD_CLEAR_COMMAND                    0x01
#define LCD_GO_TO_HOME                       0x02
#define LCD_TWO_LINES_EIGHT_BITS_MODE        0x38
#define LCD_TWO_LINES_FOUR_BITS_MODE         0x28
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   0x32
#define LCD_CURSOR_OFF                       0x0C
#define LCD_CURSOR_ON                        0x0E
#define LCD_SET_CURSOR_LOCATION              0x80

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Send a command byte to LCD */
void LCD_SendCommand(uint8 command);

/* Initialize LCD with configured data mode */
void LCD_init(void);

/* Display single character on LCD */
void LCD_displayCharacter(uint8 character);

/* Display string on LCD */
void LCD_displayString(uint8* str);

/* Convert integer to string and display it */
void LCD_intgerToString(int data);

/* Clear the LCD screen */
void LCD_clearScreen(void);

/* Move LCD cursor to specific row and column */
void LCD_moveCursor(uint8 row, uint8 col);

/* Display a string at specified row and column */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *Str);

#endif /* LCD_H_ */
