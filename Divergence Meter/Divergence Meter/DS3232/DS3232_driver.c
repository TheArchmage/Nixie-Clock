
#include "DS3232_driver.h"
#include "twi_driver.h"

void get_time_bcd(uint8_t time_bcd[], uint8_t* ampm, uint8_t* mode_12_24)
{
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_SECONDS);
	twi_start();
	twi_send_byte(DS3232_ADDRESS_R);
	uint8_t seconds_bcd = twi_recieve_byte_ack();
	uint8_t minutes_bcd = twi_recieve_byte_ack();
	uint8_t hours_bcd = twi_recieve_byte_noack();
	twi_stop();
	
	//Set the hours 10's place
	if(!(hours_bcd & (1 << DS3232_BV_HOURS_12_24)))
	{
		time_bcd[0] = (hours_bcd & ((1 << DS3232_BV_HOURS_AM_PM_20) | (1 << DS3232_BV_HOURS_10))) >> DS3232_BV_HOURS_10;
		*mode_12_24 = 0;
		*ampm = 0;
	}
	else
	{
		time_bcd[0] = ((hours_bcd & (1 << DS3232_BV_HOURS_10)) >> DS3232_BV_HOURS_10);
		*mode_12_24 = 1;
		*ampm = (hours_bcd & (1 << DS3232_BV_HOURS_AM_PM_20));
	}
	//Set the hours 1's place
	time_bcd[1] = hours_bcd % 16;
	
	//Set the minutes 10's place
	time_bcd[2] = minutes_bcd >> DS3232_BV_MINUTES_10;
	//Set the minutes 1's place
	time_bcd[3] = minutes_bcd % 16;
	
	//Set the seconds 10's place
	time_bcd[4] = seconds_bcd >> DS3232_BV_SECONDS_10;
	//Set the seconds 1's place
	time_bcd[5] = seconds_bcd % 16;
}


void get_date_bcd(uint8_t date_bcd[])
{	
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_DATE);
	twi_start();
	twi_send_byte(DS3232_ADDRESS_R);
	uint8_t day_bcd = twi_recieve_byte_ack();
	uint8_t month_bcd = twi_recieve_byte_ack();
	uint8_t year_bcd = twi_recieve_byte_noack();
	twi_stop();
	
	date_bcd[0] = (month_bcd & (1 << DS3232_BV_MONTH_10)) >> DS3232_BV_MONTH_10;
	date_bcd[1] = month_bcd % 16;
	date_bcd[2] = day_bcd >> DS3232_BV_DATE_10;
	date_bcd[3] = day_bcd % 16;
	date_bcd[4] = year_bcd >> DS3232_BV_YEAR_10;
	date_bcd[5] = year_bcd % 16;
}


void set_hours(uint8_t hours)
{
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_HOURS);
	twi_start();
	twi_send_byte(DS3232_ADDRESS_R);
	uint8_t current = twi_recieve_byte_noack();
	twi_stop();
	
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_HOURS);
	twi_send_byte((current & (1 << DS3232_BV_HOURS_12_24)) | (hours/10 << DS3232_BV_HOURS_10) | (hours % 10));
	twi_stop();
}

void set_minutes(uint8_t minutes)
{
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_MINUTES);
	twi_send_byte(((minutes/10) << DS3232_BV_MINUTES_10) | (minutes % 10));
	twi_stop();
}

void zero_seconds(void)
{
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_SECONDS);
	twi_send_byte(0);
	twi_stop();
}

void set_month(uint8_t month)
{
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_MONTH);
	twi_start();
	twi_send_byte(DS3232_ADDRESS_R);
	uint8_t current = twi_recieve_byte_noack();
	twi_stop();
	
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_MONTH);
	twi_send_byte((current & (1 << DS3232_BV_CENTURY)) |
					((month/10) << DS3232_BV_MONTH_10) |
					(month%10));
	twi_stop();
}

void set_day(uint8_t day)
{
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_DATE);
	twi_send_byte(((day / 10) << DS3232_BV_DATE_10) | (day % 10));
	twi_stop();
}

void set_year(uint8_t year)
{
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_YEAR);
	twi_send_byte(((year/10) <<  DS3232_BV_YEAR_10) | (year % 10));
	twi_stop();
}

void set_all_mode_12_24(uint8_t mode)
{
	uint8_t time_bcd[6];
	uint8_t time_ampm;
	uint8_t time_mode_12_24;
	get_time_bcd(time_bcd, &time_ampm, &time_mode_12_24);
	
	uint8_t alarm1_bcd[6];
	uint8_t alarm1_ampm;
	uint8_t alarm1_mode_12_24;
	get_alarm1_time(alarm1_bcd, &alarm1_ampm, &alarm1_mode_12_24);
	
	uint8_t alarm2_bcd[4];
	uint8_t alarm2_ampm;
	uint8_t alarm2_mode_12_24;
	get_alarm2_time(alarm2_bcd, &alarm2_ampm, &alarm2_mode_12_24);
	
	uint8_t hours;
	
	if(mode)
	{
		if(!time_mode_12_24)
		{
			if((time_bcd[0]*10+time_bcd[1]) > 12)
			{
				hours = time_bcd[0]*10 + time_bcd[1] - 12;
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_HOURS);
				twi_send_byte((1 << DS3232_BV_HOURS_12_24) | (1 << DS3232_BV_HOURS_AM_PM_20) | hours);
				twi_stop();
			}
			else if((time_bcd[0] == 0) & (time_bcd[1] == 0))
			{
				hours = 12;
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_HOURS);
				twi_send_byte((1 << DS3232_BV_HOURS_12_24) | hours);
				twi_stop();
			}
			else
			{
				hours = time_bcd[0]*10 + time_bcd[1];				
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_HOURS);
				twi_send_byte((1 << DS3232_BV_HOURS_12_24) | hours);
				twi_stop();
			}
			
			if((alarm1_bcd[0]*10+alarm1_bcd[1]) > 12)
			{
				hours = alarm1_bcd[0]*10 + alarm1_bcd[1] - 12;
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_ALARM1_HOUR);
				twi_send_byte((1 << DS3232_BV_HOURS_12_24) | (1 << DS3232_BV_HOURS_AM_PM_20) | hours);
				twi_stop();
			}
			else if((alarm1_bcd[0] == 0) & (alarm1_bcd[1] == 0))
			{
				hours = 12;
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_ALARM1_HOUR);
				twi_send_byte((1 << DS3232_BV_HOURS_12_24) | hours);
				twi_stop();			
			}
			else
			{
				hours = alarm1_bcd[0]*10 + alarm1_bcd[1];
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_ALARM1_HOUR);
				twi_send_byte((1 << DS3232_BV_HOURS_12_24) | hours);
				twi_stop();
			}

			
			if((alarm2_bcd[0]*10+alarm2_bcd[1]) > 12)
			{
				hours = alarm2_bcd[0]*10 + alarm2_bcd[1] - 12;
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_ALARM2_HOUR);
				twi_send_byte((1 << DS3232_BV_HOURS_12_24) | (1 << DS3232_BV_HOURS_AM_PM_20) | hours);
				twi_stop();
			}
			else if((alarm2_bcd[0] == 0) & (alarm2_bcd[1] == 0))
			{
				hours = 12;
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_ALARM2_HOUR);
				twi_send_byte((1 << DS3232_BV_HOURS_12_24) | hours);
				twi_stop();
			}
			else
			{
				hours = alarm2_bcd[0]*10 + alarm2_bcd[1];
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_ALARM1_HOUR);
				twi_send_byte((1 << DS3232_BV_HOURS_12_24) | hours);
				twi_stop();
			}
		}
	}
	else
	{
		if(time_mode_12_24)
		{
			if(time_ampm & ((time_bcd[0]*10+time_bcd[1]) != 12))
			{
				hours = time_bcd[0]*10+time_bcd[1] + 12;
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_HOURS);
				twi_send_byte(hours);
				twi_stop();				
			}
			else if(!time_ampm & ((time_bcd[0]*10+time_bcd[1]) == 12))
			{
				hours = 0;
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_HOURS);
				twi_send_byte(hours);
				twi_stop();				
			}
			else
			{
				hours = time_bcd[0]*10+time_bcd[1];
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_HOURS);
				twi_send_byte(hours);
				twi_stop();
			}
			
			if(alarm1_ampm & ((alarm1_bcd[0]*10+alarm1_bcd[1]) != 12))
			{
				hours = alarm1_bcd[0]*10+alarm1_bcd[1] + 12;
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_ALARM1_HOUR);
				twi_send_byte(hours);
				twi_stop();
			}
			else if(!alarm1_ampm & ((alarm1_bcd[0]*10+alarm1_bcd[1]) == 12))
			{
				hours = 0;
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_ALARM1_HOUR);
				twi_send_byte(hours);
				twi_stop();
			}
			else
			{
				hours = alarm1_bcd[0]*10+alarm1_bcd[1];
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_ALARM1_HOUR);
				twi_send_byte(hours);
				twi_stop();
			}
			
			if(alarm2_ampm & ((alarm2_bcd[0]*10+alarm2_bcd[1]) != 12))
			{
				hours = alarm2_bcd[0]*10+alarm2_bcd[1] + 12;
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_ALARM2_HOUR);
				twi_send_byte(hours);
				twi_stop();
			}
			else if(!alarm2_ampm & ((alarm2_bcd[0]*10+alarm2_bcd[1]) == 12))
			{
				hours = 0;
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_ALARM2_HOUR);
				twi_send_byte(hours);
				twi_stop();
			}
			else
			{
				hours = alarm2_bcd[0]*10+alarm2_bcd[1];
				twi_start();
				twi_send_byte(DS3232_ADDRESS_W);
				twi_send_byte(DS3232_PTR_ALARM2_HOUR);
				twi_send_byte(hours);
				twi_stop();
			}
		}
	}
}

uint8_t get_mode_12_24(void)
{
	uint8_t time_bcd[6];
	uint8_t time_ampm;
	uint8_t time_mode_12_24;
	get_time_bcd(time_bcd, &time_ampm, &time_mode_12_24);
	return time_mode_12_24;
}

void get_alarm1_time(uint8_t alarm1_bcd[], uint8_t* ampm, uint8_t* mode_12_24)
{
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_ALARM1_SEC);
	twi_start();
	twi_send_byte(DS3232_ADDRESS_R);
	uint8_t seconds_bcd = twi_recieve_byte_ack();
	uint8_t minutes_bcd = twi_recieve_byte_ack();
	uint8_t hours_bcd = twi_recieve_byte_noack();
	twi_stop();
	
	//Set the hours 10's place
	if(hours_bcd & (1 << DS3232_BV_HOURS_12_24))
	{
		alarm1_bcd[0] = (hours_bcd & ((1 << DS3232_BV_HOURS_AM_PM_20) | (1 << DS3232_BV_HOURS_10))) >> DS3232_BV_HOURS_10;
		*mode_12_24 = 1;
		*ampm = 0;
	}
	else
	{
		alarm1_bcd[0] = (hours_bcd & (1 << DS3232_BV_HOURS_10));
		*mode_12_24 = 0;
		*ampm = (hours_bcd & (1 << DS3232_BV_HOURS_AM_PM_20));
	}
	//Set the hours 1's place
	alarm1_bcd[1] = hours_bcd % 16;
	
	//Set the minutes 10's place
	alarm1_bcd[2] = minutes_bcd >> DS3232_BV_MINUTES_10;
	//Set the minutes 1's place
	alarm1_bcd[3] = minutes_bcd % 16;
	
	//Set the seconds 10's place
	alarm1_bcd[4] = seconds_bcd >> DS3232_BV_SECONDS_10;
	//Set the seconds 1's place
	alarm1_bcd[5] = seconds_bcd % 16;
}

void get_alarm2_time(uint8_t alarm2_bcd[], uint8_t* ampm, uint8_t* mode_12_24)
{
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_ALARM2_MIN);
	twi_start();
	twi_send_byte(DS3232_ADDRESS_R);
	uint8_t minutes_bcd = twi_recieve_byte_ack();
	uint8_t hours_bcd = twi_recieve_byte_noack();
	twi_stop();
	
	//Set the hours 10's place
	if(hours_bcd & (1 << DS3232_BV_HOURS_12_24))
	{
		alarm2_bcd[0] = (hours_bcd & ((1 << DS3232_BV_HOURS_AM_PM_20) | (1 << DS3232_BV_HOURS_10))) >> DS3232_BV_HOURS_10;
		*mode_12_24 = 1;
		*ampm = 0;
	}
	else
	{
		alarm2_bcd[0] = (hours_bcd & (1 << DS3232_BV_HOURS_10));
		*mode_12_24 = 0;
		*ampm = (hours_bcd & (1 << DS3232_BV_HOURS_AM_PM_20));
	}
	//Set the hours 1's place
	alarm2_bcd[1] = hours_bcd % 16;
	
	//Set the minutes 10's place
	alarm2_bcd[2] = minutes_bcd >> DS3232_BV_MINUTES_10;
	//Set the minutes 1's place
	alarm2_bcd[3] = minutes_bcd % 16;
}

void set_alarm1_time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t ampm)
{
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_ALARM1_HOUR);
	twi_start();
	twi_send_byte(DS3232_ADDRESS_R);
	uint8_t current = twi_recieve_byte_noack();
	twi_stop();
	
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_ALARM1_SEC);
	twi_send_byte(((seconds/10) << DS3232_BV_SECONDS_10) | (seconds%10));
	twi_send_byte(((minutes/10) << DS3232_BV_MINUTES_10) | (minutes%10));
	if(current & (1 << DS3232_BV_HOURS_12_24))
	{
		twi_send_byte((1 << DS3232_BV_HOURS_12_24) | 
					 (ampm << DS3232_BV_HOURS_AM_PM_20) | 
					 ((hours/10) << DS3232_BV_HOURS_10) | 
					 (hours%10));
	}
	else
	{
		twi_send_byte(((hours/10) << DS3232_BV_HOURS_10) |
						(hours%10));
	}
	twi_send_byte(1 << DS3232_BV_AM1M4);
	twi_stop();
}

void set_alarm2_time(uint8_t hours, uint8_t minutes, uint8_t ampm)
{
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_ALARM2_HOUR);
	twi_start();
	twi_send_byte(DS3232_ADDRESS_R);
	uint8_t current = twi_recieve_byte_noack();
	twi_stop();
	
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_ALARM2_MIN);
	twi_send_byte(((minutes/10) << DS3232_BV_MINUTES_10) | (minutes%10));
	if(current & (1 << DS3232_BV_HOURS_12_24))
	{
		twi_send_byte((1 << DS3232_BV_HOURS_12_24) |
		(ampm << DS3232_BV_HOURS_AM_PM_20) |
		((hours/10) << DS3232_BV_HOURS_10) |
		(hours%10));
	}
	else
	{
		twi_send_byte(((hours/10) << DS3232_BV_HOURS_10) |
		(hours%10));
	}
	twi_send_byte(1 << DS3232_BV_AM2M4);
	twi_stop();
}

void set_offset_adjust(uint8_t offset)
{
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_AGING_OFFSET);
	twi_send_byte(offset);
	twi_stop();
}

void get_offset_adjust(uint8_t* aging_offset)
{
	twi_start();
	twi_send_byte(DS3232_ADDRESS_W);
	twi_send_byte(DS3232_PTR_AGING_OFFSET);
	twi_start();
	twi_send_byte(DS3232_ADDRESS_R);
	uint8_t offset = twi_recieve_byte_noack();
	*aging_offset = offset & 0b01111111;
}





