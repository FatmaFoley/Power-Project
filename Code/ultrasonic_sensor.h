/*
 * ultrasonic_sensor.h
 *
 *  Created on: Jun 7, 2026
 */

#ifndef ULTRASONIC_SENSOR_H_
#define ULTRASONIC_SENSOR_H_

#include "std_types.h"

// Global variable to hold the measured distance value
extern uint16 g_distance;

// Definitions for the ultrasonic sensor trigger and echo pins and ports
#define ULTRASONIC_TRIG_PORT_ID        PORTD_ID
#define ULTRASONIC_TRIG_PIN_ID         PIN7_ID
#define ULTRASONIC_ECO_PORT_ID         PORTD_ID
#define ULTRASONIC_ECO_PIN_ID          PIN6_ID


// Function prototypes for ultrasonic sensor operations


/*******************************************************************************
*                      Functions Prototypes                                   *
*******************************************************************************/
/*
* Description :
* Initialize ICU configuration.
* SetCall back function.
* Set PINB5 as output pin, with initial value: LOGIC_LOW.
*/
void Ultrasonic_init(void);

/*
* Description :
* Call back function handling ICU interrupts.
* Updates the value of first_high from rising to falling edge.
*/
void Ultrasonic_edgeProcessing(void);

/*
* Description :
* This function is used to generate a 10us trigger.
* Set PORTB5 as LOGIC_HIGH , then delay , then set PORTB5 as LOGIC_LOW.
*
*/
void Ultrasonic_Trigger(void);

/*
* Description :
* Calls the trigger function to generate a 10us pulse.
* Polling until the value of the first_high is updated.
* distance = first_high /58.8.
* return distance.
*/
uint16 Ultrasonic_readDistance(void);


#endif /* ULTRASONIC_SENSOR_H_ */
