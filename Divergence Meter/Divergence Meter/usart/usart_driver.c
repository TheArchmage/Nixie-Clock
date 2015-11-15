/* This function contains the necessary function 
implementations to bring up and utilize the USART
on the ATmega328p

Date: 8-21-15
Author: Jim Herner
*/

#include <avr/io.h>
#include <stdlib.h>
#include "usart_driver.h"
#include "board_conf.h"
#include <util/setbaud.h>

/* USART Control Functions */
void init_usart(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	#if USE_2X
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &= ~(1 << U2X0);
	#endif
	
	/* Enable Receiving */
	UCSR0B |= (1 << RXEN0);
	
	/* Enable Transmitting */
	UCSR0B |= (1 << TXEN0);
	
	UCSR0C |= ((1 << UCSZ01) | (1 << UCSZ00));
}

void transmit_byte(uint8_t usartbyte)
{
	while( !(UCSR0A & (1 << UDRE0)) )
	;
	UDR0 = usartbyte;
}

uint8_t recieve_byte(void)
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

void print_string(const char myString[])
{
	uint8_t i = 0;
	while (myString[i])
	{
		transmit_byte(myString[i]);
		i++;
	}
}

void print_byte(uint8_t byte)
{
	// converts a byte to a string of decimal text and sends over USART
	transmit_byte('0' + (byte / 100));
	transmit_byte('0' + (byte / 10) % 10);
	transmit_byte('0' + (byte % 10));
}