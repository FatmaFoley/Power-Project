/*
 * timer.h
 *
 *  Created on: Jun 7, 2026
 */


#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

/* Types to identify timers */
typedef enum {
    TIMER0,
    TIMER1,
    TIMER2
} Timer_ID_Type;

/* Timer clock prescaler selections */
typedef enum {
    NO_CLOCK,
    NO_PRESCALER,
    PRESCALER_8,
    PRESCALER_64,
    PRESCALER_256,
    PRESCALER_1024,
    EXT_CLOCK_FALLING,
    EXT_CLOCK_RISING
} Timer_ClockType;

/* Timer mode */
typedef enum {
    NORMAL_MODE,
    COMPARE_MODE
} Timer_ModeType;

/* Configuration structure */
typedef struct {
    uint16_t timer_InitialValue;
    uint16_t timer_compare_MatchValue; /* Used only in compare mode */
    Timer_ID_Type timer_ID;
    Timer_ClockType timer_clock;
    Timer_ModeType timer_mode;
} Timer_ConfigType;

/* Function prototypes */
void Timer_init(const Timer_ConfigType * Config_Ptr);
void Timer_deInit(Timer_ID_Type timer_type);
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID);

#endif /* TIMER_H_ */
