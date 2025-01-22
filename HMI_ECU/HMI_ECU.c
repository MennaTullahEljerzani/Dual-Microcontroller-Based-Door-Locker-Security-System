/******************************************************************************
* Module: HUMAN MACHINE INTERFACE ECU
*
* File Name: HMI_ECU.c
*
* Description: Main Application for the Control Unit (Control_ECU)
*
* Author: Menna Tullah Medhat
*******************************************************************************/
#include "lcd.h"
#include "keypad.h"
#include "std_types.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "timer_interface.h"
#include "uart.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PASSWORD_SIZE 				5
#define PASSWORD_SAVED 				0x11
#define DIFF_PASSWORDS				0x22
#define TRUE_PASSWORD				0x33
#define WRONG_PASSWORD				0x32
#define CONTROL_ECU_READY			0xFF
#define LOCKING_DOOR				0x44
#define UNLOCK_DOOR    				0x55
#define ALARM_MODE					0x53
#define CHANGE_PASSWORD				0xE3
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
volatile uint8 g_seconds_counter = 0;
volatile uint8 g_change_flag = 0;
/* Create configuration structure for timer driver */
			/* Initialize the Timer driver :
			 * use timer 1
			 * prescaler 1024
			 * compare mode
			 * initial value = 0
			 * compare value = 7812, so the interrupt occurs every 1 seconds
			 */
Timer_ConfigType timerConfig = {0,7812,TIMER1_ID,TIMER_F_CPU_1024,TIMER_COMPARE_MODE};
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void changePassword(void);
void createPassword(void);
void timer1_SetRequiredCallBackFunction(void);
void getPassword(uint8 * pass, uint8 size);
void checkPassword(uint8* isPassTrue);
void alarmMode(void);
/*******************************************************************************
 *                                    Main                                     *
 *******************************************************************************/
int main(void){
	uint8 key;
	uint8 isPassTrue;
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
	/* Initialize the LCD */
	LCD_init();
	/* At the beginning, display "Door Lock System"  */
	LCD_displayString("Door Lock System");
	_delay_ms(500);
	/* when the system start for the first time, create New Password */
	Timer_setCallBack(timer1_SetRequiredCallBackFunction, TIMER1_ID);
	createPassword();
	LCD_clearScreen();
	for(;;){
		/* Display always the main system options */
		LCD_displayString("+ : Open Door");
		LCD_displayStringRowColumn(1,0,"- : Change Pass");
		/* Get the key pressed by user */
		key = KEYPAD_getPressedKey();
		_delay_ms(250);
		/* if user chooses (+) Open Door */
		if(key == '+'){
			/* The user should enter the password saved in EEPROM */
			checkPassword(&isPassTrue);
			/* if the user entered the true password */
			if (isPassTrue == TRUE_PASSWORD) {
				/* Send UNLOCK_DOOR to the Control ECU to open the Door (rotate motor) */
				UART_sendByte(UNLOCK_DOOR);
				/* Set the Call back function pointer in the timer driver */

				/* Initialize the Timer driver :
				 * use timer 1
				 * prescaler 1024
				 * compare mode
				 * initial value = 0
				 * compare value = 7812, so the interrupt occurs every 1 seconds
				 */
				Timer_init(&timerConfig);
				/* Display Door Unlocking please wait on LCD for 15 seconds */
				LCD_clearScreen();
				LCD_displayString("Door Unlocking");
				LCD_displayStringRowColumn(1, 0, "please wait");
				 /* wait until t_15s_flag become true, this will happen after 15 seconds */
				g_seconds_counter = 0;
   			    while (g_seconds_counter <= 15){

				}
   			    Timer_deInit(TIMER1_ID);
   			    g_seconds_counter = 0;


				/* Display wait for people to enter */
   		    	Timer_init(&timerConfig);
				LCD_clearScreen();
				LCD_displayString("wait for people");
				LCD_displayStringRowColumn(1, 0,"to enter");
				while(g_seconds_counter<3){};
				/* wait until Control ECU sends LOCKING_DOOR */
				while (UART_recieveByte() != LOCKING_DOOR);

				Timer_deInit(TIMER1_ID);
				  g_seconds_counter = 0;
				/* Set the call back function and initialize timer driver again  */

				Timer_init(&timerConfig);
				/* Display Door Locking on LCD for 15 seconds */
				LCD_clearScreen();
				LCD_displayString("Door Locking");
				/* wait until t_15s_flag become true, this will happen after 15 seconds */
				/*while (t_15s_flag != TRUE);*//*to stop the system one minute*/
				g_seconds_counter = 0;
				while (g_seconds_counter <= 15){

				}
				Timer_deInit(TIMER1_ID);
				 g_seconds_counter = 0;
				LCD_clearScreen();
			}
			/* if the user entered wrong password 3 times */
			else if(isPassTrue == WRONG_PASSWORD){
				/* enter alarm mode for 1 minute */
				alarmMode();
			}
		}
		/* if user chooses (-) Change Password */
		else if(key == '-'){
			/* The user should enter the password saved in EEPROM */
			checkPassword(&isPassTrue);
			/* if the user entered the true password */
			if(isPassTrue == TRUE_PASSWORD){
				/* Send CHANGE_PASSWORD to the Control ECU to get ready to save new password */
				if (g_change_flag==0){
					g_change_flag =1;
					changePassword();
				}
			}
			/* if the user entered wrong password 3 times */
			else if(isPassTrue == WRONG_PASSWORD){
				/* enter alarm mode for 1 minute */
				alarmMode();
			}
		}
	}
}
void changePassword(void){
	if (g_change_flag==1){
	/* Send CHANGE_PASSWORD to the Control ECU to get ready to save new password */
	UART_sendByte(CHANGE_PASSWORD);
	/* Create new password */
	uint8 size = PASSWORD_SIZE+2;
	uint8 pass1[size], pass2[size];
	uint8 isSaved;
	/* loop until the user enters same password twice for confirmation */
	for(;;){
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"plz enter pass: ");
		LCD_moveCursor(1,0);
		/* Get the password from the user */
		getPassword(pass1,size);
		/* wait until the user press enter button */
		while(KEYPAD_getPressedKey() != '=');
		_delay_ms(250);
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"plz re-enter the");
		LCD_displayStringRowColumn(1,0,"same pass:");
		/* Get the password again from the user for confirmation */
		getPassword(pass2,size);
		/* wait until the user press enter button */
		while(KEYPAD_getPressedKey() != '=');
		_delay_ms(250);
		/* Wait until Control_ECU is ready to receive the password */
		while(UART_recieveByte() != CONTROL_ECU_READY);
		/* send the to passwords to Control_ECU */
		UART_sendString(pass1);
		UART_sendString(pass2);
		/* if the two passwords are the same Control_ECU will save the password in the EEPROM and send PASSWORD_SAVED */
		isSaved = UART_recieveByte();
		/* if Control_ECU saves the password return */
		if(isSaved == PASSWORD_SAVED){
			return;
		}
	}
	LCD_clearScreen();
	g_change_flag = 0;
	}
}
/*
 * Description :
 * Function responsible for :
 * 1. Display error message on LCD for 1 minute
 * 2. Wait for 1 minute and no inputs from the keypad will be accepted during this time period.
 */
void alarmMode(void){
	/* Set the call back function and initialize timer driver again  */

	Timer_init(&timerConfig);
	/* Display error message on LCD */
	LCD_clearScreen();
	LCD_displayString("System LOCKED");
	LCD_displayStringRowColumn(1,0,"Wait for 1 min");
	/* wait until 1 minute */
	g_seconds_counter = 0;
	while (g_seconds_counter <= 60){

	}
	/*Timer_deInit(TIMER1_ID);*/
	Timer_deInit(TIMER1_ID);

	g_seconds_counter=0;
	LCD_clearScreen();
}
/*
 * Description :
 * Function responsible for :
 * 1. get the password from the user.
 * 2. send it to Control ECU.
 * 3. set a flag to  TRUE_PASSWORD if the user enters the true password
 *    or to WRONG_PASSWORD if the user enters a wrong password.
 */
void checkPassword(uint8* flag_ptr){
	uint8 size = PASSWORD_SIZE+2;
	uint8 pass[size] , loop_counter;
	/* loop 3 times each time get password from the user it true return */
	for(loop_counter = 0 ; loop_counter<3 ; loop_counter++){
		LCD_clearScreen();
		LCD_displayString("enter old pass:");
		LCD_moveCursor(1,0);
		/* Get the password from the user */
		getPassword(pass,size);
		/* wait until the user press enter button */
		while(KEYPAD_getPressedKey() != '=');
		_delay_ms(250);
		/* Wait until Control_ECU is ready to receive the password */
		while(UART_recieveByte() != CONTROL_ECU_READY);
		/* send the password */
		UART_sendString(pass);
		/* if the password is true Control_ECU will send TRUE_PASSWORD , if not it will sends WRONG_PASSWORD */
		*flag_ptr = UART_recieveByte();
		if(*flag_ptr == TRUE_PASSWORD){
			return;
		}
	} /* it will reach here if the user enters wrong password 3 times and then (*flag_ptr=WRONG_PASSWORD)  */
}
/*
 * Description :
 * Function responsible for create new password by:
 * 1. take the password from the user and take the password again for confirmation.
 * 2. send the two passwords to Control_ECU.
 * 3. it will repeat until the user enters the same password twice
 */
void createPassword(void){
	uint8 size = PASSWORD_SIZE+2;
	uint8 pass1[size], pass2[size];
	uint8 isSaved;
	/* loop until the user enters same password twice for confirmation */
	for(;;){
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"plz enter pass: ");
		LCD_moveCursor(1,0);
		/* Get the password from the user */
		getPassword(pass1,size);
		/* wait until the user press enter button */
		while(KEYPAD_getPressedKey() != '=');
		_delay_ms(250);
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"plz re-enter the");
		LCD_displayStringRowColumn(1,0,"same pass:");
		/* Get the password again from the user for confirmation */
		getPassword(pass2,size);
		/* wait until the user press enter button */
		while(KEYPAD_getPressedKey() != '=');
		_delay_ms(250);
		/* Wait until Control_ECU is ready to receive the password */
		while(UART_recieveByte() != CONTROL_ECU_READY);
		/* send the to passwords to Control_ECU */
		UART_sendString(pass1);
		UART_sendString(pass2);
		/* if the two passwords are the same Control_ECU will save the password in the EEPROM and send PASSWORD_SAVED */
		isSaved = UART_recieveByte();
		/* if Control_ECU saves the password return */
		if(isSaved == PASSWORD_SAVED){
			return;
		}
	}
}
/*
 * Description :
 * Function responsible for take password from user.
 */
void getPassword(uint8 * pass, uint8 size){
	uint8 loop_counter;
	for (loop_counter = 0; loop_counter < size-2; loop_counter++) {
		pass[loop_counter] = KEYPAD_getPressedKey() + 48;
		LCD_displayCharacter('*');
		_delay_ms(250);
	}
	/* end the password with # and null */
	pass[loop_counter++] = '#';
	pass[loop_counter] = '\0';
}
/*
 * Description :
 * call-back function.
 */
void timer1_SetRequiredCallBackFunction(void){
	 g_seconds_counter++;
}
