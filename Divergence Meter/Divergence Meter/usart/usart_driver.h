/* This function contains the necessary function
definitions and other defines needed  to bring up 
and utilize the USART on the ATmega328p

Date: 8-21-15
Author: Jim Herner
*/


#ifndef _USART_DRIVER_H_
#define _USART_DRIVER_H_ 1


#define BAUD 9600

// USART Functions
void init_usart(void);
void transmit_byte(uint8_t byte);
uint8_t recieve_byte(void);
void print_string(const char myString[]);
void print_byte(uint8_t byte);

#endif