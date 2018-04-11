#include <avr/io.h>
#include <util/delay.h>

void port_init()
{
	DDRA = 0x00;
	DDRC  = 0xFF;
	PORTA = 0xFF;
}

int main()
{
	uint8_t data = 0; 
	port_init();
	while(1)
	{
		data = PINA;
		PORTC = data;
		/*else
			PORTC &= ~(1 << P2C0);
		
		if(data & 0b00000100)
			PORTC |= (1 << PC0) | (1 << PC1);
			
		else {
			PORTC &= ~(1 << PC0);
			PORTC &= ~(1 << PC1);
		}
		
		if(data & 0b00000010)
			PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2);
		else
			PORTC = 0x00;
		
		if(data & 0b00000001)
			PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 <<PC3);
		else
			PORTC = 0x00;
		
		if(data & 0b00000000)
			PORTC = 0x00;
		
		if(data & 0b00001111)
			PORTC = 0x00;*/
	}
}