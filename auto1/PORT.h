#define BIT(x)  (1<<(x))
#define CHECKBIT(x,b)  x&b
#define SETBIT(x,b)  x|=b;
#define CLEARBIT(x,b)  x&=~b;
#define TOGGLEBIT(x,b)  x^=b;


/************************************************/
/********************************************/
void port_init()
{
	DDRA=0XFF;
	DDRB=0XFF;
	DDRC=0XFF;
	DDRD=0X90;
	DDRE=0XFF;
	DDRF=0XFF;
	DDRG=0XFE;
	PORTD=0X6F;
	PORTG=0X01;
}

