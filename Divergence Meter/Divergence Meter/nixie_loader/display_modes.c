




#include "display_modes.h"
#include "nixie_loader.h"
#include "DS3232_driver.h"
#include <avr/io.h>
#include "board_conf.h"
#include "divergence_numbers.h"
#include "usart_driver.h"

void init_states(void)
{
	prog_divergence[0] = 571024;
	prog_divergence[1] = 571013;
	prog_divergence[2] = 523299;
	prog_divergence[3] = 456903;
	prog_divergence[4] = 409420;
	prog_divergence[5] = 337187;
	prog_divergence[6] = 409431;
	prog_divergence[7] = 456914;
	prog_divergence[8] = 523307;
	prog_divergence[9] = 571046;
	prog_divergence[10] = 1130205;
	
	divergence_rand = 0;
	display_mode_state = DISPLAY_MODE_CLOCK;
	display_mode_substate = 0;
}

void init_button_interrupts(void)
{
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT1) | (1 << PCINT7);
	sei();
}

void display_mode_timer_init(void)
{
	OCR1A = 32;
	TCCR1A |= (1 << WGM01);
	TCCR1B = (1 << CS00) | (1 << CS02);
	TIMSK1 = (1 << OCIE1A);
	TIFR1 = (1 << OCF1A);
	sei();
}

ISR(BUTTON_INTERUPT_VECT)
{
	
	if(bit_is_set(PIN_BUTTON1, BV_BUTTON1)/* & bit_is_clear(PIN_BUTTON2, BV_BUTTON2)*/)
	{
		
		switch(display_mode_state)
		{
			case DISPLAY_MODE_CLOCK:
				display_mode_state = DISPLAY_MODE_DATE;
				// START TIMER TO RETURN TO CLOCK STATE AFTER 30 SECONDS
				break;
			
			case DISPLAY_MODE_DATE:
				display_mode_state = DISPLAY_MODE_SETTINGS;
				break;
				
			case DISPLAY_MODE_SETTINGS:
				if(display_mode_substate == DISPLAY_MODE_SUBSTATE_MAX_SETTINGS)
				{
					display_mode_state = DISPLAY_MODE_CLOCK;
					display_mode_substate = 0;
				}
				else
					display_mode_substate++;
				break;
				
			case DISPLAY_MODE_PROG_DIVERGENCE:
				display_mode_state = DISPLAY_MODE_RAND_DIVERGENCE;
				break;
				
			case DISPLAY_MODE_RAND_DIVERGENCE:
				display_mode_state = DISPLAY_MODE_CUST_DIVERGENCE;
				break;
			
			case DISPLAY_MODE_CUST_DIVERGENCE:
				if(display_mode_substate == DISPLAY_MODE_SUBSTATE_MAX_DIVERGENCE)
				{
					display_mode_state = DISPLAY_MODE_CUST_RAND_DIVERGENCE;
					display_mode_substate = 0;
				}
				break;
			
			case DISPLAY_MODE_CUST_RAND_DIVERGENCE:
				display_mode_state = DISPLAY_MODE_PROG_DIVERGENCE;
				break;
				
		}
	}
	
	if(bit_is_set(PIN_BUTTON2, BV_BUTTON2)/* & bit_is_clear(PIN_BUTTON1, BV_BUTTON1)*/)
	{
		switch(display_mode_state)
		{
			case DISPLAY_MODE_CLOCK:
				display_mode_state = DISPLAY_MODE_PROG_DIVERGENCE;
				break;
			
			case DISPLAY_MODE_DATE:
				display_mode_state = DISPLAY_MODE_CLOCK;
				break;
			
			case DISPLAY_MODE_SETTINGS:
				switch(display_mode_substate)
				{
					case DISPLAY_MODE_SETTINGS_HOURS:
						if(clock_mode_12_24)
						{
							if(time[0]>=1 & time[1]>=2)
								set_hours(1);
							else
								set_hours(time[0]*10+time[1]+1);
						}
						else
						{
							if(time[0]>=2 & time[1]>=3)
								set_hours(0);
							else
								set_hours(time[0]*10+time[1]+1);
						}
						break;
					
					case DISPLAY_MODE_SETTINGS_MINUTES:
						if(time[2]==5 & time[3]==9)
							set_minutes(0);
						else
							set_minutes(time[2]*10+time[3]+1);
						break;
					
					case DISPLAY_MODE_SETTINGS_SECONDS:
						zero_seconds();
						break;
					
					case DISPLAY_MODE_SETTINGS_12_24:
						set_all_mode_12_24(!clock_mode_12_24);
						clock_mode_12_24 = !clock_mode_12_24;
						break;
					
					case DISPLAY_MODE_SETTINGS_MONTH:
						if(date[0]==1 & date[1]==2)
							set_month(1);
						else
							set_month(date[0]*10+date[1]+1);
						break;
					
					case DISPLAY_MODE_SETTINGS_DAY:
						if( date[0]==0 & date[1]==1 |
							date[0]==0 & date[1]==3 |
							date[0]==0 & date[1]==5 |
							date[0]==0 & date[1]==7 |
							date[0]==0 & date[1]==8 |
							date[0]==1 & date[1]==0 |
							date[0]==1 & date[1]==2)
						{
							if(date[2]==3 & date[3]==1)
								set_day(1);
							else
								set_day(date[2]*10+date[3]+1);
						}
						else if(date[0]==0 & date[1]==2)
						{
							if((date[4]*10+date[5])%4 | (!((date[4]*10+date[5])%100) & ((long)(date[4]*10+date[5])%400)) )
							{
								if(date[2]==2 & date[3]==8)
									set_day(1);
								else
									set_day(date[2]*10+date[3]+1);
							}
							else
							{
								if(date[2]==2 & date[3]==9)
									set_day(1);
								else
									set_day(date[2]*10+date[3]+1);
							}
						}
						else
						{
							if(date[2]==3 & date[3]==0)
								set_day(1);
							else
								set_day(date[2]*10+date[3]+1);
						}
						break;
					
					case DISPLAY_MODE_SETTINGS_YEAR:
						if(date[4]==9 & date[5]==9)
							set_year(0);
						else
							set_year(date[4]*10+date[5]+1);
						break;
					
					case DISPLAY_MODE_SETTINGS_MMDD_DDMM:
						if(clock_mode_mmdd_ddmm)
							clock_mode_mmdd_ddmm = 0;
						else
							clock_mode_mmdd_ddmm = 1;
						break;
					
					case DISPLAY_MODE_SETTINGS_BLANKING_HOUR:
						if(clock_mode_12_24)
						{
							if(blanking_hour == 23)
								set_alarm1_time(0, 0, 0, 0);
							else
								set_alarm1_time(blanking_hour+1, 0, 0, 0);
						}
						else
						{
							if(blanking_hour == 12)
								set_alarm1_time(1, 0, 0, alarm1_ampm_bit);
							else
								set_alarm1_time(blanking_hour+1, 0, 0, alarm1_ampm_bit);
						}
						break;
					
					case DISPLAY_MODE_SETTINGS_UNBLANKING_HOUR:
						if(clock_mode_12_24)
						{
							if(unblanking_hour == 23)
								set_alarm2_time(0, 0, 0);
							else
								set_alarm2_time(unblanking_hour+1, 0, 0);
						}
						else
						{
							if(unblanking_hour == 12)
								set_alarm2_time(1, 0, alarm2_ampm_bit);
							else
								set_alarm2_time(unblanking_hour+1, 0, alarm2_ampm_bit);
						}
						break;
						
					case DISPLAY_MODE_SETTINGS_BRIGHTNESS:
						if(global_nixie_brightness == BRIGHTNESS_MAX)
							global_nixie_brightness = 1;
						else
							global_nixie_brightness++;
						break;
					
					case DISPLAY_MODE_SETTINGS_OFFSET_ADJUST:
						if(offset_adjust == OFFSET_ADJUST_MAX)
							set_offset_adjust(0);
						else
							set_offset_adjust(offset_adjust+1);
						break;
				}
				break;
				
			case DISPLAY_MODE_PROG_DIVERGENCE:
				if(display_mode_substate == PROG_DIVERGENCE_MAX)
					display_mode_substate = 0;
				else
					display_mode_substate++;
				break;
			
			case DISPLAY_MODE_RAND_DIVERGENCE:
			//GENERATE NEW RANDOM DIVERGENCE
			
			//PLACEHOLDER
				divergence_rand++;
				break;
			
			case DISPLAY_MODE_CUST_DIVERGENCE:
				if(divergence[display_mode_substate] == 9)
					divergence[display_mode_substate] = 0;
				else
					divergence[display_mode_substate]++;
				break;
			
			case DISPLAY_MODE_CUST_RAND_DIVERGENCE:
				display_mode_state = DISPLAY_MODE_CLOCK;
				break;
		}
	}
}

ISR(DISPLAY_MODE_INT_VECT)
{
	switch(display_mode_state)
	{
		case DISPLAY_MODE_CLOCK:
			get_time_bcd(time, &time_ampm_bit, &clock_mode_12_24);
			if(!(time[4]+time[5]) ||
			 (time[4] == 2 && time[5] == 9) ||
			 (time[4] == 3 && time[5] == 4))
			{
				global_excercise_flag = 1;	
			}
			else if(time[4]==3 && (time[5] < 4))
			{
				get_date_bcd(date);
				global_excercise_flag = 0;
				generate_digits_date(date, global_digits);
			}			
			else
			{
				global_excercise_flag = 0;
				generate_digits_time(time, global_digits);
			}
			break;
			
		case DISPLAY_MODE_DATE:
			get_date_bcd(date);
			generate_digits_date(date, global_digits);				
			break;
			
		case DISPLAY_MODE_SETTINGS:
			switch(display_mode_substate)
			{
				case DISPLAY_MODE_SETTINGS_HOURS:
					get_time_bcd(time, &time_ampm_bit, &clock_mode_mmdd_ddmm);
					generate_digits_settings(DISPLAY_MODE_SETTINGS_HOURS,
											time_ampm_bit,
											time[0]*10+time[1],
											global_digits);
					break;
			
				case DISPLAY_MODE_SETTINGS_MINUTES:
					get_time_bcd(time, &time_ampm_bit, &clock_mode_mmdd_ddmm);
					generate_digits_settings(DISPLAY_MODE_SETTINGS_MINUTES,
											0,
											time[2]*10+time[3],
											global_digits);
					break;
			
				case DISPLAY_MODE_SETTINGS_SECONDS:
					get_time_bcd(time, &time_ampm_bit, &clock_mode_mmdd_ddmm);
					generate_digits_settings(DISPLAY_MODE_SETTINGS_SECONDS,
											0,
											time[4]*10+time[5],
											global_digits);
					break;
			
				case DISPLAY_MODE_SETTINGS_12_24:
					clock_mode_12_24 = get_mode_12_24();
					if(clock_mode_12_24)
						generate_digits_settings(DISPLAY_MODE_SETTINGS_12_24, 0, 12, global_digits);
					else
						generate_digits_settings(DISPLAY_MODE_SETTINGS_12_24, 0, 24, global_digits);
					break;
			
				case DISPLAY_MODE_SETTINGS_MONTH:
					get_date_bcd(date);
					generate_digits_settings(DISPLAY_MODE_SETTINGS_MONTH,
											0,
											date[0]*10+date[1],
											global_digits);
					break;
			
				case DISPLAY_MODE_SETTINGS_DAY:
					get_date_bcd(date);
					generate_digits_settings(DISPLAY_MODE_SETTINGS_DAY,
											0,
											date[2]*10+date[3],
											global_digits);
					break;
					
				case DISPLAY_MODE_SETTINGS_YEAR:
					get_date_bcd(date);
					generate_digits_settings(DISPLAY_MODE_SETTINGS_YEAR,
											0,
											date[4]*10+date[5],
											global_digits);
					break;
			
				case DISPLAY_MODE_SETTINGS_MMDD_DDMM:
					if(clock_mode_mmdd_ddmm)
						generate_digits_settings(DISPLAY_MODE_SETTINGS_MMDD_DDMM, 0, 1, global_digits);
					else
						generate_digits_settings(DISPLAY_MODE_SETTINGS_MMDD_DDMM, 0, 0, global_digits);
					break;
			
				case DISPLAY_MODE_SETTINGS_BLANKING_HOUR:
					blanking_hour = read_blanking_hour();
					generate_digits_settings(DISPLAY_MODE_SETTINGS_BLANKING_HOUR,
											alarm1_ampm_bit,
											blanking_hour,
											global_digits);
					break;
			
				case DISPLAY_MODE_SETTINGS_UNBLANKING_HOUR:
					unblanking_hour = read_unblanking_hour();
					generate_digits_settings(DISPLAY_MODE_SETTINGS_UNBLANKING_HOUR,
											alarm1_ampm_bit,
											unblanking_hour,
											global_digits);
					break;
					
				case DISPLAY_MODE_SETTINGS_BRIGHTNESS:
					generate_digits_settings(DISPLAY_MODE_SETTINGS_BRIGHTNESS,
											0,
											global_nixie_brightness,
											global_digits);
					break;
			
				case DISPLAY_MODE_SETTINGS_OFFSET_ADJUST:
					get_offset_adjust(&offset_adjust);
					generate_digits_settings(DISPLAY_MODE_SETTINGS_OFFSET_ADJUST,
											0,
											offset_adjust,
											global_digits);
					break;
			}
			break;
			
		case DISPLAY_MODE_PROG_DIVERGENCE:

			generate_digits_divergence(prog_divergence[display_mode_substate], global_digits);
			break;
		
		case DISPLAY_MODE_RAND_DIVERGENCE:
			generate_digits_divergence(divergence_rand, global_digits);
			break;
		
		case DISPLAY_MODE_CUST_RAND_DIVERGENCE:
			// PLACEHOLDER
			generate_digits_divergence(0, global_digits);
	}
}

uint8_t read_blanking_hour()
{
	get_alarm1_time(alarm1, &alarm1_ampm_bit, &clock_mode_12_24);
	return alarm1[0]*10+alarm1[1];
}

uint8_t read_unblanking_hour()
{
	get_alarm2_time(alarm2, &alarm2_ampm_bit, &clock_mode_12_24);
	return alarm2[0]*10+alarm2[1];
}