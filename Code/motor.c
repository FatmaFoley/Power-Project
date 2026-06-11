/*
 * motor.c
 *
 *  Created on: Jun 7, 2026
 */

/******************************************************************************
 *                             Include Files                                   *
 ******************************************************************************/

#include "motor.h"
#include "common_macros.h"
#include <avr/delay.h>
#include "gpio.h"


void DcMotor_Init(void)
{
    /* Set motor control pins as output for Motor 1 */
    GPIO_setupPinDirection(DC_MOTOR_1_IN1_PORT_ID, DC_MOTOR_1_IN1_PIN_ID, PIN_OUTPUT);
    GPIO_setupPinDirection(DC_MOTOR_1_IN2_PORT_ID, DC_MOTOR_1_IN2_PIN_ID, PIN_OUTPUT);
    GPIO_setupPinDirection(DC_MOTOR_1_EN_PORT_ID, DC_MOTOR_1_EN_PIN_ID, PIN_OUTPUT);

    /* Stop Motor 1 by setting input pins low */
    GPIO_writePin(DC_MOTOR_1_IN1_PORT_ID, DC_MOTOR_1_IN1_PIN_ID, LOGIC_LOW);
    GPIO_writePin(DC_MOTOR_1_IN2_PORT_ID, DC_MOTOR_1_IN2_PIN_ID, LOGIC_LOW);
}

void DcMotor_Rotate(DcMotor_State fan_state)
{
    /* Set motor control pins to rotate motor or stop based on the state */
    switch(fan_state)
    {
        case CLOSE_FAN:
            /* Both control pins low to stop motor */
            {
                GPIO_writePin(DC_MOTOR_1_IN1_PORT_ID, DC_MOTOR_1_IN1_PIN_ID, LOGIC_LOW);
                GPIO_writePin(DC_MOTOR_1_IN2_PORT_ID, DC_MOTOR_1_IN2_PIN_ID, LOGIC_LOW);
            }
            break;

        case OPEN_FAN:
            /* Rotate motor Anti-Clockwise: control pin1 low, control pin2 high */
            {
                GPIO_writePin(DC_MOTOR_1_IN1_PORT_ID, DC_MOTOR_1_IN1_PIN_ID, LOGIC_LOW);
                GPIO_writePin(DC_MOTOR_1_IN2_PORT_ID, DC_MOTOR_1_IN2_PIN_ID, LOGIC_HIGH);
            }
            break;
    }
    return;
}
