#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
//#include <avr/stdint.h>
#include <stdio.h>
#include "Joystick.h"


void calibrate(){
	int x_akse=ADC_read(0);
	int y_akse=ADC_read(1);

	int x_akse_min=-100;
	int x_akse_max=100;
	int y_akse_min=-100;
	int y_akse_max=100;
	int x_center = ADC_read(0);
	int y_center = ADC_read(1);
	printf("Rotate the joystick ");
	
	// Wating for user and printing none calibrated values of the joystick
	while((-10 > x_akse> +10) & (-10 > y_akse> +10)){
		_delay_ms(1000);
		printf("Rotate joystick and press butten when done\n");
		printf("Un.cal. values: x=%d y=%d \n",x_akse,y_akse);
	}
	while((PINB & (1<<PINB2))){
		x_akse=ADC_read(0);
		y_akse=ADC_read(1);
		if(x_akse_min>x_akse){x_akse_min=x_akse;}
		if(x_akse_max<x_akse){x_akse_max=x_akse;}
		if(y_akse_min>y_akse){y_akse_min=y_akse;}
		if(y_akse_max<y_akse){y_akse_max=y_akse;}
		printf ("xMin: %i xMaks: %i yMin: %i yMaks: %i \n", x_akse_min,x_akse_max,y_akse_min,y_akse_max);
	}
	int calibrated [] = {x_akse_min,x_akse_max,y_akse_min,y_akse_max};
	printf ("Kalibrerte verdier: \n xMin: %i xMaks: %i yMin: %i yMaks: %i \n xAVG: %i yAVG: %i \n", calibrated[0],calibrated[1],calibrated[2],calibrated[3], x_center, y_center);
}


char read_y()
{
return ADC_read(0);
}

char read_x()
{
return ADC_read(1);
}

char read_knappJoy()
{
	if(1>(PINB & (1<<PINB2))){
		return 1;
	}
	return 0;
}

// Omgjør verdiene til bool 
char read_knappLeft()
{
	if(1<PINB & (1<<PINB1)){
		return 1;
	}
	return 0;
}


char read_knappRight()
{
	if(PINB & (1<<PINB0)){
		return 1;
	}
	return 0;		
}