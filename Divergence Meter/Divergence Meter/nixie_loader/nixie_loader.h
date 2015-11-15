/* This file contains necessary definitions for the nixie-tube loader
functions and controls
Date: 8-20-15
Author: Jim Herner
*/

#ifndef _NIXIE_LOADER_H_
#define _NIXIE_LOADER_H_ 1

#define REFRESH_RATE 30

#define BRIGHTNESS_MAX 7

#define REFRESH_TIMER_THRESHOLD (F_CPU/REFRESH_RATE)


volatile char global_bitstream[12];
volatile int global_mapped_digits[8];
volatile int global_digits[8];

volatile uint8_t global_nixie_brightness_counter;
volatile uint8_t global_nixie_brightness;

volatile uint8_t global_excercise_flag;
volatile uint8_t global_excercise_counter;

void init_display_vars(void);

void display_refresh_timer_init(void);

void latchHold(void);
void latchDump(void);

void cycleClock(void);

void generate_digits_time(uint8_t time[], int* digits);
void generate_digits_divergence(uint32_t divergence, int* digits);
void generate_digits_settings(uint8_t setting, uint8_t ampm, uint8_t value, int* digits);
void generate_digits_date(uint8_t date[], int* digits);

void generate_bitstream(uint8_t* bitstream, int digits[8]);

void map_digits(int digits[], int* mapped_digits);

void load_latches(char bitstream[12]);

void blankDisplay(void);
void unblankDisplay(void);

void display_excercise(int* digits);


#endif