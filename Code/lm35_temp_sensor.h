/*
 * lm35_temp_sensor.h
 *
 *  Created on: Jun 7, 2026
 */

#ifndef LM35_TEMP_SENSOR_H_
#define LM35_TEMP_SENSOR_H_

/******************************************************************************
 *                             Include Files                                   *
 ******************************************************************************/


#include "std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* ADC channel number connected to the LM35 sensor output */
#define SENSOR_CHANNEL_ID           1

/* Maximum expected sensor output voltage in volts */
#define SENSOR_MAX_VOLT_VALUE       1.5

/* Maximum temperature the LM35 sensor can measure (degrees Celsius) */
#define SENSOR_MAX_TEMPERATURE      150

/* The port and pin ID for LM35 sensor output (analog input pin) */
#define SENSOR_VOUT_PORT_ID         PORTA_ID
#define SENSOR_VOUT_PIN_ID          PIN1_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Initialize the LM35 sensor input pin as ADC input */
void LM35_init(void);

/* Read the temperature from LM35 sensor and return the temperature in Celsius degrees */
uint8 LM35_getTemperature(void);

#endif /* LM35_TEMP_SENSOR_H_ */
