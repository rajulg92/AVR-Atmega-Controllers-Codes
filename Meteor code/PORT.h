#define BIT(x)  (1<<(x))
#define CHECKBIT(x,b)  x&b
#define SETBIT(x,b)  x|=b;
#define CLEARBIT(x,b)  x&=~b;
#define TOGGLEBIT(x,b)  x^=b;


/************************************************/
void ports_init(void)
{

DDRC=0X03;
DDRB=0X06;
DDRD=0X00;
PORTC |=(1<<PC5)|(1<<PC4)|(1<<PC3)| (1<<PC2);
}
/********************************************/

