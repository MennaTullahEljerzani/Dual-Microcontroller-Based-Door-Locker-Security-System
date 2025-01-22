/******************************************************************************
 * Module: PIR
 *
 * File Name: pir.h
 *
 * Description: Header file for the PIR sensor driver.
 *
 * Author: Menna Tullah Medhat
 *******************************************************************************/

#ifndef PIR_H_
#define PIR_H_

#include "std_types.h"
#include "gpio.h"
/* Define PIR sensor pin and port */
#define PIR_SENSOR_PORT PORTC_ID
#define PIR_SENSOR_PIN  PIN2_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initializes the PIR sensor.
 */
void PIR_init(void);

/*
 * Description :
 * Returns the current state of the PIR sensor (motion detected or not).
 * Return:
 *   - 1 if motion is detected
 *   - 0 if no motion is detected
 */
uint8 PIR_getState(void);

#endif /* PIR_H_ */
