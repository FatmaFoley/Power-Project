/*
 * keypad.h
 *
 *  Created on: Jun 7, 2026
 */


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

// Port and pin definitions for keypad rows (4 rows)
#define KEYPAD_ROWs_PORT_ID              PORTB_ID
#define KEYPAD_ROW_ONE_PIN_ID            PIN0_ID  // First row pin ID

// Port and pin definitions for keypad columns (4 columns)
#define KEYPAD_COLUMNS_PORT_ID           PORTD_ID
#define KEYPAD_COLUMN_ONE_PIN_ID         PIN2_ID  // First column pin ID

// Keypad dimensions
#define KEYPAD_NUM_OF_ROWS               4
#define KEYPAD_NUM_OF_COLUMNS            4

// Active logic levels for pressed and released keys
#define KEYPAD_PRESSED_BUTTON            LOGIC_LOW   // Key pressed state (active low)
#define KEYPAD_RELEASED_BUTTON           LOGIC_HIGH  // Key released state

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

uint8 KEYPAD_getPressedKey(void);

#endif /* KEYPAD_H_ */
