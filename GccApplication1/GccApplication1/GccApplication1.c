/*
 * GccApplication1.c
 *
 * Created: 23.10.2017 15:19:53
 *  Author: gautevn
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "SPI.h"
#include "MCP2515.h"
#include "CAN.h"



int main(void)
{
	CAN_init();
	sei();
	CAN_message h;								//Receiver generated message

	int ident = 29;
	
    while(1)
    {
		CAN_read2(&h);
		//h.id=h.data[0];
		printf("h id = %d "  ,h.id  );
		printf("h length = %d "  , h.length  );
		printf("h= ");
		if (h.id == ident)
		{
			for (int i =0; i<8; i++)
			{
				printf("%d, ",h.data[i]);
			}
			printf("\n");
		}
		CAN_send(&h);
    }
}