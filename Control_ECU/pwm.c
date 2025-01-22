/*
 ============================================================================
 Name        : PMW.c
 Author      : Menna Tullah Medhat
 Description : PWM driver to control the fan's speed based on the temperature.
 *  The driver uses Timer0 in PWM mode
 Date        : 10/6/2024
 ============================================================================
 */
#include <avr/io.h>
#include "pwm.h"
#include "common_macros.h"
#include "gpio.h"

/*Initializes Timer0 in PWM mode and sets the required duty cycle.
Prescaler: F_CPU/64
Non-inverting mode
The function configures OC0 as the output pin.
Parameters:
duty_cycle: Percentage (0 to 100%) representing the PWM duty cycle.*/
void PWM_Timer0_Start(uint8 duty_cycle){
	TCNT0 = 0; //Set Timer Initial value

	// Set Fast PWM Mode by setting WGM01 and WGM00
	SET_BIT(TCCR0,WGM00);
	SET_BIT(TCCR0,WGM01);

	// Set Non-Inverting Mode by setting COM01 and clearing COM00
	 SET_BIT(TCCR0,COM01);
	 CLEAR_BIT(TCCR0,COM00);

	/* clock = F_CPU/64 CS00=1 CS01=0 CS02=1*/
	SET_BIT(TCCR0,CS00);
	SET_BIT(TCCR0,CS01);
	CLEAR_BIT(TCCR0,CS02);

	/* Set Compare Value (Duty Cycle here is Percentage) */
	OCR0 =duty_cycle;

	/* Set PB3/OC0 as Output pin --> pin where the PWM signal is generated from MC. */
	GPIO_setupPinDirection(OC0_PORT_ID, OC0_PIN_ID, PIN_OUTPUT);
}

