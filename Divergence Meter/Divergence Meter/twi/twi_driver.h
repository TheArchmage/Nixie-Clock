/* This file contains function definitions and other
preprocessor definitions necessary for the use of the
twi functionality of the ATmega328p

Date: 8-21-15
Author: Jim Herner
*/

#ifndef _TWI_DRIVER_H_
#define _TWI_DRIVER_H_ 1

#include <stdint.h>


void init_twi(void);
void twi_start(void);
void twi_send_byte(uint8_t byte);
uint8_t twi_recieve_byte_ack(void);
uint8_t twi_recieve_byte_noack(void);
void twi_stop(void);
void twi_wait_for_complete(void);





#endif