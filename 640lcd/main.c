#include<avr/io.h>
#include<util/delay.h>
#include"4bitLCD.h"

int main()
{
lcd_init();
lcd_string("Team Robocon");
lcd_cmd(LINE2);
lcd_string("2012");
while(1)
{
;
}
return 0;
}