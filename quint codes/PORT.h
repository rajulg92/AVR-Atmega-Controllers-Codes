#define BIT(x)  (1<<(x))
#define cb(x,b)  x&b
#define SETBIT(x,b)  x|=b;
#define CLEARBIT(x,b)  x&=~b;
#define TOGGLEBIT(x,b)  x^=b;


/************************************************/
void ports_init(void)
{
DDRD=0b00110000;
DDRC=0b00001010;
}
/********************************************/

