/*
 * GccApplication1.c
 *
 * Created: 23.10.2017 15:19:53
 *  Author: gautevn Boi
 */ 

#include "setup.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "UART.h"
#include "SPI.h"
#include "MCP2515.h"
#include "CAN.h"

CAN_message *h;								//Receiver generated message
CAN_message test;

 main(void)
{
	UartInit();
	CAN_init();

	sei();
	
	
	printf("Start p� program\n");
	
    while(1)
    {
		_delay_ms(10);
		CAN_read2(&test);
		if (1) // test.id < 10
		{
			printf("knalla 1 = ID %i ID %i %i %i %i \n ",test.id,test.data[0],test.data[1],test.data[2],test.data[3]);
		}
    }
}