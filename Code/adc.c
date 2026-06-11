/*
 * adc.c
 *
 *  Created on: Jun 7, 2026
 */

/******************************************************************************
 *                             Include Files                                   *
 ******************************************************************************/

#include "adc.h"
#include "common_macros.h"
#include "std_types.h"
#include <avr/io.h>

/*******************************************************************************
 *                          Global Variables                                   *
 *******************************************************************************/

/* Global variable to store the result of ADC conversion */
volatile uint16 g_adcResult = 0;


/*******************************************************************************
 *                          ISR's Definitions                                  *
 *******************************************************************************/

/* ISR for ADC conversion complete interrupt */
ISR(ADC_vect)
{
    /* Read ADC data after conversion is complete and store in global variable */
    g_adcResult = ADC;
}

/* Initialize ADC with the given configurations */
void ADC_init(ADC_DYNAMIC_CONVEGRATIONS * configrations)
{
    /* Enable ADC */
    ADCSRA = (1<<ADEN);

    /*
     * Select ADC reference voltage
     * Clear MUX5:0 bits (keep last 6 bits) and set reference voltage bits from config
     */
    ADMUX = (ADMUX & 0x3F) | configrations->adc_ref_volt;

    /*
     * Set the ADC clock prescaler value (last 3 bits of ADCSRA)
     * Keep the first 5 bits and update the prescaler bits from config
     */
    ADCSRA = (ADCSRA & 0xF8) | configrations->adc_prescaler_value;

    /* Disable Auto Trigger Enable (ADATE) and ADC Interrupt Enable (ADIE) */
    ADCSRA &= ~(1<<ADATE) & ~(1<<ADIE);
}

/* Read analog data from the specified ADC channel */
uint16 ADC_readChannel(uint8 ch_num)
{
    /* Ensure channel number is between 0 and 7 */
    ch_num &= 0x07;

    /*
     * Clear last 5 bits of ADMUX to select the channel,
     * then set with channel number
     */
    ADMUX &= 0xE0;
    ADMUX |= ch_num;

    /* Start ADC conversion by setting ADSC bit */
    SET_BIT(ADCSRA,ADSC);

    /* Wait until conversion is complete (ADIF bit is set) */
    while(BIT_IS_CLEAR(ADCSRA,ADIF))
    {
           /* Do Nothing - wait for interrupt flag */
    }

    /* Clear ADIF flag by writing 1 to it */
    SET_BIT(ADCSRA,ADIF);

    /* Return the ADC conversion result */
    return ADC;
}
