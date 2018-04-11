#include <avr/io.h>
#include <util/delay.h>
#include"LCD.h"

void pwm()
{
TCCR1A=0xA0;
TCCR1B=0x11;
ICR1=500;
OCR1A=250;
OCR1B=250;
}

void adc()
{
ADCSRA=0x8F;
ADMUX=0x60;
}

void main()
{
int temp;
lcd_init();
DDRD=0xFF;
DDRA=0x00;
pwm();
adc();
while(1)
{
ADCSRA |= (1<<ADSC);
while(!(ADCSRA & (1<<ADIF)))
{
;
}
temp=ADCL;
_delay_ms(1000);

_delay_ms(2);
lcd_gotoxy1(0);
lcd_string(temp);
}
while(1)
{

//lcd_char('B');
}
}