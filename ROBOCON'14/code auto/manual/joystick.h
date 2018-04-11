#define PSclock            0               // PD0   blue
#define PSdata             1               // PD1	brown
#define PSacknolage  2               // PD2			green
#define PScommand   3               // PD3			orange
#define PSattention     4               // PD4		yellow

unsigned char chk_ana = 0;
int data0=0,data1=0,data2=0,data3=0,data4=0,data5=0,temp=0;
int gameByte(int command);
void joystick_config(void);
void ps2_init(void);
void joystick_poll(void);

int gameByte(int command)
{
        int i ;
        _delay_us(1);
        int data = 0x00;                             // clear data variable to save setting low bits later.
        for(i=0;i<8;i++)
        {
                if(command & _BV(i)) 
				{
				PORTD|=(1<<PScommand);//sbi(PORTD, PScommand);
				}// bit bang "command" out on PScommand wire.
                else
				{
				PORTD&=~(1<<PScommand);
				}
                PORTD&=~(1<<PSclock);                             // CLOCK LOW
                _delay_us(1);                                              // wait for output to stabilise
                if((PIND & _BV(PSdata)))
				{
					data|=(1<<i);  // read PSdata pin and store
                }//else cbi(data, i);
                PORTD|=(1<<PSclock);                             // CLOCK HIGH
        }
        PORTD|=(1<<PScommand);

        _delay_us(20);                                                   // wait for ACK to pass.

        return(data);
}


void joystick_config(void)
{
	 // put controller in config mode
       PORTD|=(1<<PScommand);
       PORTD|=(1<<PSclock);
       PORTD&=~(1<<PSattention);

       gameByte(0x01);
       gameByte(0x43);
       gameByte(0x00);
       gameByte(0x01);
       gameByte(0x00);

       PORTD|=(1<<PScommand);
       _delay_ms(1);
       PORTD|=(1<<PSattention);

       _delay_ms(10);

       // put controller in analouge mode
       PORTD|=(1<<PScommand);
       PORTD|=(1<<PSclock);
       PORTD&=~(1<<PSattention);

       gameByte(0x01);
       gameByte(0x44);
       gameByte(0x00);
       gameByte(0x01);
       gameByte(0x03);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);

       PORTD|=(1<<PScommand);
       _delay_ms(1);
       PORTD|=(1<<PSattention);

       _delay_ms(10);

       // exit config mode
       PORTD|=(1<<PScommand);
       PORTD|=(1<<PSclock);
       PORTD&=~(1<<PSattention);

       gameByte(0x01);
       gameByte(0x43);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x5A);
       gameByte(0x5A);
       gameByte(0x5A);
       gameByte(0x5A);
       gameByte(0x5A);

       PORTD|=(1<<PScommand);
       _delay_ms(1);
       PORTD|=(1<<PSattention);

       _delay_ms(10);

       // poll controller and check in analouge mode.
       PORTD|=(1<<PScommand);
       PORTD|=(1<<PSclock);
       PORTD&=~(1<<PSattention);

       gameByte(0x01);
       chk_ana = gameByte(0x42);            // the 2nd byte to be returned from the controller should = 0x73 for "red" analouge controller.
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);

       PORTD|=(1<<PScommand);
       _delay_ms(1);
       PORTD|=(1<<PSattention);
       _delay_ms(10);
		
}

void ps2_init(void)
{
	DDRD|=(1<<PD0);                 // clock. output. (blue)

   DDRD&=~(1<<PD1);                 // data. input. (brown)
   PORTD|=(1<<PD1);               //    enable pullup resistor

   DDRD&=~(1<<PD2);                 // acknolage. input. (green)
  PORTD|=(1<<PD2);               //    enable pullup resistor

   DDRD|=(1<<PD3);                 // command. output. (orange)

   DDRD|=(1<<PD4);                 // attention. output. (yellow)
}

void joystick_poll(void)
{
		PORTD|=(1<<PScommand);                          // start communication with PSx controller
        PORTD|=(1<<PSclock);
        PORTD&=~(1<<PSattention);

        gameByte(0x01);                                       // bite 0. header.
        temp = gameByte(0x42);                          // bite 1. header. (should possibly put test on this byte to detect unplugging of controller.)
        gameByte(0x00);                                       // bite 2. header.

        data0 = gameByte(0x00);                         // bite 3. first data bite.
        data1 = gameByte(0x00);                         // bite 4.
        data2 = gameByte(0x00);                         // bite 5.
        data3 = gameByte(0x00);                         // bite 6.
        data4 = gameByte(0x00);                         // bite 7.
        data5 = gameByte(0x00);                         // bite 8.

        _delay_us(1);
        PORTD|=(1<<PScommand);                      // close communication with PSx controller
        _delay_us(1);
        PORTD|=(1<<PSattention);                        // all done.
}