#ifndef DAC_H_
#define DAC_H_

void DAC_init();
void DAC_send(unsigned char adr, unsigned char nr, unsigned char val);
void DAC_reset(unsigned char adr);

#endif