#include "setup.h"
#include "control_driver.h"
#include "DAC.h"


#define vel_max 100
#define vel_min -100
#define dt 0.02

unsigned int encoder_value;
unsigned char MSB;
unsigned char LSB;
static int timer_flag;
static int playing_flag;
unsigned int encoder_max;
int clock_seconds;
uint8_t counter;
uint8_t pid_flag;
int16_t reference_value;
float error_sum;
float kp;
float kd;
float ki;
int integrator_max = 300;


void CD_init()
{
		set_bit(DDRH, RST);
		set_bit(DDRH, OE);
		set_bit(DDRH, EN);
		set_bit(DDRH, DIR);
		set_bit(DDRH, SEL);
		
		// Port K Digital input
		DDRK = 0x00;
		clear_bit(MJ1,OE);								// enable encoder						
		CD_encoder_reset();								// reset encoder
		CD_speed(0);									// sett star speed 
		set_bit(MJ1,EN);								// enable motor
		encoder_max = CD_encoder_max();					// Find encoder max
		clock_seconds = 0;								
		counter = 0;
		CD_set_ref_pos(125);							// sett startpos to middle
		error_sum = 0;
}

void CD_set_ref_pos(unsigned char pos)
{
	reference_value = -pos + 255;
}

void CD_init_clk()
{
		OCR1A = 5000;

		//Enable CTC mode
		TCCR1A |= (1 << COM1A0);

		//Prescale 64
		TCCR1B |= (1 << CS11) | (1 << CS10) | (1 << WGM12);
		
		//Enable compare match A interrupt
		TIMSK1 |= (1 << OCIE1A);
}

unsigned char CD_read_encoder()
{
	clear_bit(MJ1, OE);
	clear_bit(MJ1, SEL);
	_delay_us(20);
	
	MSB = MJ2;
	
	set_bit(MJ1, SEL);
	_delay_us(20);
	
	LSB = MJ2
	
	CD_encoder_reset();
	set_bit(MJ1, OE);
	
	int data = ((MSB<<8) | LSB);
	return data;
}

void CD_direc(unsigned char direc)
{
	if (direc == 1)
	{
		set_bit(MJ1, DIR);
	}
	If (direc == 0)
	{
		clear_bit(MJ1, DIR);
	}

}


void CD_speed(unsigned char value)
{
	DAC_send(value);
}

void CD_velocity(int vel)
{
		encoder_value = CD_read_encoder();
		
		CD_direc((uint8_t) abs(vel));
		if (vel > 0)
		{
			CD_direc(1);
		}
		else
		{
			CD_direc(0);
		}
}

unsigned char CD_pid_gain(float p,float i,float d)
{
	kp = p;
	ki = i;
	kd = d;
}

void CD_PID()
 {
	 	// Use PID when playing

		 	long encoder_value = CD_read_encoder();
		 	
		 	if((int)encoder_value < 0){
			 	encoder_value = 0L;
		 	}
		 	
		 	float error;
		 	int16_t output;
		 	error = reference_value - ((encoder_value*255)/encoder_max);
		 	if (abs(error) < 3)
		 	{
			 	CD_speed(0);
			 	return;
		 	}
		 	if ((int) error_sum < integrator_max)
		 	{
			 	error_sum += dt*error;
		 	}
		 	int prop = -kp*error;
		 	int integral = - ki*error_sum;
		 	
		 	output = prop + integral;
		 	
		 	CD_velocity(output);
 }
 
 /*ISR(TIMER1_COMPA_vect)
 {
	 // If the timer is on, count the score
	 if (timer_flag)
	 {
		 counter++;
		 // Running on 50Hz
		 if (counter >= 50)
		 {
			 counter = 0;
			 clock_seconds ++;
		 }
	 }
	 pid_flag = TRUE;
 }*/
 
 void CD_encoder_reset()
 {
	 clear_bit(MJ1, RST);
	 _delay_us(20);
	 set_bit(MJ1, RST);
 }
 
 unsigned int CD_encoder_max()
 {
	 printf("In find encoder max");
	 
	 CD_velocity(80);
	 _delay_ms(1000);
	 CD_velocity(0);
	 _delay_ms(200);
	 printf("Encoder value: %d\n",CD_read_encoder());
	 encoder_reset();
	 
	 CD_velocity(-80);
	 _delay_ms(1000);
	 CD__velocity(0);
	 _delay_ms(100);
	 printf("\tEncoder value2: %d\n",CD_read_encoder());
	 
	 uint16_t encoder = CD_read_encoder();
	 CD_velocity(80);
	 _delay_ms(200);
	 CD_velocity(0);
	 
	 return encoder;
 }
 

 
 