#include <avr/io.h>
#include "setup.h"
//#define F_CPU FOSC
#include <util/delay.h>
#include <avr/sleep.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "control_driver.h"
#include "DAC.h"

#define vel_max 100
#define vel_min -100
#define dt 0.02

#define DAC_address 0b01010000		// 7 bit address for the DAC, 3 LSB should be connected to GND on chip
#define DAC_number 0				// DAC number 0-3

int16_t encoder_value ;
//int MSB;
//int LSB;
//static int timer_flag;
//static int playing_flag;
int16_t encoder_max = 10000;
int reference_max = 200;
int clock_seconds;
uint8_t counter;
volatile uint8_t pid_flag;
//int16_t reference_value;
float error_sum = 0;
float last_error = 0;
float kp;
float kd;
float ki;
int integrator_max = 300;
volatile int CANcounter = 0;
float mean = 0; 


void CD_init()
{
	DAC_init();										// DAC init
	CD_clk_init();									// Init the Score clock
	
	// Port H pins as Outputs
	set_bit(DDRH, RST);
	set_bit(DDRH, OE);
	set_bit(DDRH, EN);
	set_bit(DDRH, DIR);
	set_bit(DDRH, SEL);
		
	// Port K Digital input Port A Output
	DDRK = 0x00;
	clear_bit(MJ1,OE);								// enable encoder						
	CD_encoder_reset();								// reset encoder
	CD_speed(0);									// sett start speed 
	set_bit(MJ1,EN);								// enable motor
	encoder_max = CD_encoder_max();					// Find encoder max
	
	clock_seconds = 0;								
	counter = 0;
	//CD_set_ref_pos(125);							// sett startpos to middle
	error_sum = 0;
	
	
}


//void CD_set_ref_pos(unsigned char pos)
//{
	//reference_value = -pos + 255;
//}

void CD_clk_init()
{
		OCR1A = 5000;

		//Enable CTC mode
		TCCR1A |= (1 << COM1A0);

		//Prescale 64
		TCCR1B |= (1 << CS11) | (1 << CS10) | (1 << WGM12);
		
		//Enable compare match A interrupt
		TIMSK1 |= (1 << OCIE1A);
		
}

int16_t CD_read_encoder()
{
	volatile int16_t data;
	clear_bit(MJ1, OE);
	clear_bit(MJ1, SEL);
	_delay_us(20);		
	
	data |= ((MJ2)<<8);
	
	set_bit(MJ1, SEL);
	_delay_us(20);		
	
	data |= ((MJ2)<<0);
	_delay_us(5);
	
	encoder_value += data;
	CD_encoder_reset();
	set_bit(MJ1, OE);
	return encoder_value;
}

void CD_direc(unsigned char direc)
{
	if (direc == 1)
	{
		set_bit(MJ1, DIR);
	}
	if (direc == 0)
	{
		clear_bit(MJ1, DIR);
	}
}


void CD_speed(int value)
{
	DAC_send(DAC_address, DAC_number, value);
}


void nyPID()
{
	
}



// FUNKER
void CD_velocity(int vel)
{
	if (vel > 1){
		CD_direc(1);
	}
	else{
		CD_direc(0);
	}
	CD_speed(abs(vel));
}

void CD_pid_gain(float p,float i,float d)
{
	kp = p/1000;
	ki = i/1000;
	kd = d/1000;
}


void CD_PID(int16_t reference_value)
 {
	 	// Use PID when playing
		 if (pid_flag){
			 
		 	int16_t encoder_value = CD_read_encoder();

		 	//printf("Encoder %i\n", encoder_value);
			//printf("EncMax: %i\n", encoder_max);
			//printf("Ref: %i\n", reference_value);
			float error;
			float dErr;
		 	float output; //  -100 - 100			0-100					80
		 	float a = ((float)encoder_value*reference_max)/encoder_max;
		 
		 	error =	(float)reference_value - a; 
			
		 	if (abs(error) < 3)
		 	{
			 	CD_speed(0);
			 	return;
		 	}
		 	//if ((int) error_sum < integrator_max)
		 	//{
			 	error_sum += (dt*counter)*error;
		 	//}
			dErr = (error - last_error) / (dt*counter);
			output = -(kp * error + ki * error_sum + kd * dErr);
			last_error = error;

			 if(output>vel_max){
				output= vel_max;
			 }
			 if(output<vel_min){
				 output= vel_min;
			 } 
		 	 CD_velocity((int) output);
			  //_delay_ms(15);
		}
		pid_flag = 0;
		counter = 0;
 }
 
 ISR(TIMER1_COMPA_vect)		
 {
	 //printf("PRINT");
	 //Running on 50Hz
	counter++;
	CANcounter++;
	pid_flag = 1;
 }
 
 
 
 void CD_encoder_reset()
 {
	 clear_bit(MJ1, RST);
	 _delay_us(200);
	 set_bit(MJ1, RST);
 }
 
int16_t CD_encoder_max()
 {
	 printf("In find encoder max");
	 
	 CD_velocity(80);
	 _delay_ms(3000);
	 CD_velocity(0);
	 _delay_ms(200);
	 //printf("Encoder value: %d\n",CD_read_encoder());
	 CD_encoder_reset();
	 _delay_ms(10);
	 CD_velocity(-80);
	 _delay_ms(4000);
	 CD_velocity(0);
	 _delay_ms(100);
	 //printf("\tEncoder value2: %d\n",CD_read_encoder());
	 _delay_ms(10);
	 int16_t encoder = CD_read_encoder();
	 _delay_ms(10);
	 //encoder = (double) (encoder + CD_read_encoder())/2;
	 //_delay_ms(10);
	 //encoder = (double) (encoder + CD_read_encoder())/2;
	 //_delay_ms(10);
	 //encoder = (double) (encoder + CD_read_encoder())/2;
	 printf("Enqaaa: %d\n", encoder);
	 CD_velocity(80);
	 _delay_ms(500);
	 CD_velocity(0);
	 return (encoder);
 }
 

 
 