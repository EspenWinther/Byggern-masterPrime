#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

//#include <avr/io.h>

#define RST PH6
#define OE PH5
#define EN PH4
#define DIR PH1
#define SEL PH3
#define MJ2 PINK
#define MJ1 PORTH

void CD_init();
void CD_set_ref_pos(unsigned char pos);
void CD_clk_init();
unsigned char CD_read_encoder();
void CD_direc(unsigned char direc);
void CD_speed(unsigned char value);
void CD_velocity(int vel);
unsigned char CD_pid_gain(float p,float i,float d);
void CD_PID();
void CD_encoder_reset();
unsigned int CD_encoder_max();

#endif