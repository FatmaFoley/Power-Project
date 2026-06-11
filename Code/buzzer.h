/*
 * buzzer.h
 *
 *  Created on: Jun 7, 2026
 */

#ifndef BUZZER_H_
#define BUZZER_H_

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Define the port ID for buzzer (e.g., PORTD) */
#define BUZZER_PORT_ID       PORTD_ID

/* Define the pin ID for buzzer (e.g., PIN3) */
#define BUZZER_PIN_ID        PIN0_ID

/* Define logic level to turn the buzzer off */
#define BUZZER_OFF           LOGIC_LOW

/* Define logic level to turn the buzzer on */
#define BUZZER_ON            LOGIC_HIGH

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/* Initialize the buzzer pin as output and turn off buzzer */
void Buzzer_init(void);

/* Turn the buzzer on */
void Buzzer_on(void);

/* Turn the buzzer off */
void Buzzer_off(void);

#endif /* BUZZER_H_ */
