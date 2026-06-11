/*
 * buzzer.c
 *
 *  Created on: Jun 7, 2026
 */

/******************************************************************************
 *                             Include Files                                   *
 ******************************************************************************/

#include "buzzer.h"
#include "gpio.h"

/* Initialize the buzzer pin as output and turn it off initially */
void Buzzer_init(void)
{
    GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
    GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_OFF);
}

/* Turn the buzzer on by setting the respective pin high */
void Buzzer_on(void)
{
    GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_ON);
}

/* Turn the buzzer off by setting the respective pin low */
void Buzzer_off(void)
{
    GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_OFF);
}
