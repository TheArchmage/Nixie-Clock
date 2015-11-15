/*
This file provides useful functions for handling the initialization of the 
Main Board for the Divergence Meter Project
Date: 8-20-15
Author: Jim Herner
*/

#include "ATMEGA328p_gpio.h"
#include "board_conf.h"
#include <util/delay.h>

int board_init(void)
{
	//Configure output pins for output
	setBit(DDR_LATCHENABLE, BV_LATCHENABLE);
	setBit(DDR_POLARITY, BV_POLARITY);
	setBit(DDR_BLANKING, BV_BLANKING);
	setBit(DDR_DATA, BV_DATA);
	setBit(DDR_CLOCK, BV_CLOCK);
	setBit(DDR_HVPSUENABLE, BV_HVPSUENABLE);
	setBit(DDR_RTCRESET, BV_RTCRESET);
	
	// Configure input pins for input
// 	clearBit(DDR_BUTTON1, BV_BUTTON1);
// 	clearBit(DDR_BUTTON2, BV_BUTTON2);
	
	/*Disable the HV_PSU until the shift registers can be properly configured
	and wait to allow for any residual charge to dissapate */
	disableHVPSU();
	_delay_ms(2000);
	
	/*Set the Shift Registers to an initial configuration which will blank
	the display before initial loading */
	blankDisplay();
	
	//Set the Latchenable to hold until the loader function loads the registers
	setBit(PORT_LATCHENABLE, BV_LATCHENABLE);
	
	//Finally activate the HV_PSU to light the tubes (when properly loaded)
	enableHVPSU();
	return 1;
}

void enableHVPSU(void)
{
	clearBit(PORT_HVPSUENABLE, BV_HVPSUENABLE);
}

void disableHVPSU(void)
{
	setBit(PORT_HVPSUENABLE, BV_HVPSUENABLE);
}

void blankDisplay(void)
{
	clearBit(PORT_BLANKING, BV_BLANKING);
	clearBit(PORT_POLARITY, BV_POLARITY);
}

void unblankDisplay(void)
{	
	setBit(PORT_BLANKING, BV_BLANKING);
	clearBit(PORT_POLARITY, BV_POLARITY);
}