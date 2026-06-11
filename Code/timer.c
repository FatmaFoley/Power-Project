/*
 * timer.c
 *
 *  Created on: Jun 7, 2026
 */

#include "timer.h"
#include "std_types.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/* Static array of callback function pointers for timers */
static void (*g_timer0_callback_normal)(void) = NULL_PTR;
static void (*g_timer0_callback_compare)(void) = NULL_PTR;

static void (*g_timer1_callback_normal)(void) = NULL_PTR;
static void (*g_timer1_callback_compare)(void) = NULL_PTR;

static void (*g_timer2_callback_normal)(void) = NULL_PTR;
static void (*g_timer2_callback_compare)(void) = NULL_PTR;

/* Initialization function */
void Timer_init(const Timer_ConfigType * Config_Ptr)
{

    /* Select timer to configure */
    switch(Config_Ptr->timer_ID)
    {
        case TIMER0:
            /* Set initial value */
            TCNT0 = (uint8_t)(Config_Ptr->timer_InitialValue);

            /* Configure mode */
            if(Config_Ptr->timer_mode == NORMAL_MODE)
            {
                /* Normal Mode -> WGM00=0 & WGM01=0 */
                TCCR0 = (TCCR0 & 0xB7) | (0 << WGM00) | (0 << WGM01);
                /* Enable overflow interrupt */
                TIMSK |= (1 << TOIE0);
                /* Disable compare interrupt */
                TIMSK &= ~(1 << OCIE0);
            }
            else if(Config_Ptr->timer_mode == COMPARE_MODE)
            {
                /* CTC Mode (Clear Timer on Compare Match) WGM01=1, WGM00=0 */
                TCCR0 = (TCCR0 & 0xB7) | (0 << WGM00) | (1 << WGM01);
                /* Set compare match value */
                OCR0 = (uint8_t)(Config_Ptr->timer_compare_MatchValue);
                /* Enable compare interrupt */
                TIMSK |= (1 << OCIE0);
                /* Disable overflow interrupt */
                TIMSK &= ~(1 << TOIE0);
            }

            /* Configure clock prescaler */
            TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->timer_clock & 0x07);
            break;

        case TIMER1:
            /* 16-bit timer: Set initial value */
            TCNT1 = Config_Ptr->timer_InitialValue;

            /* Configure mode */
            if(Config_Ptr->timer_mode == NORMAL_MODE)
            {
                /* Normal mode: WGM10=0, WGM11=0, WGM12=0, WGM13=0 */
                TCCR1A &= ~((1 << WGM10) | (1 << WGM11));
                TCCR1B &= ~((1 << WGM12) | (1 << WGM13));
                /* Enable overflow interrupt */
                TIMSK |= (1 << TOIE1);
                /* Disable compare interrupt A */
                TIMSK &= ~(1 << OCIE1A);
            }
            else if(Config_Ptr->timer_mode == COMPARE_MODE)
            {
                /* CTC Mode with OCR1A as top: WGM13=0 WGM12=1 WGM11=0 WGM10=0 */
                TCCR1A &= ~((1 << WGM10) | (1 << WGM11));
                TCCR1B = (TCCR1B & ~(1 << WGM13)) | (1 << WGM12);
                /* Set compare match value */
                OCR1A = Config_Ptr->timer_compare_MatchValue;
                /* Enable compare interrupt A */
                TIMSK |= (1 << OCIE1A);
                /* Disable overflow interrupt */
                TIMSK &= ~(1 << TOIE1);
            }

            /* Configure clock prescaler */
            TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->timer_clock & 0x07);
            break;

        case TIMER2:
            /* Set initial value */
            TCNT2 = (uint8_t)(Config_Ptr->timer_InitialValue);

            /* Configure mode */
            if(Config_Ptr->timer_mode == NORMAL_MODE)
            {
                /* Normal mode: WGM20=0 WGM21=0 */
                TCCR2 = (TCCR2 & 0xB7) | (0 << WGM20) | (0 << WGM21);
                /* Enable overflow interrupt */
                TIMSK |= (1 << TOIE2);
                /* Disable compare interrupt */
                TIMSK &= ~(1 << OCIE2);
            }
            else if(Config_Ptr->timer_mode == COMPARE_MODE)
            {
                /* CTC mode: WGM20=0 WGM21=1 */
                TCCR2 = (TCCR2 & 0xB7) | (0 << WGM20) | (1 << WGM21);
                /* Set compare match value */
                OCR2 = (uint8_t)(Config_Ptr->timer_compare_MatchValue);
                /* Enable compare interrupt */
                TIMSK |= (1 << OCIE2);
                /* Disable overflow interrupt */
                TIMSK &= ~(1 << TOIE2);
            }

            /* Configure clock prescaler */
            TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->timer_clock & 0x07);
            break;

        default:
            /* Invalid timer ID */
            break;
    }
}

/* De-initialize timer */
void Timer_deInit(Timer_ID_Type timer_type)
{
    switch(timer_type)
    {
        case TIMER0:
            /* Stop timer clock */
            TCCR0 &= 0xF8;
            /* Disable all Timer0 interrupts */
            TIMSK &= ~((1 << TOIE0) | (1 << OCIE0));
            /* Reset initial values */
            TCNT0 = 0;
            OCR0 = 0;
            /* Clear callbacks */
            g_timer0_callback_normal = 0;
            g_timer0_callback_compare = 0;
            break;

        case TIMER1:
            /* Stop timer clock */
            TCCR1B &= 0xF8;
            /* Disable all Timer1 interrupts */
            TIMSK &= ~((1 << TOIE1) | (1 << OCIE1A));
            /* Reset initial values */
            TCNT1 = 0;
            OCR1A = 0;
            /* Clear callbacks */
            g_timer1_callback_normal = 0;
            g_timer1_callback_compare = 0;
            break;

        case TIMER2:
            /* Stop timer clock */
            TCCR2 &= 0xF8;
            /* Disable all Timer2 interrupts */
            TIMSK &= ~((1 << TOIE2) | (1 << OCIE2));
            /* Reset initial values */
            TCNT2 = 0;
            OCR2 = 0;
            /* Clear callbacks */
            g_timer2_callback_normal = 0;
            g_timer2_callback_compare = 0;
            break;

        default:
            /* Invalid timer ID */
            break;
    }
}

/* Set callback function */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID)
{
    if(a_ptr == 0) return;

    switch(a_timer_ID)
    {
        case TIMER0:
            /* Determine if normal or compare mode interrupt enabled */
            if (TIMSK & (1 << TOIE0))
                g_timer0_callback_normal = a_ptr;
            else if (TIMSK & (1 << OCIE0))
                g_timer0_callback_compare = a_ptr;
            break;

        case TIMER1:
            if (TIMSK & (1 << TOIE1))
                g_timer1_callback_normal = a_ptr;
            else if (TIMSK & (1 << OCIE1A))
                g_timer1_callback_compare = a_ptr;
            break;

        case TIMER2:
            if (TIMSK & (1 << TOIE2))
                g_timer2_callback_normal = a_ptr;
            else if (TIMSK & (1 << OCIE2))
                g_timer2_callback_compare = a_ptr;
            break;

        default:
            break;
    }
}

/* ISR Implementations */

/* Timer0 Overflow */
ISR(TIMER0_OVF_vect)
{
    if(g_timer0_callback_normal)
        g_timer0_callback_normal();
}

/* Timer0 Compare Match */
ISR(TIMER0_COMP_vect)
{
    if(g_timer0_callback_compare)
        g_timer0_callback_compare();
}

/* Timer1 Overflow */
ISR(TIMER1_OVF_vect)
{
    if(g_timer1_callback_normal)
        g_timer1_callback_normal();
}

/* Timer1 Compare Match */
ISR(TIMER1_COMPA_vect)
{
    if(g_timer1_callback_compare)
        g_timer1_callback_compare();
}

/* Timer2 Overflow */
ISR(TIMER2_OVF_vect)
{
    if(g_timer2_callback_normal)
        g_timer2_callback_normal();
}

/* Timer2 Compare Match */
ISR(TIMER2_COMP_vect)
{
    if(g_timer2_callback_compare)
        g_timer2_callback_compare();
}
