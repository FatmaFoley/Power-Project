/*
 * adc.h
 *
 *  Created on: Jun 7, 2026
 */

#ifndef ADC_H_
#define ADC_H_

/******************************************************************************
 *                             Include Files                                   *
 ******************************************************************************/

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Maximum ADC digital value for 10-bit ADC resolution */
#define ADC_MAXIMUM_VALUE    1023

/* Reference voltage value used by ADC in volts */
#define ADC_REF_VOLT_VALUE   5

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern declaration of the global variable that stores ADC conversion result */
extern volatile uint16 g_adcResult;

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

/* Enumeration for ADC reference voltage selection */
typedef enum {
    AREF_Internal_Vref_turned_off,         /* External AREF pin, internal ref off */
    AVCC_with_external_capacitor_at_AREF_pin, /* AVCC with external capacitor at AREF pin */
    Reserved,                              /* Reserved value */
    Internal_2_56V                        /* Internal 2.56V reference */
} ADC_REF_VOLTAGE;

/* Enumeration for ADC clock prescaler options */
typedef enum {
    PRESCALER_1,    /* No prescaling */
    PRESCALER_2,    /* Divide clock by 2 */
    PRESCALER_4,    /* Divide clock by 4 */
    PRESCALER_8,    /* Divide clock by 8 */
    PRESCALER_16,   /* Divide clock by 16 */
    PRESCALER_32,   /* Divide clock by 32 */
    PRESCALER_64,   /* Divide clock by 64 */
    PRESCALER_128   /* Divide clock by 128 */
} ADC_PRESCALER;

/* Structure to hold dynamic ADC configuration parameters */
typedef struct ADC_DYNAMIC_CONVEGRATION
{
    ADC_REF_VOLTAGE adc_ref_volt;          /* Reference voltage selection */
    ADC_PRESCALER adc_prescaler_value;     /* ADC clock prescaler value */
} ADC_DYNAMIC_CONVEGRATIONS;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/* Initialize the ADC according to the received configurations */
void ADC_init(ADC_DYNAMIC_CONVEGRATIONS * configrations);

/* Read the analog data from a specific ADC channel */
uint16 ADC_readChannel(uint8 ch_num);

#endif /* ADC_H_ */
