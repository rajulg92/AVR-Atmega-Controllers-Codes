#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include"PORT.h"
#include"LCD.h"
int i=120,result;


ISR(ADC_vect)
{

//result=ADCH;
lcd_gotoxy2(0);
lcd_showvalue(result);

}

void main(void)
{
//int a=13000;
//float b=4.52,c;
  DDRC=0x03;
  DDRB=0XFF;
  DDRD=0xFF;
  /*TCCR1A=0xA0;
  TCCR1B=0x11;
  ICR1=500;
  OCR1A=250;
  OCR1B=250;*/
  SREG=0X80;
  ADCSRA=0b10001001;
	ADMUX=0b00100010;

  lcd_init();
  lcd_string("ADC");
  sei();
 // lcd_showvalue(a);
 //lcd_cmd(LINE2);
 // lcd_showvaluef(b);
	/*TCCR1B=0x06;
	TCNT1=0;
	while(TCNT1<=500)
	{
	lcd_gotoxy2(0);
	lcd_showvalue(TCNT1);
	}*/
	while(1)
	{
	/*ADCSRA|=(1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	lcd_gotoxy2(0);
	lcd_showvalue(ADCH);
	_delay_ms(100);
	*/
	}

}
