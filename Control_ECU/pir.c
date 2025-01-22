/******************************************************************************
 * Module: PIR
 *
 * File Name: pir.c
 *
 * Description: source file for the PIR sensor driver.
 *
 * Author: Menna Tullah Medhat
 *******************************************************************************/

#include "pir.h"


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initializes the PIR sensor by configuring the sensor pin as input.
 */
void PIR_init(void) {
    /* Set the PIR sensor pin as input */
    GPIO_setupPinDirection(PIR_SENSOR_PORT, PIR_SENSOR_PIN, PIN_INPUT);
}

/*
 * Description :
 * Returns the current state of the PIR sensor.
 * - 1: Motion detected
 * - 0: No motion detected
 */
uint8 PIR_getState(void) {
    /* Check if the PIR sensor pin is high (motion detected) */
	uint8 motion ;
		motion=GPIO_readPin(PIR_SENSOR_PORT, PIR_SENSOR_PIN);

		/*if their is a motion we will return MOTION_DETECTED else we will return NO_MOTION_DETECTED */
		return motion;
}
