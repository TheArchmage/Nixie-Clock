/* This header contains macros and functions for interfacing with the 
GPIO pins of the ATmega328p
Date: 8-20-15
Author: Jim Herner
*/

#ifndef _ATMEGA328P_GPIO_H_
#define _ATMEGA328P_GPIO_H_ 1

#define setBit(byte, bit)		(byte |= _BV(bit))
#define clearBit(byte, bit)		(byte &= ~_BV(bit))
#define toggleBit(byte, bit)	(byte ^= _BV(bit))












#endif