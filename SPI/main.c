#include<avr/io.h>
#include<util/delay.h>
void spi_init();
uint8_t spi_txrx(uint8_t dataout);
void ps2_init();
void ps2_poll();

int rx_buffer[5];
int ps2rx=0,ps2ry=0,ps2lx=0,ps2ly=0;
int pressure[12];

void main()
{
DDRC=0x01;
int i;
spi_init();
while(1)
{
ps2_init();
if(rx_buffer[2]==0x5a)
{
PORTC |= (1<<PC0);
ps2_poll();
}
/*for(i=0;i<12;i++)
{
PORTD=pressure[i];
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
}*/
/*PORTD=ps2rx;
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
PORTD=ps2ry;
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
PORTD=ps2lx;
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
PORTD=ps2ly;
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
*/
}
}

void spi_init()
{
DDRB=0b00101100;
DDRD=0b11111111;
SPCR=(1<<MSTR) |(1<<CPOL)|(1<<DORD)|(1<<CPHA);
}

uint8_t spi_txrx(uint8_t dataout)
{
	SPCR |= (1<<SPE);
    uint8_t datain;

    SPDR = dataout;

    while(!(SPSR & (1<<SPIF)));

    _delay_us(30);  
    datain = SPDR;
	//PORTD = datain;
	_delay_ms(1000);
    return datain;
	SPCR &= ~(1<<SPE);
	PORTB |= (1<<PB5);

}


void ps2_init()
{

	PORTB &= ~(1<<PB2);
	rx_buffer[0] = spi_txrx(0x01);
    rx_buffer[1] = spi_txrx(0x42);
    rx_buffer[2] = spi_txrx(0x00);
	rx_buffer[3] = spi_txrx(0x00);
	rx_buffer[4] = spi_txrx(0x00);
	
   // PORTB |= (1<<PB2);
	
	_delay_ms(250);

}

void ps2_poll()
{
int i;
	//PORTB &= ~(1<<PB2);
	ps2rx=spi_txrx(0x00);
	ps2ry=spi_txrx(0x00);
	ps2lx=spi_txrx(0x00);
	ps2ly=spi_txrx(0x00);
	
	for(i=0;i<12;i++)
	{
		pressure[i]=spi_txrx(0x00);
	}
	PORTB |= (1<<PB2);

}
