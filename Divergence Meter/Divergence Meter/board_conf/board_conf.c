/*
This file provides useful functions for handling the initialization of the 
Main Board for the Divergence Meter Project
Date: 8-20-15
Author: Jim Herner
*/

#include "ATMEGA328p_gpio.h"
#include "board_conf.h"
#include <util/delay.h>
#include "usart_driver.h"
#include "display_modes.h"

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
	clearBit(DDR_BUTTON1, BV_BUTTON1);
	clearBit(DDR_BUTTON2, BV_BUTTON2);
	
	/*Disable the HV_PSU until the shift registers can be properly configured
	and wait to allow for any residual charge to dissapate */
	disableHVPSU();
	_delay_ms(2000);
	
	/*Set the Shift Registers to an initial configuration which will blank
	the display before initial loading */
	blankDisplay();
	
	//Set the Latchenable to hold until the loader function loads the registers
	clearBit(PORT_LATCHENABLE, BV_LATCHENABLE);
	
	//Initialize the debug USART
	init_usart();
	
	//Initialize TWI for the RTC
	init_twi();
	
	//Finally activate the HV_PSU to light the tubes (when properly loaded)
	enableHVPSU();
	
	init_display_vars();
	init_states();
	
	get_time_bcd(time, &time_ampm_bit, &clock_mode_12_24);
	
	display_refresh_timer_init();
	display_mode_timer_init();
	init_button_interrupts();
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

