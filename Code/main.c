/*
 ================================================================================================
 Name        : main.c
 Description : Controller Code
 Date        : 10/6/2026
 ================================================================================================
 */

#define F_CPU 8000000UL

#include "lm35_temp_sensor.h"
#include "ultrasonic_sensor.h"
#include "external_eeprom.h"
#include "common_macros.h"
#include "motor.h"
#include "twi.h"
#include "timer.h"
#include "lcd.h"
#include "buzzer.h"
#include "keypad.h"
#include <util/delay.h>
#include <avr/io.h>

#define ERROR_TEMP_HIGH_ADDR  0x10  // EEPROM address for temperature error counter
#define ERROR_DIST_LOW_ADDR   0x20  // EEPROM address for distance error counter

uint8 g_tick = 0;

/* Timer0 callback function increments global tick counter */
void Timer0_callback_fun(void) {
	g_tick++;
}

int main(void) {
	/* Variable declarations and initialization */
	uint8 key = 0, temp_prev = 0, temp = 0;
	uint8 P001_Dist_error_counter = 0, P002_Temp_error_counter = 0, repeat = 1;
	uint8 eeprom_data = 0, check = 0, faults_buffer[2] = { 0, 0 };
	uint16 distance_prev = 0, distance = 50;
	DcMotor_State fan_state = CLOSE_FAN;

	/* Initialize sensors, motor, and TWI */
	LM35_init();
	Ultrasonic_init();
	DcMotor_Init();
	Buzzer_init();
	LCD_init();
	TWI_ConfigType twi_settings = { 0x01, 400000 };

	/* Initialize TWI (I2C) */
	TWI_init(&twi_settings);

	/* Timer1 configuration for 1-second intervals (with prescaler 1024 and compare value 15625) */
	Timer_ConfigType Config_Ptr =
			{ 0, 250, TIMER0, PRESCALER_1024, COMPARE_MODE };

	/* Enable global interrupts */
	SREG |= (1 << 7);

	while (1) {
		/* Display main menu on LCD */
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "1.StartOperation");
		LCD_displayStringRowColumn(1, 0, "2.Display Values");
		LCD_displayStringRowColumn(2, 0, "3.RetrieveFaults");
		LCD_displayStringRowColumn(3, 0, "4.StopMonitoring");

		/* Get keypad input */
		key = KEYPAD_getPressedKey();

		switch (key) {
		case 1:
			LCD_clearScreen();
			g_tick = 0;

			/* Initialize timer and set callback */
			Timer_init(&Config_Ptr);
			Timer_setCallBack(Timer0_callback_fun, TIMER0);

			LCD_displayStringRowColumn(0, 0, "OperationStarted");
			LCD_displayStringRowColumn(1, 0, "MonitoringActive");
			/* In case 1: collect sensor data and update error counters */
			while (g_tick < 40) {

				/* Read current temperature and distance */
				temp = LM35_getTemperature();
				distance = Ultrasonic_readDistance();

				//Read current error counters from EEPROM
				EEPROM_readByte(ERROR_DIST_LOW_ADDR, &eeprom_data);
				_delay_ms(10);
				P001_Dist_error_counter = eeprom_data;
				EEPROM_readByte(ERROR_TEMP_HIGH_ADDR, &eeprom_data);
				_delay_ms(10);
				P002_Temp_error_counter = eeprom_data;

				//If temperature exceeds 60 and has changed, increment temp error counter
				if (temp > 60 && temp != temp_prev) {
					P002_Temp_error_counter++;
					EEPROM_writeByte(ERROR_TEMP_HIGH_ADDR,
							P002_Temp_error_counter);
					_delay_ms(10);
				} else

				//If distance less than 10 and changed, increment distance error counter
				if (distance < 10 && distance != distance_prev) {
					P001_Dist_error_counter++;
					EEPROM_writeByte(ERROR_DIST_LOW_ADDR,
							P001_Dist_error_counter);
					_delay_ms(10);
				}

				if (temp > 60)
					fan_state = OPEN_FAN;
				else
					fan_state = CLOSE_FAN;
				DcMotor_Rotate(fan_state);

				if (distance < 10)
					Buzzer_on();
				else
					Buzzer_off();

				//Save previous values for next comparison
				temp_prev = temp;
				distance_prev = distance;
			}
			/* Deinitialize timer */
			Timer_deInit(TIMER0);
			break;

		case 2:
			/* Case 2: continuous monitoring and controlling motor state */
			repeat = 1;
			while (repeat) {
				LCD_clearScreen();
				g_tick = 0;

				Timer_init(&Config_Ptr);
				Timer_setCallBack(Timer0_callback_fun, TIMER0);

				/* Display sensor value labels */
				LCD_displayStringRowColumn(0, 0, "Temp = ");
				LCD_displayStringRowColumn(0, 11, " C");
				LCD_displayStringRowColumn(1, 0, "Dist = ");
				LCD_displayStringRowColumn(1, 10, " cm");

				while (g_tick < 50) {

					// Get latest temperature and distance
					temp = LM35_getTemperature();
					distance = Ultrasonic_readDistance();

					//Display temperature
					LCD_moveCursor(0, 7);
					LCD_intgerToString(temp);

					//Display distance
					LCD_moveCursor(1, 7);
					LCD_intgerToString(distance);
					LCD_displayStringRowColumn(2, 0, "FAN:Close");

					//Clear extra digits if temp or distance less than certain thresholds
					if ((temp >= 10) && (temp < 100)) {
						LCD_displayStringRowColumn(0, 9, " ");
					} else if (temp < 10) {
						LCD_displayStringRowColumn(0, 8, "  ");
					}
					if ((distance >= 10) && (distance < 100)) {
						LCD_displayStringRowColumn(1, 9, " ");
					} else if (distance < 10) {
						LCD_displayStringRowColumn(1, 8, "  ");
					}

					if (distance < 10)
						Buzzer_on();
					else
						Buzzer_off();

					// Update error counters if thresholds exceeded and values changed
					if (temp > 60 && temp != temp_prev) {
						P002_Temp_error_counter++;
						EEPROM_writeByte(ERROR_TEMP_HIGH_ADDR,
								P002_Temp_error_counter);
						_delay_ms(10);
					}
					if (distance < 10 && distance != distance_prev) {
						P001_Dist_error_counter++;
						EEPROM_writeByte(ERROR_DIST_LOW_ADDR,
								P001_Dist_error_counter);
						_delay_ms(10);
					}

					if (temp > 60)
						fan_state = OPEN_FAN;
					else
						fan_state = CLOSE_FAN;
					DcMotor_Rotate(fan_state);

					//Display Fan state
					switch (fan_state) {
					case OPEN_FAN:
						LCD_displayStringRowColumn(2, 0, "FAN:Open");
						LCD_displayStringRowColumn(2, 8, " ");
						break;
					case CLOSE_FAN:
						LCD_displayStringRowColumn(2, 0, "FAN:Close");
						break;
					}

					/* Update previous values */
					temp_prev = temp;
					distance_prev = distance;
				}
				/* Deinitialize timer */
				Timer_deInit(TIMER0);
				LCD_clearScreen();

				/* Ask user if they want to display again */
				LCD_displayStringRowColumn(0, 0, "Display again?");
				LCD_displayStringRowColumn(1, 0, "Press 2 = YES");
				LCD_displayStringRowColumn(2, 0, "Other key = MAIN MENU");

				key = KEYPAD_getPressedKey();
				_delay_ms(50);

				if (2 == key) {
					repeat = 1;
				} else {
					repeat = 0;
				}

			}
			g_tick = 0;
			break;

		case 3:
			/* Case 3: send the error counters and basic sensors continuously */
			g_tick = 0;
			repeat = 1;

			while (repeat) {
				/* Initialize timer and set callback */
				Timer_init(&Config_Ptr);
				Timer_setCallBack(Timer0_callback_fun, TIMER0);

				//Display Faults
				LCD_clearScreen();
				LCD_displayStringRowColumn(0, 0, "Logged Faults:");
				LCD_displayStringRowColumn(1, 0, "P001:");
				LCD_displayStringRowColumn(2, 0, "P002:");
				LCD_displayStringRowColumn(3, 0, "--End of List--");
				g_tick = 0;
				while (g_tick < 40) {

					/* Read error counters from EEPROM */
					EEPROM_readByte(ERROR_DIST_LOW_ADDR, &eeprom_data);
					_delay_ms(10);
					faults_buffer[0] = eeprom_data;

					EEPROM_readByte(ERROR_TEMP_HIGH_ADDR, &eeprom_data);
					_delay_ms(10);
					faults_buffer[1] = eeprom_data;

					/* Read sensors */
					temp = LM35_getTemperature();
					if (temp >= 60)
						fan_state = OPEN_FAN;
					else
						fan_state = CLOSE_FAN;
					DcMotor_Rotate(fan_state);

					distance = Ultrasonic_readDistance();

					/* Update error counters for threshold breaches */
					if (temp > 60 && temp != temp_prev) {
						P002_Temp_error_counter++;
						EEPROM_writeByte(ERROR_TEMP_HIGH_ADDR,
								P002_Temp_error_counter);
						_delay_ms(10);
					}
					if (distance < 10 && distance != distance_prev) {
						P001_Dist_error_counter++;
						EEPROM_writeByte(ERROR_DIST_LOW_ADDR,
								P001_Dist_error_counter);
						_delay_ms(10);
					}

					/* Update previous sensor values */
					temp_prev = temp;
					distance_prev = distance;

				/* Display faults on LCD */
				LCD_moveCursor(1, 6);
				LCD_intgerToString(P001_Dist_error_counter);
				LCD_moveCursor(2, 6);
				LCD_intgerToString(P002_Temp_error_counter);

				/* Clear extra spaces for fault count display */
				if (P001_Dist_error_counter < 10) {
					LCD_displayStringRowColumn(1, 7, "   ");
				} else if (P001_Dist_error_counter > 100) {
					LCD_displayStringRowColumn(1, 8, " ");
				}
				if (P002_Temp_error_counter < 10) {
					LCD_displayStringRowColumn(2, 7, "   ");
				} else if (P002_Temp_error_counter > 100) {
					LCD_displayStringRowColumn(2, 8, " ");
				}
				}

			Timer_deInit(TIMER0);

			LCD_clearScreen();
			/* Ask user if they want to display again */
			LCD_displayStringRowColumn(0, 0, "Display again?");
			LCD_displayStringRowColumn(1, 0, "Press 3 = YES");
			LCD_displayStringRowColumn(2, 0, "Other key = MAIN MENU");

			key = KEYPAD_getPressedKey();
			_delay_ms(50);

			if (3 == key) {
				repeat = 1;
			} else {
				repeat = 0;
			}
		}

		g_tick = 0;
		break;

		case 4:
		/* Case 4: Reset error counters and sensor previous values to zero */
		P001_Dist_error_counter = 0;
		P002_Temp_error_counter = 0;
		temp_prev = 0;
		distance_prev = 0;

		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "SystemMonitoring");
		LCD_displayStringRowColumn(1, 0, "Stopped!");
		LCD_displayStringRowColumn(2, 0, "ReturningToMenu.");
		_delay_ms(300);

		/* Write zero to EEPROM error counter addresses */
		EEPROM_writeByte(ERROR_TEMP_HIGH_ADDR, 0);
		_delay_ms(10);
		EEPROM_writeByte(ERROR_DIST_LOW_ADDR, 0);
		_delay_ms(10);
		break;
	}
}
}
