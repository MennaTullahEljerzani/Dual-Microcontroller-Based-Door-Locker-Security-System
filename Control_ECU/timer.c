/******************************************************************************
* Module: TIMER
*
* File Name: timer.c
*
* Description: source file for timer to count the time for controlling the motor.
*
* Author: Menna Tullah Medhat
*******************************************************************************/


#include "timer_private.h"
#include <avr/interrupt.h>
#include "timer_interface.h"
#include "timer_interface.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global array of pointers to function to hold the address of the call back function in the application */

static volatile void (*g_Timers_callBackPtr[3])(void)={NULL_PTR};

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/


/***************************** TIMER0 **********************************/

/*interrupt service routine for timer0 OverFlow*/
ISR(TIMER0_OVF_vect)
{
	if(g_Timers_callBackPtr[0] != NULL_PTR)
	{
		g_Timers_callBackPtr[0]();
	}
}
/*interrupt service routine for timer0 CTC*/
ISR(TIMER0_COMP_vect)
{
	if(g_Timers_callBackPtr[0] != NULL_PTR)
	{
		g_Timers_callBackPtr[0]();
	}
}

/*************************** TIMER1 *******************************/

/*interrupt service routine for timer1 Overflow*/
ISR(TIMER1_OVF_vect)
{
	if(g_Timers_callBackPtr[1] != NULL_PTR)
	{
		g_Timers_callBackPtr[1]();
	}
}

/*interrupt service routine for timer1 CTC*/
ISR(TIMER1_COMPA_vect)
{
	if(g_Timers_callBackPtr[1] != NULL_PTR)
	{
		g_Timers_callBackPtr[1]();
	}
}

/************************** TIMER2 **************************/

/*interrupt service routine for timer2 Overflow*/
ISR(TIMER2_OVF_vect)
{
	if(g_Timers_callBackPtr[2] != NULL_PTR)
	{
		g_Timers_callBackPtr[2]();
	}
}

/*interrupt service routine for timer2 CTC*/
ISR(TIMER2_COMP_vect)
{
	if(g_Timers_callBackPtr[2] != NULL_PTR)
	{
		g_Timers_callBackPtr[2]();
	}
}
/*****************************************************************************************/


/**********************************************************************************************************
 *                 					Set CallBack Function based on timer id               				  *
 **********************************************************************************************************/

/*
 * Description :
 * A function to set Call back for timers
 */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID )
{

	switch(a_timer_ID)
	{
	case TIMER0_ID:
		/* Save the address of the Call back function in a global Ptr to Function variable for timer0 */
		g_Timers_callBackPtr[0]= a_ptr;
		break;

	case TIMER1_ID:
		/* Save the address of the Call back function in a global Ptr to Function variable for timer1 */
		g_Timers_callBackPtr[1] = a_ptr;
		break;

	case TIMER2_ID:
		/* Save the address of the Call back function in a global Ptr to Function variable for timer2 */
		g_Timers_callBackPtr[2] = a_ptr;
		break;
	}

}

/**********************************************************************************************************
 *                 					intialize timer used based on timer id               				  *
 **********************************************************************************************************/

/*
 * Description :
 * A function to initialize the timers
 */
void Timer_init(const Timer_ConfigType * Config_Ptr)
{

	switch(Config_Ptr->timer_ID)
	{

	case TIMER0_ID:
		/*Clear registers of Timer0 And Interrupts Modules Enable before any access to prevent any mistakes
		 * on changing the mode of same timer as you might forget to use deinit() before init() one more*/
		TIMER0_TCCR0_REG.Byte=0;
		TIMER0_TCNT0_REG.Byte=0;
		TIMER0_OCR0_REG.Byte=0;

		/*Clear all Interrupt modules Enable of Timer0 (OCIE0=0,TOIE0=0)*/
		TIMER_TIMSK_REG.Bits.OCIE0_Bit=0;
		TIMER_TIMSK_REG.Bits.TOIE0_Bit=0;


		/*Set Intial Value*/
		TIMER0_TCNT0_REG.Byte=((Config_Ptr->timer_InitialValue))&0x00FF;

		/*
		 * TIMER Prescaler value for Timer0 in TCCR0 Register
		 * insert the required clock value in the first three bits (CS00, CS01 and CS02)
		 * 0XF8 to make sure that the least 3-bits in TCCR0 register=0
		 */
		TIMER0_TCCR0_REG.Byte=((TIMER0_TCCR0_REG.Byte&0xF8)|(Config_Ptr->timer_clock));

		/*
		 * Compare output mode:
		 * 		OC0 disconnected
		 *      Clear COM00 bit in TCCR0 register
		 *      Clear COM01 bit in TCCR0 register
		 */
		TIMER0_TCCR0_REG.Bits.COM00_Bit=0;
		TIMER0_TCCR0_REG.Bits.COM01_Bit=0;

		switch(Config_Ptr->timer_mode)
		{
		case TIMER_NORMAL_MODE:
			/*
			 * Normal (Overflow) mode:
			 * 		Clear WGM00 bit in TCCR0 register
			 *      Clear WGM01 bit in TCCR0 register
			 */
			TIMER0_TCCR0_REG.Bits.WGM00_Bit=0;
			TIMER0_TCCR0_REG.Bits.WGM01_Bit=0;

			/* SET FOC0 in any mode except PWM mode */
			TIMER0_TCCR0_REG.Bits.FOC0_Bit=1;

			/* Enable Timer0 overflow interrupt */
			TIMER_TIMSK_REG.Bits.TOIE0_Bit=1;
			break;


		case TIMER_COMPARE_MODE:
			/*
			 * Compare mode:
			 * 		Clear WGM00 bit in TCCR0 register
			 *      SET WGM01 bit in TCCR0 register
			 */
			TIMER0_TCCR0_REG.Bits.WGM00_Bit=0;
			TIMER0_TCCR0_REG.Bits.WGM01_Bit=1;

			/* SET FOC0 in any mode except PWM mode */
			TIMER0_TCCR0_REG.Bits.FOC0_Bit=1;

			/* set the compare value in register OCR0 & with 0xFF to make sure that it doesn't exceed 8 bits */
			TIMER0_OCR0_REG.Byte=((Config_Ptr->timer_compare_MatchValue))&0x00FF;

			/* Enable Timer0 compare match interrupt*/
			TIMER_TIMSK_REG.Bits.OCIE0_Bit=1;

			break;
		}

		break; /*End of Timer0 case*/


		case TIMER1_ID:
			/*Clear registers of Timer1 And Interrupts Modules Enables before any access to prevent any mistakes
			* on changing the mode of same timer as you might forget to use deinit() before init() one more
			* & with 0xC3 to Clear 4 bits of Interrupt modules Enables for timer1 in TIMSK*/
			TIMER1_TCCR1A_REG.Byte=0;
			TIMER1_TCCR1B_REG.Byte=0;
			TIMER1_TCNT1_REG.TwoBytes=0;
			TIMER1_OCR1A_REG.TwoBytes=0;

			/*Clear all Interrupt modules Enable of Timer1 (TICIE1=0,OCIE1A=0,OCIE1B=0,TOIE1=0)*/
			TIMER_TIMSK_REG.Byte&=0xC3;

			/*Set Intial Value*/
			TIMER1_TCNT1_REG.TwoBytes=(Config_Ptr->timer_InitialValue);

			/*
			 * TIMER Prescaler value for Timer1 in TCCR1B Register
			 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
			 * 0XF8 to make sure that the least 3-bits in TCCR0 register=0
			 */
			TIMER1_TCCR1B_REG.Byte=(TIMER1_TCCR1B_REG.Byte&0xF8)|(Config_Ptr->timer_clock);

			/*
			 * Compare mode:
			 * 		OC1A/OC1B disconnected
			 *      Clear COM1A0/COM1A1 bits in TCCR1A register
			 *      Clear COM1B0/COM1B1 bits in TCCR1A register
			 */
			TIMER1_TCCR1A_REG.Byte&=0x0F;


			switch(Config_Ptr->timer_mode)
			{
			case TIMER_NORMAL_MODE:

				/*
				 * Normal Overflow mode:
				 * 		Clear WGM10/WMG11 bit in TCCR1A register
				 *      Clear WGM12/WGM13 bit in TCCR1B register
				 */
				TIMER1_TCCR1A_REG.Bits.WGM10_Bit=0;
				TIMER1_TCCR1A_REG.Bits.WGM11_Bit=0;
				TIMER1_TCCR1B_REG.Bits.WGM12_Bit=0;
				TIMER1_TCCR1B_REG.Bits.WGM13_Bit=0;

				/* Enable Timer1 overflow interrupt */
				TIMER_TIMSK_REG.Bits.TOIE1_Bit=1;

				break;


			case TIMER_COMPARE_MODE:
				/*
				 *  Compare mode:
				 *  	Clear WGM10/WGM11 bits in TCCR1A register
				 *      Set WGM12 bit in TCCR1B register
				 *      Clear WGM13 bit in TCCR1B register
				 */
				TIMER1_TCCR1A_REG.Byte&=0xFC;
				TIMER1_TCCR1B_REG.Bits.WGM12_Bit=1;
				TIMER1_TCCR1B_REG.Bits.WGM13_Bit=0;

				/* SET FOC1A/FOC1B in any mode except PWM mode */
				TIMER1_TCCR1A_REG.Bits.FOC1A_Bit=1;
				TIMER1_TCCR1A_REG.Bits.FOC1B_Bit=1;

				/* set the compare value in register OCR1A*/
				TIMER1_OCR1A_REG.TwoBytes = (Config_Ptr->timer_compare_MatchValue);

				/* Enable Timer1 compare match module interrupt*/
				TIMER_TIMSK_REG.Bits.OCIE1A_Bit=1;

				break;
			}



			break; /*End of Timer1 case*/



			case TIMER2_ID:

				/*Clear registers of Timer2 And Interrupts Modules Enable before any access to prevent any mistakes
				 * on changing the mode of same timer as you might forget to use deinit() before init() one more*/
				TIMER2_TCCR2_REG.Byte=0;
				TIMER2_TCNT2_REG.Byte=0;
				TIMER2_OCR2_REG.Byte=0;

				/*Clear all Interrupt modules Enable of Timer2 (OCIE2=0,TOIE2=0)*/
				TIMER_TIMSK_REG.Bits.OCIE2_Bit=0;
				TIMER_TIMSK_REG.Bits.TOIE2_Bit=0;

				/*Set Intial Value & with 0xFF to make sure that it doesn't exceed 8 bits */
				TIMER2_TCNT2_REG.Byte=((Config_Ptr->timer_InitialValue))&0x00FF;

				/*
				 * TIMER2 Prescaler value for Timer0 in TCCR2 Register
				 * insert the required clock value in the first three bits (CS20, CS21 and CS22)
				 * 0XF8 to make sure that the least 3-bits in TCCR2 register=0
				 */
				TIMER2_TCCR2_REG.Byte=((TIMER2_TCCR2_REG.Byte&0xF8)|(Config_Ptr->timer_clock));

				/*
				 *  Compare mode:
				 *  	OC2 disconnected
				 *      Clear COM20 bit in TCCR2 register
				 *      Clear COM21 bit in TCCR2 register
				 */
				TIMER2_TCCR2_REG.Bits.COM20_Bit=0;
				TIMER2_TCCR2_REG.Bits.COM21_Bit=0;

				switch(Config_Ptr->timer_mode)
				{
				case TIMER_NORMAL_MODE:

					/*
					 * Normal Overflow mode:
					 * 		Clear WGM20 bit in TCCR2 register
					 *      Clear WGM21 bit in TCCR2 register
					 */
					TIMER2_TCCR2_REG.Bits.WGM20_Bit=0;
					TIMER2_TCCR2_REG.Bits.WGM21_Bit=0;

					/* SET FOC2 in any mode except PWM mode */
					TIMER2_TCCR2_REG.Bits.FOC2_Bit=1;

					/* Enable Timer2 overflow interrupt */
					TIMER_TIMSK_REG.Bits.TOIE2_Bit=1;

					break;

				case TIMER_COMPARE_MODE:

					/*
					 *  Compare mode:
					 *  	Clear WGM20 bit in TCCR2 register
					 *      SET WGM21 bit in TCCR2 register
					 */
					TIMER2_TCCR2_REG.Bits.WGM20_Bit=0;
					TIMER2_TCCR2_REG.Bits.WGM21_Bit=1;

					/* SET FOC2 in any mode except PWM mode */
					TIMER2_TCCR2_REG.Bits.FOC2_Bit=1;

					/* set the compare value in register OCR2 & with 0xFF to make sure that it doesn't exceed 8 bits */
					TIMER2_OCR2_REG.Byte=((Config_Ptr->timer_compare_MatchValue))&0x00FF;

					/* Enable Timer2 compare match interrupt*/
					TIMER_TIMSK_REG.Bits.OCIE2_Bit=1;

					break;
				}

				break; /*End of Timer2 case*/

	}/*End of the outer Super switch case*/

}/*End of the Timer_init*/

/*
 * Description :
 * A function to Deinitialize the timers
 */
void Timer_deInit(Timer_ID_Type timer_type)
{
	switch(timer_type)
	{
	case TIMER0_ID:

		/*Clear all register of Timer0*/
		TIMER0_TCCR0_REG.Byte=0;
		TIMER0_TCNT0_REG.Byte=0;
		TIMER0_OCR0_REG.Byte=0;

		/*Clear all Interrupt modules Enable of Timer0 (OCIE0=0,TOIE0=0)*/
		TIMER_TIMSK_REG.Bits.OCIE0_Bit=0;
		TIMER_TIMSK_REG.Bits.TOIE0_Bit=0;
		break;

	case TIMER1_ID:

		/*Clear all register of TIMER1_ID_ID_ID*/
		TIMER1_TCCR1A_REG.Byte=0;
		TIMER1_TCCR1B_REG.Byte=0;
		TIMER1_TCNT1_REG.TwoBytes=0;
		TIMER1_OCR1A_REG.TwoBytes=0;

		/*Clear all Interrupt modules Enable of Timer1 (TICIE1=0,OCIE1A=0,OCIE1B=0,TOIE1=0)*/
		TIMER_TIMSK_REG.Byte&=0xC3;
		break;


	case TIMER2_ID:

		/*Clear all register of Timer2*/
		TIMER2_TCCR2_REG.Byte=0;
		TIMER2_TCNT2_REG.Byte=0;
		TIMER2_OCR2_REG.Byte=0;

		/*Clear all Interrupt modules Enable of Timer2 (OCIE2=0,TOIE2=0)*/
		TIMER_TIMSK_REG.Bits.OCIE2_Bit=0;
		TIMER_TIMSK_REG.Bits.TOIE2_Bit=0;
		break;

	}

}
