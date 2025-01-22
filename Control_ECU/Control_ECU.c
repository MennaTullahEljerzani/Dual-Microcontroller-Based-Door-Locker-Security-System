/******************************************************************************
* Module: CONTROL ECU
*
* File Name: Control_ECU_Main.c
*
* Description: Main Application for the Control Unit (Control_ECU)
*
* Author: Menna Tullah Medhat
*******************************************************************************/

#include "std_types.h"
#include "uart.h"
#include "buzzer.h"
#include "dc_motor.h"
#include "external_eeprom.h"
#include "pir.h"
#include "twi.h"
#include "string.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "timer_interface.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PASSWORD_SIZE				5
#define PASSWORD_SAVED 				0x11
#define DIFF_PASSWORDS				0x22
#define TRUE_PASSWORD				0x33
#define WRONG_PASSWORD				0x32
#define CONTROL_ECU_READY			0xFF
#define LOCKING_DOOR				0x44
#define UNLOCK_DOOR    				0x55
#define CHANGE_PASSWORD				0xE3
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
volatile uint8 g_seconds_counter = 0;
volatile uint8 motionDetected = 0;
/*volatile boolean t_15s_flag = FALSE;*/
/*volatile boolean t_60s_flag = FALSE;
volatile boolean alarm_flag = FALSE;*/
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void getAndSavePassword(void);
void changePassword(void);
void timer1_SetRequiredCallBackFunction(void);
void inits(void);
/*******************************************************************************
 *                                    Main                                     *
 *******************************************************************************/
int main(void){
	uint8 size = PASSWORD_SIZE + 1;
	uint8 enteredPass[size] , savedPass[size];
	uint8 action = 0;
	/*uint8 door_locking=0;*/
	/* Create configuration structure for timer driver */
	Timer_ConfigType timerConfig = {0,7812,TIMER1_ID,TIMER_F_CPU_1024,TIMER_COMPARE_MODE};
	/* Create configuration structure for UART driver */
	UART_ConfigType uartConfig = {UART_8_BIT, UART_NO_PARITY, UART_ONE_STOP_BIT, 9600};
	/* Enable Global Interrupt */
	SREG |= (1<<7);
	/* Initialize the UART driver with :
	 * Baud-rate = 9600 bits/sec
	 * one stop bit
	 * No parity
	 * 8-bit data
	 */
	UART_init(&uartConfig);
	/* Create configuration structure for TWI/I2C driver */
	TWI_configType twiConfig = {PRESCALER_1, 400000, 0x01};

	/* Initialize the TWI driver with :
	 * my address = 0x01
	 * bite rate = 0x02    so SCL frequency= 400 bit/s
	 */
	TWI_init(&twiConfig);
	Timer_setCallBack(timer1_SetRequiredCallBackFunction, TIMER1_ID);
	inits();
	/* Get the password from HMI_ECU and save it in the External EEPRPOM */
	getAndSavePassword();
	_delay_ms(10);
	for(;;){
		uint8 loop_counter;
		/* loop 3 times until the user enter the true password */
		for(loop_counter=0 ; loop_counter<3 ; loop_counter++){
			/* Send CONTROL_ECU_READY byte to HMI_ECU to ask it to send the password */
			UART_sendByte(CONTROL_ECU_READY);
			/* Receive the password from HMI_ECU through UART */
			UART_receiveString(enteredPass);
			/* get the password saved in the EEPROM */
			EEPROM_readData(0x0311,savedPass,PASSWORD_SIZE);
			savedPass[PASSWORD_SIZE] = '\0';
			/* compare the received password and the saved password */
			if(!strcmp((char*)enteredPass,(char*)savedPass)){
				/* if the two passwords are the same send TRUE_PASSWORD byte to HMI_ECU */
				UART_sendByte(TRUE_PASSWORD);
				/* Receive an action byte from HMI_ECU (Open Door or Change Password) */
				action = UART_recieveByte();
				break;
			}else{
				/* if the two passwords are not the same send WRONG_PASSWORD byte to HMI_ECU */
				UART_sendByte(WRONG_PASSWORD);
			}
		}
		/* if loop_counter variable reach 3, this means the user enters wrong password 3 times */
		if(loop_counter == 3){
			/* Set the Call back function pointer in the timer driver */
			/*Timer_setCallBack(timer1_SetRequiredCallBackFunction,TIMER1_ID);*/
			/* Initialize the Timer driver :
			 * use timer 1
			 * prescaler 1024
			 * compare mode
			 * initial value = 0
			 * compare value = 7812, so the interrupt occurs every 1 seconds
			 */
			Timer_init(&timerConfig);
			/* turn on buzzer */
			Buzzer_on();
			/*to stop the system one minute*/
			g_seconds_counter = 0;
			while (g_seconds_counter <= 60){
			}
			/*Timer_deInit(TIMER1_ID);*/
			Timer_deInit(TIMER1_ID);

			g_seconds_counter=0;
			/* turn off buzzer */
			Buzzer_off();
		}
		/* if loop_counter variable doesn't reach 3, then the user enter the true password */
		else{
			/* process Open Door option */
			if(action == UNLOCK_DOOR){
				/* Set the call back function and initialize timer driver again  */

				Timer_init(&timerConfig);
				/* Rotate the motor clockwise with maximum speed for 15 seconds */
				DcMotor_Rotate(ClockWise,100);
				g_seconds_counter = 0;
				/* wait 15 seconds*/
				while (g_seconds_counter <= 15){
				}

				/*Timer_deInit(TIMER1_ID);*/
				Timer_deInit(TIMER1_ID);

				g_seconds_counter=0;
				/* stop the motor to keep the door open */
				DcMotor_Rotate(STOP,0);
				/*Timer_deInit(TIMER1_ID);*/

				Timer_init(&timerConfig);

				/*wait for 3 sec and the motor is stop*/
				while(g_seconds_counter<3){};
				_delay_ms(200);
				do {
				motionDetected = PIR_getState();
				} while (motionDetected);

				/*Timer_deInit(TIMER1_ID);*/
				Timer_deInit(TIMER1_ID);

				g_seconds_counter=0;
				/* Set the call back function and initialize timer driver again*/

				/* Rotate the motor anti-clockwise with maximum speed for 15 seconds */

				/* Closing the door for 15 seconds */
				UART_sendByte(LOCKING_DOOR);
				DcMotor_Rotate(Anti_ClockWise,100);
				/* door_locking = UART_recieveByte();*/
			   	Timer_init(&timerConfig);
			   	while(g_seconds_counter<=15){};
                /*Stop Timer1*/
			   	Timer_deInit(TIMER1_ID);

			   	g_seconds_counter=0;

				/* stop the motor */
				DcMotor_Rotate(STOP,0);
			}
			/* process Change Password option */
			else if(action == CHANGE_PASSWORD){
				/* Get the password from HMI_ECU and save it in the External EEPRPOM */
				changePassword();
				_delay_ms(10);
			}
		}
	}
}
void changePassword(void)
{
	uint8 i,enteredPass1[PASSWORD_SIZE+2],savedPass1[PASSWORD_SIZE+2];
	for (i = 0; i < PASSWORD_SIZE; i++) {
		enteredPass1[i] = UART_recieveByte();
	}

	// Read the old password from the External EEPROM
	EEPROM_readData(0x0311,savedPass1,PASSWORD_SIZE);
	// Checking the password
	for (i = 0; i < PASSWORD_SIZE; ++i) {
		if (enteredPass1[i] != savedPass1[i]) {
			// Sends to the HMI that the passwords do not match
			UART_sendByte(TRUE_PASSWORD);
			break;
		}
	}
	if (i == PASSWORD_SIZE) {
		/* Sends to the HMI that the password is correct */
		UART_sendByte(WRONG_PASSWORD);
        getAndSavePassword();
		}

}
/*
 * Description :
 * Function responsible for Get the password from HMI_ECU and save it in the External EEPRPOM.
 */
void getAndSavePassword(void){
	uint8 size = PASSWORD_SIZE+1;
	uint8 pass1[size], pass2[size];
	/* loop until the user enters same password twice for confimation  */
	for(;;){
		/* Send CONTROL_ECU_READY byte to HMI_ECU to ask it to send the two passwords */
		UART_sendByte(CONTROL_ECU_READY);
		/* Receive the password and the confirmation password from HMI_ECU */
		UART_receiveString(pass1);
		UART_receiveString(pass2);
		/* compare the two passwords */
		if(!strcmp((char *)pass1,(char *)pass2)){
			/* if the two passwords are the same save the password in the EEPROM */
			EEPROM_writeData(0x0311,pass1,PASSWORD_SIZE);
			/* send PASSWORD_SAVED byte to HMI_ECU */
			UART_sendByte(PASSWORD_SAVED);
			return;
		}else{
			/* if the two passwords are not the same, send DIFF_PASSWORDS byte to HMI_ECU */
			UART_sendByte(DIFF_PASSWORDS);
		}
	}
}
/*
 * Description :
 * call-back function.
 */
void timer1_SetRequiredCallBackFunction(void){
	 g_seconds_counter++;
}
/*it Initialize buzzer, motor and PIR SENSOR*/
void inits(void){
	/* Initialize the Buzzer */
	Buzzer_init();
	/* Initialize the DC Motor */
	DcMotor_Init();
	/* Initialize the PIR Sensor */
	PIR_init();
}
