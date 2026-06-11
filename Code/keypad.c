/*
 * keypad.c
 *
 *  Created on: Jun 7, 2026
 */

#include "gpio.h"
#include "std_types.h"
#include <avr/delay.h>
#include "keypad.h"

#if(3 == KEYPAD_NUM_OF_COLUMNS)
/*
 * Description:
 * Adjusts the returned key number to actual character for 4x3 keypad.
 */
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number)
{
    uint8 keypad_button = 0;
    switch(button_number)
    {
        case 10: keypad_button = '*';  // ASCII '*'
            break;
        case 11: keypad_button = 0;
            break;
        case 12: keypad_button = '#';  // ASCII '#'
            break;
        default: keypad_button = button_number;
            break;
    }
    return keypad_button;
}

#elif(4 == KEYPAD_NUM_OF_COLUMNS)
/*
 * Description:
 * Adjusts the returned key number to actual character for 4x4 keypad.
 */
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number)
{
    uint8 keypad_button = 0;
    switch(button_number)
    {
        case 1: keypad_button = 7; break;
        case 2: keypad_button = 8; break;
        case 3: keypad_button = 9; break;
        case 4: keypad_button = '%';  // ASCII '%'
            break;
        case 5: keypad_button = 4; break;
        case 6: keypad_button = 5; break;
        case 7: keypad_button = 6; break;
        case 8: keypad_button = '*';  // ASCII '*'
            break;
        case 9: keypad_button = 1; break;
        case 10: keypad_button = 2; break;
        case 11: keypad_button = 3; break;
        case 12: keypad_button = '-';  // ASCII '-'
            break;
        case 13: keypad_button = 13;   // ASCII Enter (Carriage Return)
            break;
        case 14: keypad_button = 0; break;
        case 15: keypad_button = '=';  // ASCII '='
            break;
        case 16: keypad_button = '+';  // ASCII '+'
            break;
        default: keypad_button = button_number;
            break;
    }
    return keypad_button;
}
#endif

// Function to get the pressed key from the keypad matrix (polling)
uint8 KEYPAD_getPressedKey(void)
{
    uint8 rows_counter, columns_counter;

    // Set all row pins initially as inputs (high impedance)
    for(rows_counter = 0; rows_counter < KEYPAD_NUM_OF_ROWS; rows_counter++)
    {
        GPIO_setupPinDirection(KEYPAD_ROWs_PORT_ID, KEYPAD_ROW_ONE_PIN_ID + rows_counter, PIN_INPUT);
    }

    // Set all column pins initially as inputs
    for(columns_counter = 0; columns_counter < KEYPAD_NUM_OF_COLUMNS; columns_counter++)
    {
        GPIO_setupPinDirection(KEYPAD_COLUMNS_PORT_ID, KEYPAD_COLUMN_ONE_PIN_ID + columns_counter, PIN_INPUT);
    }

    rows_counter = 0;
    columns_counter = 0;

    // Continuously scan keypad matrix for a pressed key
    while(1)
    {
        // Scan each row one by one
        for(rows_counter = 0; rows_counter < KEYPAD_NUM_OF_ROWS; rows_counter++)
        {
            // Set the current row pin as output and drive it with the pressed button signal
            GPIO_setupPinDirection(KEYPAD_ROWs_PORT_ID, KEYPAD_ROW_ONE_PIN_ID + rows_counter, PIN_OUTPUT);
            GPIO_writePin(KEYPAD_ROWs_PORT_ID, KEYPAD_ROW_ONE_PIN_ID + rows_counter, KEYPAD_PRESSED_BUTTON);

            // Check all columns to see if any key in current row is pressed
            for(columns_counter = 0; columns_counter < KEYPAD_NUM_OF_COLUMNS; columns_counter++)
            {
                if(GPIO_readPin(KEYPAD_COLUMNS_PORT_ID, KEYPAD_COLUMN_ONE_PIN_ID + columns_counter) == KEYPAD_PRESSED_BUTTON)
                {
                    // Calculate pressed button index and adjust according to keypad layout and columns count
#if(3 == KEYPAD_NUM_OF_COLUMNS)
                    return KEYPAD_4x3_adjustKeyNumber((rows_counter * KEYPAD_NUM_OF_COLUMNS) + columns_counter + 1);
#elif(4 == KEYPAD_NUM_OF_COLUMNS)
                    return KEYPAD_4x4_adjustKeyNumber((rows_counter * KEYPAD_NUM_OF_COLUMNS) + columns_counter + 1);
#endif
                }
            }

            // Reset current row pin to input, disabling its drive signal
            GPIO_setupPinDirection(KEYPAD_ROWs_PORT_ID, KEYPAD_ROW_ONE_PIN_ID + rows_counter, PIN_INPUT);

            _delay_ms(10); /* Delay to reduce CPU load in simulation/emulation */
        }
    }
}



