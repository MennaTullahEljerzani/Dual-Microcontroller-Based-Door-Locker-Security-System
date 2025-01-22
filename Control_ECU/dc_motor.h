/*
 ============================================================================
 Name        : dc_motor.h
 Author      : Menna Tullah Medhat
 Description : The DC motor will control the fan, adjusting its speed based
               on temperature readings.
 Date        : 10/6/2024
 ============================================================================
 */
#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_
#include "std_types.h"

#define MOTOR_PINS_PORT_ID         PORTD_ID

#define MOTOR_IN1_PIN_ID           PIN6_ID
#define MOTOR_IN2_PIN_ID           PIN7_ID


/* Enum DcMotor_State to Select type of motion of DC-Motor (CW, A_CW, Stop) */
typedef enum{
	STOP,Anti_ClockWise,ClockWise
}DcMotor_State;



/*Initializes the DC motor by setting the direction for the motor pins and stopping the
motor at the beginning.*/
void DcMotor_Init(void);

/*Controls the motor's state (Clockwise/Anti-Clockwise/Stop) and adjusts the speed based
on the input duty cycle.*/
/*Parameters:
▪ state: Enum or uint8 that indicates motor direction (CW, A-CW, Stop).
▪ speed: Motor speed in percentage (0 to 100%).*/
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* DC_MOTOR_H_ */
