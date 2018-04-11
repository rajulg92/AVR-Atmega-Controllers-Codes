Hallo erstmal
ich hätte eine rissen bitte da ich mich mit c Null auskenne könnte mir 
jemand diesen Code erklären oder eventuell ins bascom übersetzen ?


/*********************************************************************** 
*************************************

SPI Interface code for Playstation (Ps1 or Ps2) game controllers.

version:   1.4
date:      April 4, 2009
compiler:   gcc-avr compiler
AVR:      ATmega168
clock source:   16 MHz external crystal

The Playstation Controller Pinouts 
(http://www.gamesx.com/controldata/psxcont/psxcont.htm)

        LOOKING AT THE PLUG (male connector)
        -------------------------------
 PIN 1->| o  o  o | o  o  o | o  o  o |
        \_____________________________/

PIN # USAGE

   1. DATA
   2. COMMAND
   3. N/C (9 Volts unused)
   4. GND
   5. VCC
   6. ATT
   7. CLOCK
   8. N/C
   9. ACK


*Note: All Playstation controllers are abbreviated as DSC (Dualshock 
Controller) in the code below.

************************************************************************ 
*************************************/


#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#define BAUDRATE      57600   // Define baudrate for RS232 communication
#define   STATUS_LED_RED      PC2   // Red LED cathode on PC2; indicates 
no controller or communication error
#define STATUS_LED_GREEN   PC4   // Green LED cathode on PC4; indicates 
proper communication with DSC
#define COMMAND         PB3   // SPI MOSI; sends commands from AVR to 
DSC
#define DATA         PB4   // SPI MISO; receives incoming data from DSC; 
1k external pull-up required
#define CLOCK         PB5   // SPI SCK; serial clock controlled by AVR, 
DATA is read on rising edge
#define ATTENTION      PC1   // Initiates and closes each data packet 
transmission
#define MODE_SELECT      PC0   // MODE_SELECT pin state defines DSC 
mode; low = full analog (Ps2 only), high = default
#define SS         PB2   // SPI slave select pin; set as output to 
ensure AVR remains SPI master
#define MODE_SELECT_PORT   PORTC
#define MODE_SELECT_PORT_IN   PINC
#define LED_PORT      PORTC
#define ATTENTION_PORT      PORTC


/* Global Variables */
uint8_t received_byte, analog_flag, data[18];


/* init() sets all registers and enables necessary interrupts */
void init(void){

   /*Set pin I/O registers*/
   // Set output on SCK (CLOCK) and MOSI (COMMAND) pins for spi 
communication
   DDRB |= _BV(SS) | _BV(COMMAND) | _BV(CLOCK);
   // Set output mode for all status LEDs and ATTENTION pin
   DDRC |= _BV(STATUS_LED_RED) | _BV(STATUS_LED_GREEN) | _BV(ATTENTION);
   // Enable internal pull-up resistor on mode select pin
   MODE_SELECT_PORT |= _BV(MODE_SELECT);

   /* Configure SPI registers (pg.173) */
   // SPE enables SPI hardware; DORD sets data order to LSB first; MSTR 
selects master mode; CPOL sets clock polarity (high when idle)
   // CPHA selects data setup on falling edge of clock, read on rising 
edge; SPR1 sets clock prescale factor at 64 (250kHz clock)
   SPCR |= _BV(SPE) | _BV(DORD) | _BV(MSTR) | _BV(CPOL) | _BV(CPHA) | 
_BV(SPR1);

   /* Configure USART registers */
   // Equations for calculating and setting UBRR0L (baudrate register) 
(pg. 179)
   if((F_CPU  16  BAUDRATE - 1) < 20){
     UCSR0A |= _BV(U2X0);
     UBRR0L = F_CPU  8  BAUDRATE - 1;}
     else{
     UBRR0L = F_CPU  16  BAUDRATE - 1;}
   // USART control/status register B (pg. 196); RXCIE0 enables RX 
complete interrupt; TXEN0/RXEN0 enable receiver/ transmitter
   UCSR0B |= _BV(RXCIE0) | _BV(TXEN0) | _BV(RXEN0);
   // USART control and status register C (pg. 197); UCSZ01 and UCSZ00 
sets character size to 8 bits;
   UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);

   /* Enable global interrupts*/
   sei();

   /* Clear status flags. */
   uint8_t x;
       x = SPSR;
       x = SPDR;
}


/* Core AVR-DSC send/receive communication function; takes command_byte, 
returns single data byte from controller. */
uint8_t ps2_comm(uint8_t command_byte){

   SPDR = command_byte;         // Write command byte to SPI data 
register to begin transmission (pg. 175)
   while(!(SPSR & _BV(SPIF)));      // Loop to prevent write collision
   _delay_us(20);            // Delay between byte transmissions
   return(SPDR);            // Return byte from controller in shift 
register receive buffer (pg. 175)
}


/* Function to simplify configuration of DSC; requires 4 command bytes, 
loop_bytes sets number of bytes following the default 6 */
void config_comm(uint8_t byte2, uint8_t byte4, uint8_t byte5, uint8_t 
byte6, uint8_t loop_bytes){

   ATTENTION_PORT &= ~_BV(ATTENTION);

   _delay_us(16);

   ps2_comm(0x01);
   analog_flag = ps2_comm(byte2);
   ps2_comm(0x00);
   ps2_comm(byte4);
   ps2_comm(byte5);
   ps2_comm(byte6);

   for(uint8_t x = 0; x < loop_bytes; x++)
      ps2_comm(0x00);

   _delay_us(16);

   ATTENTION_PORT |= _BV(ATTENTION);

   _delay_ms(16);
}


/* USART RS232 transmit single byte function */
void avr_transmit_byte (uint8_t data) {
   while (!(UCSR0A & _BV(UDRE0)));    // Wait for USART data register 
empty flag (pg. 195)
   UDR0 = data;            // Write byte to USART I/O data register to 
initiate transmission (p. 195)
}


/*Interrupt routines*/
// USART receive completion interrupt service routine
ISR(SIG_USART_RECV){

   received_byte = UDR0;         // Store byte received from serial port

   if(received_byte == 0x7A){      // If byte received is 0x7A ("z" in 
ASCII)

      for(uint8_t x = 0; x < ((analog_flag & 0x0F) << 1); x++)   // Loop 
transmits mode specific byte count from data array
         avr_transmit_byte(data[x]);

      received_byte = 0;
   }
}


int main(void){

   init();

   /* Infinite while loop*/
   while(1){

      LED_PORT |= _BV(STATUS_LED_GREEN);      // Green status LED off
      LED_PORT &= ~_BV(STATUS_LED_RED);      // Red status LED on (no 
controller/communication error)

      analog_flag = 0x00;            // Variable to verify controller 
communication/mode status

      /* DSC initialization/configuration loop; protocol and command 
details from http://www.curiousinventor.com/guides/ps2 */

      while((analog_flag != 0x79) && (analog_flag != 0x73) && 
(analog_flag != 0x41)){

         config_comm(0x43, 0x01, 0x00, 0x00, 0x00);   // Put controller 
in config mode
         config_comm(0x44, 0x01, 0x03, 0x00, 0x03);   // Set up for 
analog mode
         if(~MODE_SELECT_PORT_IN & _BV(MODE_SELECT))   // Check for mode 
selection
         config_comm(0x4F, 0xFF, 0xFF, 0x03, 0x03);   // Configures full 
analog mode (if supported)
         config_comm(0x43, 0x00, 0x5A, 0x00, 0x03);   // Exit config 
mode
         config_comm(0x42, 0x00, 0x00, 0x00, 0x0F);   // Poll once with 
standard header
      }

      LED_PORT &= ~_BV(STATUS_LED_GREEN);      // Green status LED on 
(controller connected and configured)
      LED_PORT |= _BV(STATUS_LED_RED);      // Red status LED off

      /* Auto-configured polling loop for all three controller modes */
      while((((analog_flag == 0x41) || (analog_flag == 0x73)) && 
(MODE_SELECT_PORT_IN & _BV(MODE_SELECT))) || ((analog_flag == 0x79) && 
(~MODE_SELECT_PORT_IN & _BV(MODE_SELECT)))){

         ATTENTION_PORT &= ~_BV(ATTENTION);   // Begin communication 
with DSC

         _delay_us(16);            // Delay between ATTENTION dropping 
low and data transmission

         ps2_comm(0x01);            // byte 0; standard header
         analog_flag = ps2_comm(0x42);      // byte 1; standard header
         ps2_comm(0x00);            // byte 2; standard header

         for(uint8_t x = 0; x < ((analog_flag & 0x0F) << 1); x++)   // 
Loop loads data array with mode specific byte count
            data[x] = ps2_comm(0x00);

         _delay_us(16);            // Delay following data transmission 
before ATTENTION returns high

         ATTENTION_PORT |= _BV(ATTENTION);   // End communication with 
DSC

         _delay_ms(16);            //Delay for roughly 60 samples per 
second
      }
         _delay_ms(16);
   }
}