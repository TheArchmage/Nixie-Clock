
#ifndef _DS3232_DRIVER_H_
#define _DS3232_DRIVER_H_ 1

#include <stdint.h>

//I2C register addresses
#define DS3232_ADDRESS_W			0b11010000
#define DS3232_ADDRESS_R			0b11010001

#define DS3232_PTR_SECONDS			0x00
	#define DS3232_BV_SECONDS_10		4
	
#define DS3232_PTR_MINUTES			0x01
	#define DS3232_BV_MINUTES_10		4
	
#define DS3232_PTR_HOURS			0x02
	#define DS3232_BV_HOURS_10			4
	#define DS3232_BV_HOURS_AM_PM_20	5
	#define DS3232_BV_HOURS_12_24		6
	
#define DS3232_PTR_WEEKDAY			0x03
	
#define DS3232_PTR_DATE				0x04
	#define DS3232_BV_DATE_10			4
	
#define DS3232_PTR_MONTH			0x05
	#define DS3232_BV_MONTH_10			4
	#define DS3232_BV_CENTURY			7
	
#define DS3232_PTR_YEAR				0x06
	#define DS3232_BV_YEAR_10			4
	
#define DS3232_PTR_ALARM1_SEC		0x07
	#define DS3232_BV_AM1M1				7
	
#define DS3232_PTR_ALARM1_MIN		0x08
	#define DS3232_BV_AM1M2				7
	
#define DS3232_PTR_ALARM1_HOUR		0x09
	#define DS3232_BV_AM1M3				7
	
#define DS3232_PTR_ALARM1_DAY		0x0A
	#define DS3232_BV_AM1DYDT			6
	#define DS3232_BV_AM1M4				7	
	
#define DS3232_PTR_ALARM2_MIN		0x0B
	#define DS3232_BV_AM2M2				7
	
#define DS3232_PTR_ALARM2_HOUR		0x0C
	#define DS3232_BV_AM2DYDT			6
	#define DS3232_BV_AM2M3				7
	
#define DS3232_PTR_ALARM2_DAY		0x0D
	#define DS3232_BV_AM2M4				7
	
#define DS3232_PTR_CONTROL			0x0E
	#define DS3232_BV_A1IE				0
	#define DS3232_BV_A2IE				1
	#define DS3232_BV_INTCN				2
	#define DS3232_BV_RS1				3
	#define DS3232_BV_RS2				4
	#define DS3232_BV_CONV				5
	#define DS3232_BV_BBSQW				6
	#define DS3232_BV_EOSC				7
	
#define DS3232_PTR_STATUS			0x0F
	#define DS3232_BV_A1F				0
	#define DS3232_BV_A2F				1
	#define DS3232_BV_BSY				2
	#define DS3232_BV_EN32KHZ			3
	#define DS3232_BV_CRATE0			4
	#define DS3232_BV_CRATE1			5
	#define DS3232_BV_BB32KHZ			6
	#define DS3232_BV_OSF				7
	
#define DS3232_PTR_AGING_OFFSET		0x10
	#define DS3232_BV_AGING_OFFSET_SIGN	7
	
#define DS3232_PTR_TEMP_MSB			0x11

#define DS3232_PTR_TEMP_LSB			0x12

#define DS3232_PTR_SRAM_START		0x14
	

//DS3232 functions

void get_time_bcd(uint8_t time_bcd[], uint8_t* ampm, uint8_t* mode_12_24);

void get_date_bcd(uint8_t date_bcd[]);

void set_hours(uint8_t hours);
void set_minutes(uint8_t minutes);
void zero_seconds(void);

void set_month(uint8_t month);
void set_day(uint8_t day);
void set_year(uint8_t year);

//MAKE SURE THIS SETS FOR ALARMS AS WELL
void set_all_mode_12_24(uint8_t mode);

uint8_t get_mode_12_24(void);

void get_alarm1_time(uint8_t alarm1_bcd[], uint8_t* ampm, uint8_t* mode_12_24);
void get_alarm2_time(uint8_t alarm2_bcd[], uint8_t* ampm, uint8_t* mode_12_24);

void set_alarm1_time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t ampm);
void set_alarm2_time(uint8_t hours, uint8_t minutes, uint8_t ampm);

void set_offset_adjust(uint8_t offset);
void get_offset_adjust(uint8_t* aging_offset);
#endif

















