/*
 * lm35_temp_sensor.c
 *
 *  Created on: Jun 7, 2026
 */

/******************************************************************************
 *                             Include Files                                   *
 ******************************************************************************/


#include "lm35_temp_sensor.h"
#include "adc.h"
#include "gpio.h"
#include <avr/delay.h>

/*
 * Function: LM35_init
 * -------------------
 * Configures the LM35 sensor output pin as analog input pin to ADC.
 * Usually the LM35 output is connected to an ADC channel of the MCU.
 */
void LM35_init(void)
{
	GPIO_setupPinDirection(SENSOR_VOUT_PORT_ID, SENSOR_VOUT_PIN_ID, PIN_INPUT);
	ADC_DYNAMIC_CONVEGRATIONS configrations ={AVCC_with_external_capacitor_at_AREF_pin, PRESCALER_128};
	ADC_init(&configrations);
	_delay_ms(500);

}

/*
 * Function: LM35_getTemperature
 * -----------------------------
 * Reads the ADC digital value from the LM35 output channel, converts this ADC value to
 * temperature in Celsius using calibration constants and returns the temperature value.
 *
 * The LM35 outputs 10mV per degree Celsius, so the voltage converted from ADC reading
 * is scaled accordingly to get temperature.
 *
 * Returns:
 *   8-bit unsigned integer representing temperature in Celsius degrees.
 */
uint8 LM35_getTemperature(void)
{
	uint8 temp_value = 0;

	// Read the ADC value from the configured sensor channel
	g_adcResult = ADC_readChannel(SENSOR_CHANNEL_ID);

	_delay_ms(5); // Delay to stabilize ADC reading (optional)

	/*
	 * Convert the ADC digital reading to temperature in Celsius.
	 * This formula uses:
	 * - g_adcResult: raw ADC digital value
	 * - SENSOR_MAX_TEMPERATURE: maximum temperature sensor can measure (calibration)
	 * - ADC_REF_VOLT_VALUE: ADC reference voltage (e.g., 5V or 2.56V)
	 * - ADC_MAXIMUM_VALUE: max ADC digital value (1023 for 10-bit ADC)
	 * - SENSOR_MAX_VOLT_VALUE: maximum voltage sensor outputs at max temperature
	 */
	temp_value = (uint8)(((uint32)g_adcResult * SENSOR_MAX_TEMPERATURE * ADC_REF_VOLT_VALUE) /
			(ADC_MAXIMUM_VALUE * SENSOR_MAX_VOLT_VALUE));

	return temp_value;
}
