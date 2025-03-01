/******************************************************************************
* Module: TIMER
*
* File Name: timer.h
*
* Description:  Private Header File For Timer 0 , Timer 1 , Timer 2 Registers
*               Definition and type structure declaration
*
* Author: Menna Tullah Medhat
*******************************************************************************/

#ifndef TIMER_PRIVATE_H_
#define TIMER_PRIVATE_H_

#include "std_types.h"

/****************************************************************************************************
 *       Registers type structure declaration for common registers in timer0,timer1,timer2		    *
 ***************************************************************************************************/
typedef union
{
	uint8 Byte;
	struct
	{
		uint8 TOIE0_Bit :1;
		uint8 OCIE0_Bit :1;
		uint8 TOIE1_Bit :1;
		uint8 OCIE1B_Bit :1;
		uint8 OCIE1A_Bit :1;
		uint8 TICIE1_Bit :1;
		uint8 TOIE2_Bit :1;
		uint8 OCIE2_Bit :1;
	} Bits;
} Timers_TIMSK_Type;

typedef union
{
	uint8 Byte;
	struct
	{
		uint8 TOV0_Bit :1;
		uint8 OCF0_Bit :1;
		uint8 TOV1_Bit :1;
		uint8 OCF1B_Bit :1;
		uint8 OCF1A_Bit :1;
		uint8 ICF1_Bit :1;
		uint8 TOV2_Bit :1;
		uint8 OCF2_Bit :1;
	} Bits;
} Timers_TIFR_Type;



/********************************************************************
 *       Registers type structure declaration for timer0            *
 ********************************************************************/
typedef union
{
	uint8 Byte;
	struct
	{
		uint8 CS00_Bit :1;
		uint8 CS01_Bit :1;
		uint8 CS02_Bit :1;
		uint8 WGM01_Bit :1;
		uint8 COM00_Bit :1;
		uint8 COM01_Bit :1;
		uint8 WGM00_Bit :1;
		uint8 FOC0_Bit :1;
	} Bits;
} Timer0_TCCR0_Type;

typedef union
{
	uint8 Byte;
	struct
	{
		uint8 Bit0 :1;
		uint8 Bit1 :1;
		uint8 Bit2 :1;
		uint8 Bit3 :1;
		uint8 Bit4 :1;
		uint8 Bit5 :1;
		uint8 Bit6 :1;
		uint8 Bit7 :1;
	} Bits;
} Timer0_TCNT0_Type;

typedef union
{
	uint8 Byte;
	struct
	{
		uint8 Bit0 :1;
		uint8 Bit1 :1;
		uint8 Bit2 :1;
		uint8 Bit3 :1;
		uint8 Bit4 :1;
		uint8 Bit5 :1;
		uint8 Bit6 :1;
		uint8 Bit7 :1;
	} Bits;
} Timer0_OCR0_Type;


/*******************************************************************************
 *                  Registers type structure declaration for timer1            *
 *******************************************************************************/
typedef union
{
	uint8 Byte;
	struct
	{
		uint8 WGM10_Bit :1;
		uint8 WGM11_Bit :1;
		uint8 FOC1A_Bit :1;
		uint8 FOC1B_Bit :1;
		uint8 COM1B0_Bit :1;
		uint8 COM1B1_Bit :1;
		uint8 COM1A0_Bit :1;
		uint8 COM1A1_Bit :1;
	} Bits;
} Timer1_TCCR1A_Type;

typedef union
{
	uint8 Byte;
	struct
	{
		uint8 CS10_Bit :1;
		uint8 CS11_Bit :1;
		uint8 CS12_Bit :1;
		uint8 WGM12_Bit :1;
		uint8 WGM13_Bit :1;
		uint8 :1;
		uint8 ICES1_Bit :1;
		uint8 ICNC1_Bit :1;
	} Bits;
} Timer1_TCCR1B_Type;

typedef union
{
	uint16 TwoBytes;
	struct
	{
		uint16 Bit0 :1;
		uint16 Bit1 :1;
		uint16 Bit2 :1;
		uint16 Bit3 :1;
		uint16 Bit4 :1;
		uint16 Bit5 :1;
		uint16 Bit6 :1;
		uint16 Bit7 :1;
		uint16 Bit8 :1;
		uint16 Bit9 :1;
		uint16 Bit10 :1;
		uint16 Bit11 :1;
		uint16 Bit12 :1;
		uint16 Bit13 :1;
		uint16 Bit14 :1;
		uint16 Bit15 :1;
	} Bits;
} Timer1_TCNT1_Type;

typedef union
{
	uint16 TwoBytes;
	struct
	{
		uint16 Bit0 :1;
		uint16 Bit1 :1;
		uint16 Bit2 :1;
		uint16 Bit3 :1;
		uint16 Bit4 :1;
		uint16 Bit5 :1;
		uint16 Bit6 :1;
		uint16 Bit7 :1;
		uint16 Bit8 :1;
		uint16 Bit9 :1;
		uint16 Bit10 :1;
		uint16 Bit11 :1;
		uint16 Bit12 :1;
		uint16 Bit13 :1;
		uint16 Bit14 :1;
		uint16 Bit15 :1;
	} Bits;
} Timer1_OCR1A_Type;

typedef union
{
	uint16 TwoBytes;
	struct
	{
		uint16 Bit0 :1;
		uint16 Bit1 :1;
		uint16 Bit2 :1;
		uint16 Bit3 :1;
		uint16 Bit4 :1;
		uint16 Bit5 :1;
		uint16 Bit6 :1;
		uint16 Bit7 :1;
		uint16 Bit8 :1;
		uint16 Bit9 :1;
		uint16 Bit10 :1;
		uint16 Bit11 :1;
		uint16 Bit12 :1;
		uint16 Bit13 :1;
		uint16 Bit14 :1;
		uint16 Bit15 :1;
	} Bits;
} Timer1_OCR1B_Type;

typedef union
{
	uint16 TwoBytes;
	struct
	{
		uint16 Bit0 :1;
		uint16 Bit1 :1;
		uint16 Bit2 :1;
		uint16 Bit3 :1;
		uint16 Bit4 :1;
		uint16 Bit5 :1;
		uint16 Bit6 :1;
		uint16 Bit7 :1;
		uint16 Bit8 :1;
		uint16 Bit9 :1;
		uint16 Bit10 :1;
		uint16 Bit11 :1;
		uint16 Bit12 :1;
		uint16 Bit13 :1;
		uint16 Bit14 :1;
		uint16 Bit15 :1;
	} Bits;
} Timer1_ICR1_Type;

/*******************************************************************************
 *                  Registers type structure declaration for timer2            *
 *******************************************************************************/
typedef union
{
	uint8 Byte;
	struct
	{
		uint8 CS20_Bit :1;
		uint8 CS21_Bit :1;
		uint8 CS22_Bit :1;
		uint8 WGM21_Bit :1;
		uint8 COM20_Bit :1;
		uint8 COM21_Bit :1;
		uint8 WGM20_Bit :1;
		uint8 FOC2_Bit :1;
	} Bits;
} Timer2_TCCR2_Type;

typedef union
{
	uint8 Byte;
	struct
	{
		uint8 Bit0 :1;
		uint8 Bit1 :1;
		uint8 Bit2 :1;
		uint8 Bit3 :1;
		uint8 Bit4 :1;
		uint8 Bit5 :1;
		uint8 Bit6 :1;
		uint8 Bit7 :1;
	} Bits;
} Timer2_TCNT2_Type;

typedef union
{
	uint8 Byte;
	struct
	{
		uint8 Bit0 :1;
		uint8 Bit1 :1;
		uint8 Bit2 :1;
		uint8 Bit3 :1;
		uint8 Bit4 :1;
		uint8 Bit5 :1;
		uint8 Bit6 :1;
		uint8 Bit7 :1;
	} Bits;
} Timer2_OCR2_Type;


/***************************************************************************************************************
 * Registers definitions for each type struct for common registers for timers in the memory mapped Registers   *
 ***************************************************************************************************************/
#define TIMER_TIFR_REG      (*((volatile Timers_TIFR_Type* const)0x0058))
#define TIMER_TIMSK_REG     (*((volatile Timers_TIMSK_Type* const)0x0059))


/******************************************************************************************
 * Registers definitions for each type struct for timer0 in the memory mapped Registers   *
 ******************************************************************************************/
#define TIMER0_TCCR0_REG     (*((volatile Timer0_TCCR0_Type* const )0x0053))
#define TIMER0_TCNT0_REG     (*((volatile Timer0_TCNT0_Type* const )0x0052))
#define TIMER0_OCR0_REG     (*((volatile Timer0_OCR0_Type* const )0x005C))


/******************************************************************************************
 * Registers definitions for each type struct for timer1 in the memory mapped Registers   *
 ******************************************************************************************/
#define TIMER1_TCNT1_REG     (*((volatile Timer1_TCNT1_Type* const )0x004C))
#define TIMER1_TCCR1A_REG    (*((volatile Timer1_TCCR1A_Type* const)0x004F))
#define TIMER1_TCCR1B_REG    (*((volatile Timer1_TCCR1B_Type* const)0x004E))
#define TIMER1_OCR1A_REG     (*((volatile Timer1_OCR1A_Type* const)0x004A))
#define TIMER1_OCR1B_REG     (*((volatile Timer1_OCR1B_Type* const)0x0048))
#define TIMER1_ICR1_REG      (*((volatile Timer1_ICR1_Type* const)0x0046))

/******************************************************************************************
 * Registers definitions for each type struct for timer2 in the memory mapped Registers   *
 ******************************************************************************************/
#define TIMER2_TCCR2_REG     (*((volatile Timer2_TCCR2_Type* const )0x0045))
#define TIMER2_TCNT2_REG     (*((volatile Timer2_TCNT2_Type* const )0x0044))
#define TIMER2_OCR2_REG     (*((volatile Timer2_OCR2_Type* const )0x0043))


#endif /* TIMER_PRIVATE_H_ */
