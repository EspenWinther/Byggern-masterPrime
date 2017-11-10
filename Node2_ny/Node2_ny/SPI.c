#include <avr/io.h>
#include "setup.h"
//#define F_CPU FOSC


void SPI_MasterInit(void)
{

	DDRB    |=  (1<<DDB1)   // Slave Clock Input as output
			|   (1<<DDB2)   // Master Output/Slave Input as output
			|   (1<<DDB0)  // Slave Select as output
			|   (1<<DDB7);  // Chip Select as output
	DDRB    &= ~(1<<DDB3);  // Master Input/Slave Output as input

	//SPI Control Register
	SPCR    |=  (1<<SPE)    // SPI Enable
			|   (1<<SPR0)   // SCK frequency to F_OSC/16
			|   (1<<MSTR);  // Set SPI to master mode
}


unsigned char SPI_tranciever(unsigned char data)
{

	/* Start transmission */

	SPDR = data;
	/* Wait for transmission complete */
	while (!(SPSR & (1<<SPIF)));
	//Return received data
	return SPDR;
}

//void testSPI(){
		//uint8_t dummy=	0xFF;//MCP_read(0x0E);
		//printf("After Reset Status is: %2x\n",dummy);
		//MCP_write(0x0F,0x40);
		//dummy=	MCP_read(0x0F);
		//printf("Updated Status is: %2x\n",dummy);
		//_delay_ms(1000);
//}

void spi_chipselect(unsigned char enable){
	enable
	? (PORTB &= ~(1<<PB7))
	: (PORTB |= (1<<PB7));
} 