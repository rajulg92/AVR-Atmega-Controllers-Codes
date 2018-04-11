#define BIT(x)  (1<<(x))
#define cb(x,b)  x&b
#define SETBIT(x,b)  x|=b;
#define CLEARBIT(x,b)  x&=~b;
#define TOGGLEBIT(x,b)  x^=b;


/************************************************/
void ports_init(void)
{

DDRA=0x00;
DDRD=0xFF;
DDRC=0b00001010;
}
/********************************************/

