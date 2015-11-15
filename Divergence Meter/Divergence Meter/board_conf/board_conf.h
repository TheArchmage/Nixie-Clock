/*
This file contains the necessary pin definitions and other 
important device-specific macros for the Divergence Meter 
Project
Date: 8-19-15
Author: Jim Herner
*/
#include <avr/io.h>

#ifndef _BOARD_CONF_H_
#define _BOARD_CONF_H_ 1


#define F_CPU 8000000

// Microcontroller pin definitions
// REMEMBER PIN IS FOR INPUT, PORT IS FOR OUTPUT

//Input Buttons
#define BV_BUTTON1			PINB1
#define PIN_BUTTON1			PINB
#define DDR_BUTTON1			DDRB

#define BV_BUTTON2			PINB7
#define PIN_BUTTON2			PINB
#define DDR_BUTTON2			DDRB

//Output Pins
#define BV_LATCHENABLE		PORTD6
#define PORT_LATCHENABLE	PORTD
#define DDR_LATCHENABLE		DDRD

#define BV_POLARITY			PORTD7
#define PORT_POLARITY		PORTD
#define DDR_POLARITY		DDRD

#define BV_BLANKING			PORTC2
#define PORT_BLANKING		PORTC
#define DDR_BLANKING		DDRC

#define BV_DATA				PORTC1
#define PORT_DATA			PORTC
#define DDR_DATA			DDRC

#define BV_CLOCK			PORTC0
#define PORT_CLOCK			PORTC
#define DDR_CLOCK			DDRC

#define BV_HVPSUENABLE		PORTB0
#define PORT_HVPSUENABLE	PORTB
#define DDR_HVPSUENABLE		DDRB

#define BV_RTCRESET			PORTD5
#define PORT_RTCRESET		PORTD
#define DDR_RTCRESET		DDRD

#define PIN_SCL				PORTC5
#define PIN_SDA				PORTC4


/* This function brings up the Main-Board of the Divergence meter

*/
int board_init(void);

/* These functions toggle the appropriate bits to enable and disable the HV 
PSU as necessary.
*/
void enableHVPSU(void);
void disableHVPSU(void);






#endif


