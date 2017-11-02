/*
 * DAC.c
 *
 * Created: 02.11.2017 15:32:02
 *  Author: espeneil
 */ 

#include <avr/io.h>
#include "setup.h"
#include "TWI_Master.h"

void DAC_init()
{
	TWI_Master_Initialise();
}

void DAC_send(unsigned char adr, unsigned char nr, unsigned char val)
{
	unsigned int *msg;
	msg[0] = (adr << 1) & 0;			// 7 MSB bit address and 1 LSB read (0) byte 
	msg[1] = nr;						// Adress for DAC 0-3, DAC0 = 0
	msg[2] = val;						// Value to put on the chosen DAC
	TWI_Start_Transceiver_With_Data(&msg, 3);
}

void DAC_reset(unsigned char adr)
{
	unsigned int *msg;
	msg[0] = (adr << 1) & 0;			// 7 MSB bit address and 1 LSB read (0) byte
	msg[1] = 0b00010000;				// Reset command
	TWI_Start_Transceiver_With_Data(&msg, 2);
}

