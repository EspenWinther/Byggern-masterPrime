/*
 * Byggern.c
 *
 * Created: 04.09.2017 10:24:49
 *  Author: gautevn
 */ 
#include <avr/io.h>
#include "setup.h"
//#define F_CPU FOSC
#include <avr/interrupt.h>
#include <avr/sleep.h>
//#include <avr/stdint.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "UART.h"
//#include "SRAM_test.h"
#include "ADC_test.h"
#include "OLED.h"
#include "Joystick.h"
//#include "fonts.h"
//#include "Buzz.h"
#include "SPI.h"
#include "MCP2515.h"
#include "CAN.h"
#include "Game_play.h"

volatile char can_flag = 0;

int main(void)
{
	DDRD  = 0xFF;
	DDRB = 0b00011011;				//Output, input på 0-2
	PORTB = 0xFF;					//pull-up og høy ut
	
	UartInit();
	//SRAM_init();
	ADC_init();
	init_OLED();
	CAN_init();
	
	//Trigger interrupt with interval of 50hz
	OCR1A = 1536;

	//Enable CTC mode
	set_bit(TCCR1A,COM1A0);

	//Prescale 64
	set_bit(TCCR1B, CS11);
	set_bit(TCCR1B, CS10);
	set_bit(TCCR1B, WGM12);
	
	// Enable interrupt on output compare match timer 1
	set_bit(TIMSK, OCIE1A);
	
	// Enable Global Interrupts
	sei();


	OLED_picture();
	_delay_ms(1000);
	calibrate();
	OLED_Reset();
	OLED_NameScreen();
	OLED_menu();
			
	CAN_message myMessage;				//test message
	printf("Start på program\n");
	CAN_message h;						//Receiver generated message
    
	
	while(1)
    {
		
		if (can_flag)
		{
			Ping_Pong();
			_delay_ms(10);
			can_flag = 0;
		}

	



	}
}	

// ISR for CAN timer
ISR(TIMER1_COMPA_vect)
{	
	// Enable sending of joystick data
	can_flag = 1;
}
