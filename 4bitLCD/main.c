#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include"PORT.h"
#include"LCD.h"
int i=120;



int main(void)
{
int temp1=0,temp2=0;
	DDRB=0x00;
	DDRC=0xFF;

  lcd_init();
  lcd_string("COUNT = ");
  
while(1)
{
}
  return 0;

}
