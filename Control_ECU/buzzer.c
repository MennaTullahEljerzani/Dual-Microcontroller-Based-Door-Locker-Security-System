/*
 ============================================================================
 Name        : buzzer.c
 Author      : Menna Tullah Medhat
 Description : The buzzer will serve as an alert system in case of fire
               detection by the flame sensor.
 Date        : 10/6/2024
 ============================================================================
 */
#include "buzzer.h"
#include "gpio.h"
#include "std_types.h"

/*Initializes the buzzer pin direction and turn off the buzzer.*/
 void Buzzer_init(void)
 {
	 GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);
	 GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
 }

/*Activates the buzzer.*/
 void Buzzer_on(void)
 {
	 GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
 }
/*Deactivates the buzzer.*/
 void Buzzer_off(void)
 {
	 GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
 }


