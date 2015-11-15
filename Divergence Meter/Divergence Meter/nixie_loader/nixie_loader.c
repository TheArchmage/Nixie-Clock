

#include "ATMEGA328p_gpio.h"
#include "board_conf.h"
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "nixie_loader.h"
#include "divergence_numbers.h"
#include "usart_driver.h"
#include "display_modes.h"




void init_display_vars(void)
{
	global_nixie_brightness_counter = 0;
	global_nixie_brightness = 7;
	int i;
	for(i=0; i<12; i++)
		global_bitstream[i] = 0;
	for(i=0; i<8; i++)
		global_mapped_digits[i] = 0;
	for(i=0; i<8; i++)
		global_digits[i] = 0;
		
	global_excercise_flag = 0;
	global_excercise_counter = 0;
}

ISR(TIMER0_COMPA_vect)
{
	if(global_nixie_brightness_counter <= global_nixie_brightness)
	{
		if(global_excercise_flag & (global_excercise_counter==10))
		{
			display_excercise(global_digits);
		}
		global_nixie_brightness_counter++;
		map_digits(global_digits, global_mapped_digits);
		generate_bitstream(global_bitstream, global_mapped_digits);
		load_latches(global_bitstream);
		unblankDisplay();
	}
	else 
	{
		global_nixie_brightness_counter++;
		blankDisplay();
	}
	if(global_nixie_brightness_counter == 8)
		global_nixie_brightness_counter = 0;
	
	if(global_excercise_counter==10)	
		global_excercise_counter=0;
	else
		global_excercise_counter++;
		
}


void display_refresh_timer_init(void)
{
	OCR0A = 15;
	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);
	TCCR0B = (1 << CS00) | (1 << CS02);
	TIMSK0 = (1 << OCIE0A);
	TIFR0 = (1 << OCF0A);
	sei();
}

void latchHold(void)
{
	clearBit(PORT_LATCHENABLE, BV_LATCHENABLE);
}

void latchDump(void)
{
	setBit(PORT_LATCHENABLE, BV_LATCHENABLE);
}

void cycleClock(void)
{
	setBit(PORT_CLOCK, BV_CLOCK);
	clearBit(PORT_CLOCK, BV_CLOCK);
	setBit(PORT_CLOCK, BV_CLOCK);
}

void display_excercise(int* digits)
{
	int i;

	for(i=0;i<8;i++)
	{
		if(digits[i]>=11)
		{
			digits[i]=0;
		}
		else
		{
			digits[i]++;
		}
	}
}

void generate_digits_divergence(uint32_t divergence, int* digits)
{
	int i;
	for(i=0; i < 8; i++)
	{
		if(i!=6){
			digits[7-i] = divergence%(long)10;
			divergence = divergence/10;
		}	
	}
	digits[1] = 11;
	
}

void generate_digits_time(uint8_t time[], int* digits)
{
	digits[0] = time[0];
	digits[1] = time[1];
	digits[2] = 10+time[5]%2;
	digits[3] = time[2];
	digits[4] = time[3];
	digits[5] = 10+time[5]%2;
	digits[6] = time[4];
	digits[7] = time[5];
}

void generate_digits_settings(uint8_t setting, uint8_t ampm, uint8_t value, int* digits)
{
	digits[0] = (setting+1)/10;
	digits[1] = (setting+1)%10;
	
	int i;
	for(i=2;i<5;i++)
		digits[i] = 12;
	
	if(!ampm)
		digits[5] = 12;
	else
		digits[5] = 11;
		
	digits[6] = value/10;
	digits[7] = value%10;
}

void generate_digits_date(uint8_t date[], int* digits)
{
	digits[0] = date[0];
	digits[1] = date[1];
	digits[2] = 12;
	digits[3] = date[2];
	digits[4] = date[3];
	digits[5] = 12;
	digits[6] = date[4];
	digits[7] = date[5];
}

void map_digits(int digits[], int mapped_digits[])
{
	static const uint8_t Nixie[8][12]={
		{2, 11, 10, 9, 8, 7, 6, 5, 4, 3, 13, 1},
		{14, 24, 23, 21, 22, 20, 18, 16, 17, 15, 19, 12},
		{26, 35, 34, 33, 32, 31, 30, 29, 28, 27, 36, 25},
		{38, 47, 46, 45, 44, 43, 42, 41, 40, 39, 48, 37},
		{50, 59, 58, 57, 56, 55, 54, 53, 52, 51, 60, 49},
		{62, 71, 70, 69, 68, 67, 66, 65, 64, 63, 72, 61},
		{74, 83, 79, 81, 82, 80, 78, 76, 77, 75, 84, 73},
		{85, 95, 94, 93, 92, 91, 90, 89, 88, 87, 96, 86}
	};
	
	int i;
	for(i=0;i<8;i++)
	{
		if(digits[i]!=12)
			mapped_digits[i] = Nixie[7-i][digits[i]];
		else
			mapped_digits[i] = 0;
	}
}

void generate_bitstream(uint8_t bitstream[], int digits[])
{
	int i;
	for(i = 0; i < 12; i++)
		bitstream[i] = 0x00;
	for(i = 0; i < 8; i++)
		if(digits[i])
			setBit(bitstream[(96-digits[i])/8], (digits[i]-1)%8);
}

void load_latches(char bitstream[12])
{
	latchHold();
	int i, j;
	for(i = 0; i < 12; i++)
	{
		for(j = 7; j >= 0; j--)
		{
			if(!(bitstream[i] & (1 << j)))
			{
				clearBit(PORT_DATA, BV_DATA);
				cycleClock();
			}
			else
			{
				setBit(PORT_DATA, BV_DATA);
				cycleClock();
			}
		}
	}
	latchDump();
	latchHold();
}

void blankDisplay(void)
{
	clearBit(PORT_BLANKING, BV_BLANKING);
	setBit(PORT_POLARITY, BV_POLARITY);
}

void unblankDisplay(void)
{
	setBit(PORT_BLANKING, BV_BLANKING);
	setBit(PORT_POLARITY, BV_POLARITY);
}