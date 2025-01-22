/*
 ============================================================================
 Name        : PMW.h
 Author      : Menna Tullah Medhat
 Description : PWM driver to control the fan's speed based on the temperature.
 *  The driver uses Timer0 in PWM mode
 Date        : 10/6/2024
 ============================================================================
 */
#include "std_types.h"
#ifndef PMW_H_
#define PMW_H_

#define OC0_PORT_ID        PORTB_ID
#define OC0_PIN_ID         PIN3_ID

/*Use the PWM driver to control the fan's speed based on the temperature.
 *  The driver uses Timer0 in PWM mode*/

/*Initializes Timer0 in PWM mode and sets the required duty cycle.
Prescaler: F_CPU/64
Non-inverting mode
The function configures OC0 as the output pin.
Parameters:
duty_cycle: Percentage (0 to 100%) representing the PWM duty cycle.*/
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PMW_H_ */
