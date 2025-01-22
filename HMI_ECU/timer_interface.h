/******************************************************************************
* Module: TIMER
*
* File Name: timer.h
*
* Description: Header file for timer to count the displaying messages time on the LCD
*  while opening/closing the door.
*
* Author: Menna Tullah Medhat
*******************************************************************************/


#ifndef TIMER_INTERFACE_H_
#define TIMER_INTERFACE_H_

#include "std_types.h"

/****************************** Types Declarations ****************************/

/* Enumeration for Timer Prescaler options */
typedef enum {
    TIMER_NO_CLOCK, TIMER_F_CPU, TIMER_F_CPU_8, TIMER_F_CPU_64, TIMER_F_CPU_256, TIMER_F_CPU_1024,
    TIMER_EXTERNAL_FALLING, TIMER_EXTERNAL_RISING
} Timer_ClockType;

/* Enumeration for Timer Modes */
typedef enum {
    TIMER_NORMAL_MODE, TIMER_COMPARE_MODE
} Timer_ModeType;

/* Enumeration for Timer Identifiers */
typedef enum {
    TIMER0_ID, TIMER1_ID, TIMER2_ID
} Timer_ID_Type;

/* Timer Configuration Structure */
typedef struct {
    uint16 timer_InitialValue;
    uint16 timer_compare_MatchValue; /* Used only in compare mode */
    Timer_ID_Type timer_ID;
    Timer_ClockType timer_clock;
    Timer_ModeType timer_mode;
} Timer_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description: Initializes the Timer driver based on the provided configuration.
 *  - Sets up mode, prescaler, initial value, and compare value if needed.
 */
void Timer_init(const Timer_ConfigType * Config_Ptr);

/*
 * Description: Disables the specified Timer.
 */
void Timer_deInit(Timer_ID_Type timer_type);

/*
 * Description: Sets the callback function address for the specified Timer.
 */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID);

#endif /* TIMER_INTERFACE_H_ */

