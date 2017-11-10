#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "OLED.h"
#include "fonts.h"
#include "Game_play.h"
#include "setup.h"
#include "ADC_test.h"
#include "Joystick.h"
#include <stdio.h>

volatile char *write_c = (char *) 0x1000;
volatile char *write_d = (char *) 0x1200; 
char Name[3];
char can_flag;

//sei();

void init_OLED()
{
	MCUCR |= (1<<SRE);
	SFIOR |= (1<<XMM2);
	*write_c = (0xae);        //  display  off
	*write_c = (0xa1);        //segment  remap
	*write_c = (0xda);        //common  pads  hardware:  alternative
	*write_c = (0x12);
	*write_c = (0xc8);        //common output scan direction:com63~com0
	*write_c = (0xa8);        //multiplex  ration  mode:63
	*write_c = (0x3f);
	*write_c = (0xd5);        //display divide ratio/osc. freq. mode
	*write_c = (0x80);
	*write_c = (0x81);        //contrast  control
	*write_c = (0x50);
	*write_c = (0xd9);        //set  pre-charge  period
	*write_c = (0x21);
	*write_c = (0x20);        //Set  Memory  Addressing  Mode
	*write_c = (0x02);
	*write_c = (0xdb);        //VCOM  deselect  level  mode
	*write_c = (0x30);
	*write_c = (0xad);        //master  configuration
	*write_c = (0x00);
	*write_c = (0xa4);        //out  follows  RAM  content
	*write_c = (0xa6);        //set  normal  display
	*write_c = (0xaf);        //  display  on
}

void OLED_Reset(void)
{
	for (int i = 0; i<8; i++){
		*write_c = (0xB0+i);
		for (int j = 0; j<128; j++){
			*write_d = (0x00);
		}
	}
	OLED_Home();
}

void OLED_Home(void)
{
	*write_c = (0xB0);
	*write_c = (0x00);
	*write_c = (0x10);
}


// denne tar en char array 
int OLED_print(char *d, char font)
{
	int i = 0;
	// looper over den lenkede listen som er stringen
	while (d[i] != '\0')
	{
		OLED_print_char(d[i], font); 
		i++;
	}
	return 0;
}


// denne tar et aski tall, printer over rows
int OLED_print_char(char c, char font)
{
		for (int i = 0 ; i<font ; i++)
		{
			//if (font == 5)
			//{
				//*write_d = pgm_read_byte(&font5[c-' '][i]); // better than write_d(font8[c-' '][i]);
			//}
			//else
			//{
				*write_d = pgm_read_byte(&font8[c-' '][i]); // better than write_d(font8[c-' '][i]);
			//}
		}
	return 0;
}


void OLED_gotoline(char line)		// Input in page addressing mode, line 0-7
{
	if ( line < 8 )
	{
		*write_c = (0xB0 + line);	// Page start address number (Line number)
		*write_c = (0x00);			// Lower Column start address
		*write_c = (0x10);			// Higher Column start address
	}
	
}

void OLED_goto(char line, char col)		// Input in page addressing mode, line 0-7 
{
	if ( line < 8 )
	{
		char lsb = col & 0x0F;
		char msb = col & 0xF0;
		msb = (msb>>4);	
						// Bitwise operator to get the 4 MSB
		*write_c = (0xB0 + line);			// Page start address number (Line number)
		*write_c = (0x00 | lsb);			// Lower Column start address
		*write_c = (0x10 | msb);			// Higher Column start address
	}

}




void OLED_menu()
{
	OLED_Reset();
	OLED_Home();
	OLED_goto(0, 105);
	OLED_print(Name, 8);
	OLED_goto(0,0);
	OLED_print("PokePong!", 8);
	OLED_goto(2, 10);
	OLED_print("Ping-Pong:", 5);
	OLED_goto(4, 10);
	OLED_print("Highscore:", 5);
	OLED_goto(6, 10);
	OLED_print("Random knapp", 5);
	OLED_goto(2, 0);
	OLED_print_char('~'+1, 5);
	
	
	int p = 2;
	//while (read_knappLeft() != 1)
	//{
		
		if (read_y() > 50)
		{
			// Blanker forrige
			OLED_goto(p, 0);
			OLED_print_char(' ', 5);
			p=p-2;
			if(p<2)
			{
				p=6;
			}
			// skriver pil
			OLED_goto(p, 0);
			OLED_print_char('~'+1, 5);
			_delay_ms(300); // delay etter vi har trykket ikke når vi skal trykke
			
		}
		else if (read_y() < -50)
		{
			OLED_goto(p, 0);
			OLED_print_char(' ',5); //blank
			p=p+2;
			if(p>6)
			{
				p=2;
			}
			OLED_goto(p, 0);
			OLED_print_char('~'+1, 5);
			_delay_ms(300);// delay etter vi har trykket ikke når vi skal trykke
		
		
		}
		
		//if (p==6 & (read_knappJoy() == 1))
		//{
			//OLED_animation();
			//OLED_menu();
		//}
		
		else if(read_knappJoy() == 1)
		{
			switch(p) 
			{

				case 2:
				{
				printf("GAME!!!!");
				OLED_Reset();
				OLED_Home();
				OLED_print("PING-PONG", 8);

				}
				break;
				
				case 4  :
				{
				printf("caseting");
				}
				break;
				
				case 6:
				{
					printf("Whoop-de-fucking-dooo!");
					OLED_animation();
					OLED_menu();
				}
				break;
				
				default :
				break;
			}
		}

	//}
}

void OLED_highscore()
{

	
}

char * OLED_NameScreen()
{
		OLED_Reset();
		char bokstav = '@';
		OLED_Home();
		OLED_print("CHOOSE USERNAME", 8);
		OLED_goto(4, 0);
		OLED_print("Name:", 8);
		int minne = 0;
		char * Name;
		OLED_goto(4, 40);
		int p = 40;
		while (minne <=2)
		{
			
		if (read_y() > 50)
		{
				OLED_goto(4, p);
				bokstav++;
				OLED_print_char(bokstav, 8);
				_delay_ms(250);
		}
		if (read_y() < -50)
		{
			OLED_goto(4, p);
			bokstav--;
			OLED_print_char(bokstav, 8);
			_delay_ms(250);
		}
		if (read_knappRight() == 1)
		{
			Name[minne] = bokstav;
			minne++;
			p = p+8;
			_delay_ms(500);
		}
		if ((read_knappRight() == 1) & (minne == 1))
		{
			OLED_goto(4, p);
		}
		if ((read_knappRight() == 1) & ( minne == 2))
		{
			OLED_goto(4, p);

		}
	}
return &Name;
}

void OLED_animation(){
	OLED_Home();
	*write_c = 0x20;
	*write_c = 0b0001;
	for(int k=0;k<10;k++){
		for(int j=0; j<7; j++){
			_delay_ms(100);
			for (int i = 0 ; i<8*128 ; i++)
			{
				char a = pgm_read_byte(&Hallo[j][i]); // better than write_d(font8[c-' '][i]);
				
				// Inverting the bytes
				a = ((a>>1) & 0x55) | ((a<<1) & 0xaa);
				a = ((a>>2) & 0x33) | ((a<<2) & 0xcc);
				a = ((a>>4) & 0x0f) | ((a<<4) & 0xf0);
				// Inverting the bytes
				*write_d = a;
			}
		}
	}
	*write_c = 0x20;
	*write_c = 0b0010;
}

void OLED_picture()
{
	OLED_Home();
	OLED_goto(ADC_read(0)/12,ADC_read(1)/2);
	*write_c = 0x20;
	*write_c = 0b0001;
	for (int i = 0 ; i<8*128 ; i++)
	{
		char a = pgm_read_byte(&Pokeball[i]); // better than write_d(font8[c-' '][i]);
		
		// Inverting the bytes
		a = ((a>>1) & 0x55) | ((a<<1) & 0xaa);
		a = ((a>>2) & 0x33) | ((a<<2) & 0xcc);
		a = ((a>>4) & 0x0f) | ((a<<4) & 0xf0);
		// Inverting the bytes
		*write_d = a;
	}
	*write_c = 0x20;
	*write_c = 0b0010;
}

