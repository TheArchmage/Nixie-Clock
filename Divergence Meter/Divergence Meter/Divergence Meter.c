/*
 * Divergence_Meter.cpp
 *
 * Created: 8/19/2015 5:42:05 PM
 *  Author: Jim
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include "board_conf.h"
#include "nixie_loader.h"
#include "divergence_numbers.h"
#include "usart_driver.h"
#include "ATMEGA328p_gpio.h"
#include "DS3232_driver.h"


int main(void)
{
	//Initialize board hardware
	board_init();
	
    while(1)
    {

    }
}
