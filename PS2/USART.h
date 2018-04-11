uint16_t a = 0, b = 0, c = 0;
void Timer_init()
{
	TCCR1B = 0x07;
	DDRD &= ~(1<<6);
	PORTD |= (1<<6);
}

void Usart_init(void)
{
	UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
	UCSR1C |= (1 << UCSZ11) | (1 << UCSZ01);
	UCSR1A |= (1<<U2X1);
	UBRR1H = 0;
	UBRR1L = 191;
	DDRD &= ~(1<<PD2);
	DDRD |= (1<<PD3);	
	
	PORTD &= ~(1<<PD2);
	PORTD |= (1<<PD3);
}

uint8_t UartReceive(void)
{
	while( !(UCSR1A & (1<<RXC1)) );
	return UDR1;
}

void UartTransmit(uint8_t data)
{
	while ( !( UCSR1A & (1<<UDRE1)) );
	UCSR1B &= ~(1<<TXB81);
	if ( data & 0x0100 )
		UCSR1B |= (1<<TXB81);
	UDR1 = data;
}

uint32_t Enc_X(void)
{
	UartTransmit(10);
	c = UartReceive();
	b = UartReceive();
	a = (b<<8) | c;
	return a;
}

uint32_t Enc_Y(void)
{
	return TCNT1;
}
uint8_t Direction_X(void)
{
	UartTransmit(50);
	return UartReceive();
}

void ResetEnc_X(void)
{
	UartTransmit(30);
}

void Interruot_Init(void)
{
	CLEARBIT(DDRD, BIT(0));
	CLEARBIT(DDRD, BIT(1));
	//PORTD |= (1<<0) | (1<<1);
	EICRA |= (1<<ISC00) | (1<<ISC01) | (1<<ISC10) | (1<<ISC11);
	EIMSK |= (1<<INT0) | (1<<INT1); 
}
