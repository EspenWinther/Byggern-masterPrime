#include <avr/io.h>
#include <util/delay.h>
#include "setup.h"
#include "control_driver.h"
#include "DAC.h"
#include <avr/sleep.h>
#include <math.h>

#define vel_max 100
#define vel_min -100
#define dt 0.02

#define DAC_address 0b01010000		// 7 bit address for the DAC, 3 LSB should be connected to GND on chip
#define DAC_number 0				// DAC number 0-3

unsigned int encoder_value;
int MSB;
int LSB;
static int timer_flag;
static int playing_flag;
double encoder_max = 10000;
int reference_max = 200;
int clock_seconds;
uint8_t counter;
uint8_t pid_flag;
int16_t reference_value;
float error_sum = 0;
float last_error = 0;
float kp;
float kd;
float ki;
int integrator_max = 300;

float mean = 0; 


void CD_init()
{
	DAC_init();										// DAC init
	//CD_clk_init();									// Init the Score clock
	
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
	if (CD_encoder_max() > 7000)
	{
		encoder_max = CD_encoder_max();
	}
							// Find encoder max
	
	clock_seconds = 0;								
	counter = 0;
	CD_set_ref_pos(125);							// sett startpos to middle
	error_sum = 0;
		
	
		
}

void CD_set_ref_pos(unsigned char pos)
{
	reference_value = -pos + 255;
}

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

double CD_read_encoder()
{
	clear_bit(MJ1, OE);
	clear_bit(MJ1, SEL);
	_delay_ms(200);		// egentlig 20ms
	
	MSB = MJ2;
	
	set_bit(MJ1, SEL);
	_delay_ms(200);		// egentlig 20ms
	
	LSB = MJ2;
	
	set_bit(MJ1, OE);
	
	double data = ((MSB<<8) | LSB);
	return data;
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
	//printf("Speed: %i\n", value);
	DAC_send(DAC_address, DAC_number, value);
}


void nyPID(){
	
}



// FUNKER
void CD_velocity(int vel)
{
		//encoder_value = CD_read_encoder();
		//CD_direc((uint8_t) abs(vel));
		if (vel > 1)
		{
			CD_direc(1);
		}
		else
		{
			CD_direc(0);
		}
		
		CD_speed(abs(vel));
		//printf("Velocity: %i\n",abs(vel));
}

unsigned char CD_pid_gain(float p,float i,float d)
{
	kp = p/10000;
	ki = i/10000;
	kd = d/10000;
}

void CD_PID(reference_value)
 {
	 	// Use PID when playing
		 //printf("Slider PID: %i\n", reference_value);

		 	double encoder_value = CD_read_encoder();
		 	printf("Encoder %i\n", encoder_value);
		 	 
		 	float error;
			double dErr;
		 	float output; //  -100 - 100			0-100					80
		 	//printf("Ref: %i:\n", reference_value);
		 	//printf("Refmax: %i\n", reference_max);
		 	//printf("Encoder: %i\n",encoder_value);
		 	float a = ((double)encoder_value*reference_max)/encoder_max;
		 	printf("EncMax: %d\n", encoder_max);
			//long b = a / encoder_max;
			printf("a: %d\n", a);
			//			0-200				0-10 000		200			~10 000
		 	error =	(float)reference_value - a; // goood

			printf("Error: %d:\n", error);
			
		 	if (abs(error) < 3)
		 	{
			 	CD_speed(0);
			 	return;
		 	}
		 	//if ((int) error_sum < integrator_max)
		 	//{
			 	error_sum += dt*error;
				 printf("ErrorSum: %d\n",error_sum);
		 	//}
			dErr = (error - last_error) / dt;
			output = -(kp * error + ki * error_sum + kd * dErr);
			last_error = error;
		 	//float prop = -kp*error;
		 	//float integral = 0;//- ki*error_sum;
		 	
		 	//output = prop + integral;
		 	printf("Output PID: %i\n", output);
			 if(output>vel_max){
				output= vel_max;
			 }
			 if(output<vel_min){
				 output= vel_min;
			 } 
		 	 CD_velocity((int) output);
			  //_delay_ms(15);
			 
 }
 
 //ISR(TIMER1_COMPA_vect)
 //{
	 ////If the timer is on, count the score
	 //if (timer_flag)
	 //{
		 //counter++;
		 ////Running on 50Hz
		 //if (counter >= 50)
		 //{
			 //counter = 0;
			 //clock_seconds ++;
		 //}
	 //}
	 //pid_flag = 1;
 //}
 
 void CD_encoder_reset()
 {
	 clear_bit(MJ1, RST);
	 _delay_us(200);
	 set_bit(MJ1, RST);
 }
 
int CD_encoder_max()
 {
	 printf("In find encoder max");
	 
	 CD_velocity(80);
	 _delay_ms(40000);
	 CD_velocity(0);
	 _delay_ms(200);
	 //printf("Encoder value: %d\n",CD_read_encoder());
	 CD_encoder_reset();
	 _delay_ms(10);
	 CD_velocity(-80);
	 _delay_ms(60000);
	 CD_velocity(0);
	 _delay_ms(100);
	 //printf("\tEncoder value2: %d\n",CD_read_encoder());
	 _delay_ms(10);
	 int16_t encoder = CD_read_encoder();
	 _delay_ms(10);
	 encoder = (double) (encoder + CD_read_encoder())/2;
	 _delay_ms(10);
	 encoder = (double) (encoder + CD_read_encoder())/2;
	 _delay_ms(10);
	 encoder = (double) (encoder + CD_read_encoder())/2;
	 printf("Enqaaa: %d\n", encoder);
	 CD_velocity(80);
	 _delay_ms(1000);
	 CD_velocity(0);
	 return (encoder);
 }
 

 
 