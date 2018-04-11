uint8_t ReversePWMForPP = 40;
int MaxonSpeed = 320;
uint8_t lt_st = false, rt_st = false, discst = false, ppst = false, PidStarted = false, grip_flag = false;

//-----------------INITIALISATION-------------------
void pwm_init(void)
{
	TCCR4A = 0xA8;
	TCCR4B = 0x11;
	ICR4 = 737;
	TCCR5A = 0xA8;
	TCCR5B = 0x11;
	ICR5 = 737;
}

void port_init(void)
{
	DDRJ |= (1 << PJ0) | (1 << PJ2) | (1 << PJ1) | (1 << PJ3);
	SETBIT(DDRE,BIT(3));						// Suction Pump
	DDRL |= (1 << PL0) | (1 << PL3) |(1 << PL4)| (1 << PL1);
	DDRH |= (1 << PH4) | (1 << PH3) | (1 << PH5) | (1 << PH6);
	DDRG |= (1 << PG4);
	DDRD = 0xFF;
	DDRA = 0xFF;
	DDRC = 0x00;
	PORTC = 0xFF;
	
	CLEARBIT(DDRF,BIT(0));
	SETBIT(PORTF,BIT(0));
}
//-------------------------------------------------------------


//--------------------UART----------------------------
void UartInit(void)
{
	DDRH |= (1<<PH1);
	DDRH &= ~(1<<PH0);
	
	PORTH |= (1<<PH1);
	PORTH &= ~(1<<PH0);
	
	UCSR2B |= (1<<RXEN2) | (1<<TXEN2);
	UCSR2C |= (1<<UCSZ20) | (1<<UCSZ21);
	UCSR2A |= (1<<U2X2);
	
	UBRR2 = 191;
	// RX - PJ0
	// TX - PJ1
	
	// Using Channel 2 of 640 Present at PORTH
}

void UartTransmit(uint8_t data)
{
	while ( !( UCSR2A & (1<<UDRE2)) );
	UDR2 = data;
}
uint8_t UartReceive(void)
{
	while ( !( UCSR2A & (1<<RXC2)) );
	return UDR2;
}
//---------------------------------------------------


//------------------ADC-----------------------------
void ADCInit(void)
{
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADMUX |= (1<<ADLAR);
	ADCSRA |= (1<<ADSC);
}

int ADCConvert(void)
{
	ADCSRA |= (1<<ADSC);
	
	while( !(ADCSRA & (1<<ADIF)));
	return ADCH;
}
//--------------------------------------------------

//------------MAXON FUNCTIONS----------------------------------
void lft_fwd(void)
{
	SETBIT(PORTJ,BIT(0));
	CLEARBIT(PORTJ,BIT(1));
	OCR5A = 450;
}

void rgt_fwd(void)
{
	SETBIT(PORTJ,BIT(2));
	CLEARBIT(PORTJ,BIT(3));
	OCR5B = 455;
}

void rgt_rev(void)
{
	SETBIT(PORTJ,BIT(3));
	CLEARBIT(PORTJ,BIT(2));
	OCR5B = 450;
}

void lft_rev(void)
{
	SETBIT(PORTJ,BIT(1));
	CLEARBIT(PORTJ,BIT(0));
	OCR5A = 455;
}

void rgt_stop(void)
{
	TOGGLEBIT(PORTJ,BIT(3));
	TOGGLEBIT(PORTJ,BIT(2));
	OCR5B = 105;
}

void lft_stop(void)
{	
	TOGGLEBIT(PORTJ,BIT(0));
	TOGGLEBIT(PORTJ,BIT(1));
	OCR5A = 105;
}
//---------------------------------------------------


//-----------------DISC FUNCTIONS--------------------
void discrgt(void)
{
	OCR4B = 737;
	PORTL &= ~(1 << PL0);
}

void disclft(void)
{
	OCR4B = 737;
	PORTL |= (1 << PL0);
}

void discstop(void)
{
	OCR4B = 0;
}
//--------------------------------------------------

//-------------PARALLELOGRAM FUMCTIONS--------------
void ppup(void)
{
	OCR4A = 737;
	OCR4B = 737;
	CLEARBIT(PORTG,BIT(4));
	CLEARBIT(PORTL,BIT(0)); 
}

void ppdown(void)
{
	OCR4A = 737;
	OCR4B = 737;
	SETBIT(PORTG,BIT(4));
	SETBIT(PORTL,BIT(0));
}

void ppstop(void)
{
		OCR4A = ReversePWMForPP;
		OCR4B = ReversePWMForPP;
		CLEARBIT(PORTG,BIT(4));
		CLEARBIT(PORTL,BIT(0));
}
//--------------------------------------------------

//----------------GRIPPER MOTOR FUNCTION------------
void gripup(void)
{
	OCR3A = 700;	//PH5
	PORTH |= (1<<PINH6);
}

void gripdown(void)
{
	OCR3A = 700;	//PH5
	PORTH &= ~(1<<PINH6);
}

void gripstop(void)
{
	OCR3A = 40;	//PH5
	PORTH |= (1<<PINH6);
}
//--------------------------------------------------


void seesaw(void)
{
	//lcd_string("Seesaw");
	SETBIT(PORTA,BIT(2));		//suction cup
	
	SETBIT(DDRE,BIT(3));
	SETBIT(PORTE,BIT(3)); 
	
	int i;
	for(i = 0; i < 3; i++)
	{
		SETBIT(PORTA,BIT(0));		//seesaw cyl
		_delay_ms(2000);
		CLEARBIT(PORTA,BIT(0));
		_delay_ms(1000);
	}
	CLEARBIT(PORTA,BIT(2));		//suction cup
	CLEARBIT(PORTE,BIT(3));
}
