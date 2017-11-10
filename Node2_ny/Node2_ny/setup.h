#ifndef _SETUP_H_
#define _SETUP_H_

//Setup for the OSC fq which is then set to be the CPU fq
#define FOSC 16000000UL
//#define F_CPU FOSC
//#define BAUD 9600
//#define MYUBRR FOSC/16/BAUD-1

#define set_bit(reg, bit) (reg |= (1 << bit))
#define clear_bit(reg, bit) (reg &= ~(1 << bit))
#define test_bit(reg, bit) (reg & (1 << bit))

#endif