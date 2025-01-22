/******************************************************************************
* Module: UART
*
* File Name: uart.h
*
* Description: Header file for the UART AVR driver
*
* Author: Menna Tullah Medhat
*******************************************************************************/


#ifndef UART_H_
#define UART_H_
#include "std_types.h"

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */

typedef enum {
    UART_5_BIT, UART_6_BIT, UART_7_BIT, UART_8_BIT, UART_9_BIT
} UART_BitDataType;

typedef enum {
    UART_NO_PARITY, UART_EVEN_PARITY, UART_ODD_PARITY
} UART_ParityType;

typedef enum {
    UART_ONE_STOP_BIT, UART_TWO_STOP_BITS
} UART_StopBitType;

typedef uint32 UART_BaudRateType;

/* UART Configuration Structure */
typedef struct {
    UART_BitDataType bit_data;
    UART_ParityType parity;
    UART_StopBitType stop_bit;
    UART_BaudRateType baud_rate;
} UART_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void UART_init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

void UART_sendData(uint8 *array,uint8 arrLength);

void UART_receiveData(uint8 *data,uint8 length);

#endif /* UART_H_ */
