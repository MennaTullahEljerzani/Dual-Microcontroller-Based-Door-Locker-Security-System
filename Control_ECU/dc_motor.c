/*
 ============================================================================
 Name        : dc_motor.h
 Author      : Menna Tullah Medhat
 Description : The DC motor will control the fan, adjusting its speed based
               on temperature readings.
 Date        : 10/6/2024
 ============================================================================
 */
#include "std_types.h"
#include "gpio.h"
#include "dc_motor.h"
#include "pwm.h"

/*Initializes the DC motor by setting the direction for the motor pins and stopping the
motor at the beginning.*/
void DcMotor_Init(void)
{
	GPIO_setupPinDirection(MOTOR_PINS_PORT_ID, MOTOR_IN1_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PINS_PORT_ID, MOTOR_IN2_PIN_ID, PIN_OUTPUT);

	/* Stop the DC-Motor at the beginning (IN1 = 0, IN2 = 0) */
	GPIO_writePin(MOTOR_PINS_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
	GPIO_writePin(MOTOR_PINS_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
}
/*Controls the motor's state (Clockwise/Anti-Clockwise/Stop) and adjusts the speed based
on the input duty cycle.*/
/*Parameters:
▪ state: Enum or uint8 that indicates motor direction (CW, A-CW, Stop).
▪ speed: Motor speed in percentage (0 to 100%).*/
void DcMotor_Rotate(DcMotor_State state, uint8 speed){
	/* Setting the DC Motor rotation direction (CW/ or A-CW or stop) based on the state value. */
	switch(state)
	{
		case STOP:
			/* Stop the DC-Motor (IN1 = 0, IN2 = 0) */
			GPIO_writePin(MOTOR_PINS_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
			GPIO_writePin(MOTOR_PINS_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
			break;
		case ClockWise:
			/* DC-Motor Mode --> ClockWise Rotation (IN1 = 0, IN2 = 1) */
			GPIO_writePin(MOTOR_PINS_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
			GPIO_writePin(MOTOR_PINS_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_HIGH);
			break;
		case Anti_ClockWise:
			/* DC-Motor Mode --> Anti_ClockWise Rotation (IN1 = 1, IN2 = 0) */
			GPIO_writePin(MOTOR_PINS_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_HIGH);
			GPIO_writePin(MOTOR_PINS_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
			break;
		default:
			break;
		}

		/* Setting the Required DC Motor Speed Using PWM Signal at Enable1 */
		if ((speed >= 0) && (speed <= 255))
		{
			PWM_Timer0_Start(speed);
		}
		else
		{
			PWM_Timer0_Start(0);
		}
}
