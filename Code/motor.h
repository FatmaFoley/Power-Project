/*
 * motor.h
 *
 *  Created on: Jun 7, 2026
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

/******************************************************************************
 *                             Include Files                                   *
 ******************************************************************************/

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/*
 * Define the port and pin IDs for Motor 1 control pins and buttons
 */
#define DC_MOTOR_1_IN1_PORT_ID                    PORTA_ID
#define DC_MOTOR_1_IN1_PIN_ID                     PIN0_ID
#define DC_MOTOR_1_IN2_PORT_ID                    PORTD_ID
#define DC_MOTOR_1_IN2_PIN_ID                     PIN1_ID
#define DC_MOTOR_1_EN_PORT_ID                     PORTC_ID
#define DC_MOTOR_1_EN_PIN_ID                      PIN7_ID

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

/* Enum for motor rotation states */
typedef enum {
    OPEN_FAN,    /* Rotate motor to open the fan */
    CLOSE_FAN,   /* Stop motor to close the fan */
} DcMotor_State;


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

void DcMotor_Init(void);

void DcMotor_Rotate(DcMotor_State fan_state);

#endif /* DC_MOTOR_H_ */
