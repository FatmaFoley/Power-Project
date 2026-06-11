/*
 * pwm.h
 *
 *  Created on: Jun 7, 2026
 */

#ifndef PWM_H_
#define PWM_H_

/******************************************************************************
 *                             Include Files                                   *
 ******************************************************************************/


#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Timer identifier (only timer0 used here) */
#define PWM_TIMER_ID                0

/* Port and pin definitions for PWM output channel OC0 (Pin PB3) */
#define PWM_CHANNEL_REG_ID          PORTB_ID
#define PWM_CHANNEL_PIN_ID          PIN3_ID  // OC0 pin on AVR

/* PWM output compare register for Timer0 */
#define PWM_OUTPUT                  OCR0

/*******************************************************************************
 *                           Functions Prototypes                             *
 *******************************************************************************/

/*
 * Initializes Timer0 to generate PWM signal using Fast PWM mode with
 * non-inverting output on OC0 pin (PB3).
 *
 * Input:
 *   duty_cycle - PWM duty cycle percentage (0-100).
 *
 * The PWM frequency is based on the timer clock and prescaler (here prescaler=1024).
 */
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_H_ */
