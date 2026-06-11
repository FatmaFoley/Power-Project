/*
 * pwm.c
 *
 *  Created on: Jun 7, 2026
 */

/******************************************************************************
 *                             Include Files                                   *
 ******************************************************************************/


#include "pwm.h"
#include "common_macros.h"
#include <avr/io.h>
#include "gpio.h"

/*
 * Function: PWM_Timer0_Start
 * --------------------------
 * Initializes Timer0 to generate PWM signal in Fast PWM mode on OC0 (PB3) pin.
 * Configures Timer0 under:
 *  - Fast PWM mode (WGM00=1, WGM01=1)
 *  - Non-inverting mode (COM01=1, COM00=0)
 *  - Clock prescaler = 1024 (CS00=1, CS02=1, CS01=0)
 *
 * The duty cycle argument (0-100) sets the compare value of OCR0 register.
 * The OC0 pin is configured as output for PWM signal generation.
 *
 * duty_cycle: Duty cycle percentage (0-100) for PWM output.
 */
void PWM_Timer0_Start(uint8 duty_cycle)
{
    /* Set PB3 (OC0 pin) as output pin */
    DDRB |= (1 << PB3);

    /* Set Timer0 to Fast PWM mode: WGM00=1, WGM01=1 */
    SET_BIT(TCCR0, WGM00);
    SET_BIT(TCCR0, WGM01);

    /* Calculate and set OCR0 value based on duty cycle (0-255) */
    PWM_OUTPUT = (uint8)((uint16)(duty_cycle * 255) / 100);  /* 255 = (2^8)-1 */

    /* Set clock prescaler to 1024: CS00=1, CS01=0, CS02=1 */
    SET_BIT(TCCR0, CS00);
    CLEAR_BIT(TCCR0, CS01);
    SET_BIT(TCCR0, CS02);

    /* Set non-inverting mode: COM01=1, COM00=0 */
    SET_BIT(TCCR0, COM01);
    CLEAR_BIT(TCCR0, COM00);

    /* Configure the OC0 pin as output (ensures PWM output on this pin) */
    GPIO_setupPinDirection(PWM_CHANNEL_REG_ID, PWM_CHANNEL_PIN_ID, PIN_OUTPUT);
}
