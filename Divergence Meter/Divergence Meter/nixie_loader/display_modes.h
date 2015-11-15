/* This file contains the function definitions and preprocessor commands 
associated with the various display modes of the Divergence Meter.

Date:n 8-24-15
Author: Jim Herner
*/

#ifndef _DISPLAY_MODES_H_
#define _DISPLAY_MODES_H_ 1

#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#define BUTTON_INTERUPT_VECT	PCINT0_vect	
#define DISPLAY_MODE_INT_VECT	TIMER1_COMPA_vect

#define OFFSET_ADJUST_MAX 12

#define DISPLAY_MODE_CLOCK						0
#define DISPLAY_MODE_DATE						1
#define DISPLAY_MODE_SETTINGS					2
#define DISPLAY_MODE_PROG_DIVERGENCE			3
#define DISPLAY_MODE_RAND_DIVERGENCE			4
#define DISPLAY_MODE_CUST_DIVERGENCE			5
#define DISPLAY_MODE_CUST_RAND_DIVERGENCE		6

#define DISPLAY_MODE_SUBSTATE_MAX_SETTINGS		11
#define DISPLAY_MODE_SUBSTATE_MAX_DIVERGENCE	5

#define DISPLAY_MODE_SETTINGS_HOURS				0
#define DISPLAY_MODE_SETTINGS_MINUTES			1
#define DISPLAY_MODE_SETTINGS_SECONDS			2
#define DISPLAY_MODE_SETTINGS_12_24				3
#define DISPLAY_MODE_SETTINGS_MONTH				4
#define DISPLAY_MODE_SETTINGS_DAY				5
#define DISPLAY_MODE_SETTINGS_YEAR				6
#define DISPLAY_MODE_SETTINGS_MMDD_DDMM			7
#define DISPLAY_MODE_SETTINGS_BLANKING_HOUR		8
#define DISPLAY_MODE_SETTINGS_UNBLANKING_HOUR	9
#define DISPLAY_MODE_SETTINGS_BRIGHTNESS		10
#define DISPLAY_MODE_SETTINGS_OFFSET_ADJUST		11

volatile int display_mode_state;
volatile int display_mode_substate;


volatile uint8_t time[6];
volatile uint8_t date[6];
volatile uint8_t alarm1[6];
volatile uint8_t alarm2[4];
volatile uint8_t time_ampm_bit;
volatile uint8_t alarm1_ampm_bit;
volatile uint8_t alarm2_ampm_bit;
volatile uint8_t clock_mode_12_24;
volatile uint8_t clock_mode_mmdd_ddmm;
volatile uint8_t blanking_hour;
volatile uint8_t unblanking_hour;
volatile uint8_t offset_adjust;

long prog_divergence[11];

volatile uint8_t divergence[7];
volatile long divergence_rand;



uint8_t read_blanking_hour(void);
uint8_t read_unblanking_hour(void);

void display_mode_timer_init(void);
void init_button_interrupts(void);

void init_states(void);


#endif