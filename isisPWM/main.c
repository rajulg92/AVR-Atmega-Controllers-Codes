#include <avr/io.h>

void pwm_init()
{
TCCR1A=0xA0;
TCCR1B=0x11;
ICR1=5000; //10000-5000 -50Hz
OCR1A=2500;
//OCR1B=;
}


void main()
{
DDRB=0xFF;
pwm_init();
while(1);
}