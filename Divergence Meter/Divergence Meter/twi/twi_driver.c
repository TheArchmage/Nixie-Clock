/* This file contains function implementations and other
necessary information for implementing the TWI hardware
on the ATmega328p.

Date: 8-21-15
Author: Jim Herner
*/

#include <avr/io.h>
#include <util/twi.h>

/* TWI Control Functions */
void init_twi(void)
{
	// For an 8MHz System Clock, this Bus will run at 100kHz
	TWBR = 32;
	
	// And we enable the interface
	TWCR |= (1 << TWEN);
}

void twi_start(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	twi_wait_for_complete();
}

void twi_send_byte(uint8_t byte)
{
	TWDR = byte;
	TWCR = (1 << TWINT) | (1 << TWEN);
	twi_wait_for_complete();
}

uint8_t twi_recieve_byte_ack(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	twi_wait_for_complete();
	return(TWDR);
}

uint8_t twi_recieve_byte_noack(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN);
	twi_wait_for_complete();
	return(TWDR);
}

void twi_stop(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void twi_wait_for_complete(void)
{
	while (!(TWCR & (1<<TWINT)))
	;
}