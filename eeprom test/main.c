#include<avr/io.h>
#include<avr/eeprom.h>
#include"LCD.h"
#define	yateen 0x3B800
//#define data 11
int main()
{
	lcd_init();

	long int i = 0, j = 0, k = 0;
	
	for(i = 0x00, j = 0; i <= 1023 ; i++, j++)	
	{	
		eeprom_write_byte(i, j);
		if(j == 0xFF)
		{
			j == 0x00;
		}
	}

	
	//eeprom_write_byte(1022, 0xCC);
	
	/*for(i = 0x00; i <= 0x3F807 ; i++)
	{
		j = eeprom_read_byte(i);
		if(j == 0xFF)
			k++;
		
		if(1024 / i== 0)
			break;
	}
	
	lcd_showvalue(k);
	lcd_string("done");
	*/while(1)
	{
		
	}
	return 0;	
}