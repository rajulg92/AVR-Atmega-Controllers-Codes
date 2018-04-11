#include<avr/io.h>
#include<avr/interrupt.h>
#include"PORT.h"
#include"LCD.h"
int i=120;



int main(void)
{
  DDRC=0XFF;
  DDRG=0XFF;
// DDRE=0XFF;
  DDRA=0XFF;
 // DDRF=0XFF;
  //DDRG=0XFF;
  lcd_init();
  lcd_string("TEAM ROBOCON");
  lcd_cmd(LINE2);
 // lcd_showvalue(temp);
	/*while(1){
  //SETBIT(PORTA,BIT(4));
  //SETBIT(PORTA,BIT(6));
  //CLEARBIT(PORTD,BIT(4));
  lcd_char('A');
  }*/
    // CLEARBIT(PORTA,BIT(0));
while(1)
{
}

  return 0;

}
