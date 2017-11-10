#include <avr/io.h>
#include "setup.h"
#include <util/delay.h>




void buzzinit()
{
	//unsigned char duty_cyc_a,duty_cyc_b;
//
	//// Initial PORT Used
	//DDRD = 0b11111111;     // Set PORTD: Output
	//PORTD = 0x00;
//
	//// Initial TIMER0 Fast PWM
	//// Fast PWM Frequency = fclk / (N * 256), Where N is the Prescaler
	//// f_PWM = 11059200 / (64 * 256) = 675 Hz
	//TCCR0A = 0b10100011; // Fast PWM 8 Bit, Clear OCA0/OCB0 on Compare Match, Set on TOP
	//TCCR0B = 0b00000011; // Used 64 Prescaler
	//TCNT0 = 0;           // Reset TCNT0
	//OCR0A = 0;           // Initial the Output Compare register A & B
	//OCR0B = 0;
//
	//duty_cyc_a=0;	// Initial Duty Cycle for Channel A
	//duty_cyc_b=255;	// Initial Duty Cycle for Channel B
//
	//for(;;) {            // Loop Forever
		//while(duty_cyc_a < 255) {
			//OCR0A=duty_cyc_a++;
			//OCR0B=duty_cyc_b--;
			//_delay_ms(10);
		//}
//
		//while(duty_cyc_b < 255) {
			//OCR0A=duty_cyc_a--;
			//OCR0B=duty_cyc_b++;
			//_delay_ms(10);
		//}
}

#define LED_PORT    PORTD
#define LED_PIN     5
#define LED_off()  LED_PORT&=~_BV(LED_PIN)
#define LED_on() LED_PORT|=_BV(LED_PIN)
void buzz(unsigned int freq, int time)
{
	while (time>0)
	{
	LED_on();
	delayUS(freq);
	LED_off();
	delayUS(freq);
	time--;
	}
}


void delayUS(unsigned int time)
{
	while(time > 0)
	{
		_delay_us(1);
		time--;
	}
}

void tone(char Note, char duration)
{
	
}
