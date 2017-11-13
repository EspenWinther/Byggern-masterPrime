/*
 * GccApplication1.c
 *
 * Created: 23.10.2017 15:19:53
 *  Author: gautevn Boi
 */ 

#include "setup.h"
//#define F_CPU FOSC
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "control_driver.h"
#include "UART.h"
#include "SPI.h"
#include "MCP2515.h"
#include "CAN.h"
#include "DAC.h"
#include "pwm.h"
#include "Solenoid.h"
#include "IR_ball.h"


//CAN_message *h;								//Receiver generated message
CAN_message test;							//Receiver generated message

char DAC_address = 0b01010000;					// 7 bit address for DAC 

 int main(void)
{
	
	UartInit();								// Initialize the UART
	CAN_init();								// Initialize the CAN bus drivers
	CD_init();								// Initialize the Control Driver
	pwm_init();
	IR_init();
	sol_init();
	
	cli();
	sei();					// Enable Global Interrupts
	
	_delay_ms(1);
	
	
	
	volatile int shotcounter = 0;
	
	
	printf("Start på program\n");
	
    while(1)
    {
		
		//_delay_ms(50);
		if (CANcounter > 0){
			CAN_read2(&test);
			if (test.id == 3) // 
			{
				//sol_shot();
				//printf("Node 2 = ID: %i L: %i D: %i :%i :%i :%i :%i :%i :%i \n",test.id,test.length,test.data[0],test.data[1],test.data[2],test.data[3],test.data[4],test.data[5],test.data[6]);
				uint16_t joystick_a = -test.data[1]+200; // 0-200 -> 200-0 forandret til uint8_t som test -Espen
				uint8_t joybtn = test.data[4];			// forandret til uint8_t som test -Espen
				uint8_t slider_a = -test.data[2]+200; // 0-200 -> 200-0
				//printf("Slider: %i\n", slider_a);
				CD_pid_gain(1600,5000,0);
				pwm_set_angle(joystick_a, 1);
				//printf("Joy: %i \n",joystick_a );
				CD_PID(slider_a);
				sol_shot(joybtn);
			
				if (joybtn){
					shotcounter++;
					printf("SOL_SOL\n");
				}
			
				//printf("%i ejg er ute i if\n",test_bit(PINL, PL2));
				if (IR_read()){
					printf(("%i ejg er inne i if",test_bit(PINL, PL2)));
					//printf("IR_IR_IR_IR\n");
					_delay_ms(100);
				}
				//printf("encoder read: %i",CD_read_encoder());
			
				//int a =CD_read_encoder();
				//printf("EC: %i \n",a );
				//CD_velocity(slider_a);
				//DAC_send(DAC_address,0,slider_a);
				//printf("DAC status: %x\n",TWI_Get_State_Info());
			}	
			CANcounter = 0;	
		}
	}
	
	
}

ISR(__vector_default){}