/*
 * gpio.c
 *
 *  Created on: Jun 7, 2026
 */

/******************************************************************************
 *                             Include Files                                   *
 ******************************************************************************/

#include "gpio.h"
#include "common_macros.h" /* Macros like SET_BIT, CLEAR_BIT */
#include "avr/io.h"       /* Access to AVR IO registers */

// Setup pin direction as input or output with input validation
void GPIO_setupPinDirection(uint8 port_num, uint8 pin_num, GPIO_PinDirectionType direction)
{
    // Validate port and pin numbers
    if(port_num >= NUM_OF_PORTS || pin_num >= NUM_OF_PINS_PER_PORT)
    {
        /* Invalid port or pin: do nothing */
    }
    else
    {
        // Set or clear appropriate bit in DDRx register based on port
        switch(port_num)
        {
            case PORTA_ID:
                if(direction == PIN_OUTPUT) SET_BIT(DDRA, pin_num);
                else CLEAR_BIT(DDRA, pin_num);
                break;
            case PORTB_ID:
                if(direction == PIN_OUTPUT) SET_BIT(DDRB, pin_num);
                else CLEAR_BIT(DDRB, pin_num);
                break;
            case PORTC_ID:
                if(direction == PIN_OUTPUT) SET_BIT(DDRC, pin_num);
                else CLEAR_BIT(DDRC, pin_num);
                break;
            case PORTD_ID:
                if(direction == PIN_OUTPUT) SET_BIT(DDRD, pin_num);
                else CLEAR_BIT(DDRD, pin_num);
                break;
        }
    }
}

// Write logic high or low on a specific pin, enabling pull-up resistor if input pin
void GPIO_writePin(uint8 port_num, uint8 pin_num, uint8 value)
{
    if(port_num >= NUM_OF_PORTS || pin_num >= NUM_OF_PINS_PER_PORT)
    {
        /* Invalid port or pin: do nothing */
    }
    else
    {
        // Write given logic level value to the pin of the specified port
        switch(port_num)
        {
            case PORTA_ID:
                if(value) SET_BIT(PORTA, pin_num);
                else CLEAR_BIT(PORTA, pin_num);
                break;
            case PORTB_ID:
                if(value) SET_BIT(PORTB, pin_num);
                else CLEAR_BIT(PORTB, pin_num);
                break;
            case PORTC_ID:
                if(value) SET_BIT(PORTC, pin_num);
                else CLEAR_BIT(PORTC, pin_num);
                break;
            case PORTD_ID:
                if(value) SET_BIT(PORTD, pin_num);
                else CLEAR_BIT(PORTD, pin_num);
                break;
        }
    }
}

// Read the logic level of a pin, return LOGIC_HIGH or LOGIC_LOW
uint8 GPIO_readPin(uint8 port_num, uint8 pin_num)
{
    if(port_num >= NUM_OF_PORTS || pin_num >= NUM_OF_PINS_PER_PORT)
    {
        return LOGIC_LOW; // Default to low on invalid input
    }
    else
    {
        // Read the pin’s logic level from PINx register for the specified port
        switch(port_num)
        {
            case PORTA_ID: return BIT_IS_SET(PINA, pin_num) ? LOGIC_HIGH : LOGIC_LOW;
            case PORTB_ID: return BIT_IS_SET(PINB, pin_num) ? LOGIC_HIGH : LOGIC_LOW;
            case PORTC_ID: return BIT_IS_SET(PINC, pin_num) ? LOGIC_HIGH : LOGIC_LOW;
            case PORTD_ID: return BIT_IS_SET(PIND, pin_num) ? LOGIC_HIGH : LOGIC_LOW;
            default:       return LOGIC_LOW;
        }
    }
}

// Setup direction (input or output) for all pins in a port
void GPIO_setupPortDirection(uint8 port_num, GPIO_PortDirectionType direction)
{
    if(port_num >= NUM_OF_PORTS)
    {
        /* Invalid port: do nothing */
    }
    else
    {
        // Write the entire 8-bit direction value to DDRx register
        switch(port_num)
        {
            case PORTA_ID: DDRA = direction; break;
            case PORTB_ID: DDRB = direction; break;
            case PORTC_ID: DDRC = direction; break;
            case PORTD_ID: DDRD = direction; break;
        }
    }
}

// Write 8-bit value to entire port, affecting outputs and pull-ups on inputs
void GPIO_writePort(uint8 port_num, uint8 value)
{
    if(port_num >= NUM_OF_PORTS)
    {
        /* Invalid port: do nothing */
    }
    else
    {
        // Write 8-bit value to PORTx register to set pins high/low or enable pullups
        switch(port_num)
        {
            case PORTA_ID: PORTA = value; break;
            case PORTB_ID: PORTB = value; break;
            case PORTC_ID: PORTC = value; break;
            case PORTD_ID: PORTD = value; break;
        }
    }
}

// Read 8-bit value from entire port
uint8 GPIO_readPort(uint8 port_num)
{
    if(port_num >= NUM_OF_PORTS)
    {
        return 0; // Return 0 on invalid port
    }
    else
    {
        // Return the entire 8-bit PINx register value for the specified port
        switch(port_num)
        {
            case PORTA_ID: return PINA;
            case PORTB_ID: return PINB;
            case PORTC_ID: return PINC;
            case PORTD_ID: return PIND;
            default:       return 0;
        }
    }
}
