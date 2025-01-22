/*
 ============================================================================
 Name        : buzzer.h
 Author      : Menna Tullah Medhat
 Description : The buzzer will serve as an alert system in case of fire
               detection by the flame sensor.
 Date        : 10/6/2024
 ============================================================================
 */
#define BUZZER_PORT  PORTC_ID
#define BUZZER_PIN   PIN7_ID
#include "gpio.h"
#ifndef BUZZER_H_
#define BUZZER_H_

/*Initializes the buzzer pin direction and turn off the buzzer.*/
 void Buzzer_init(void);

/*Activates the buzzer.*/
 void Buzzer_on(void);
/*Deactivates the buzzer.*/
 void Buzzer_off(void);

#endif /* BUZZER_H_ */
