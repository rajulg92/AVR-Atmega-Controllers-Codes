#include "I2c1_640.h"

#define BUFFER_LENGTH 32


void begin(void);
void beginTransmission(int);
uint8_t endTransmission(void);
uint8_t requestFrom(uint8_t, uint8_t);
size_t write(uint8_t);
int available(void);
int read(void);
int peek(void);


uint8_t rxBuffer[BUFFER_LENGTH];
uint8_t rxBufferIndex = 0;
uint8_t rxBufferLength = 0;

uint8_t txAddress = 0;
uint8_t txBuffer[BUFFER_LENGTH];
uint8_t txBufferIndex = 0;
uint8_t txBufferLength = 0;

uint8_t transmitting = 0;


void begin(void)
{
	rxBufferIndex = 0;
	rxBufferLength = 0;

	txBufferIndex = 0;
	txBufferLength = 0;

	twi_init();
}

void beginTransmission(int address)
{
  // indicate that we are transmitting
  transmitting = 1;
  // set address of targeted slave
  txAddress = address;
  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
}

uint8_t endTransmission(void)
{
  // transmit buffer (blocking)
  int ret = twi_writeTo(txAddress, txBuffer, txBufferLength, 1, true);
  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
  // indicate that we are done transmitting
  transmitting = 0;
  return ret;
}

uint8_t requestFrom(uint8_t address, uint8_t quantity)
{
  // clamp to buffer length
  if(quantity > BUFFER_LENGTH){
    quantity = BUFFER_LENGTH;
  }
  // perform blocking read into buffer
  uint8_t read = twi_readFrom(address, rxBuffer, quantity, true);
  // set rx buffer iterator vars
  rxBufferIndex = 0;
  rxBufferLength = read;

  return read;
}

size_t write(uint8_t data)
{
  if(transmitting){
  // in master transmitter mode
    // don't bother if buffer is full
    if(txBufferLength >= BUFFER_LENGTH){
      //setWriteError();
      return 0;
    }
    // put byte in tx buffer
    txBuffer[txBufferIndex] = data;
    ++txBufferIndex;
    // update amount in buffer   
    txBufferLength = txBufferIndex;
  }else{
  // in slave send mode
    // reply to master
    twi_transmit(&data, 1);
  }
  return 1;
}

int available(void)
{
  return rxBufferLength - rxBufferIndex;
}

int read(void)
{
  int value = -1;
  
  // get each successive byte on each call
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }

  return value;
}

int peek(void)
{
  int value = -1;
  
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
  }

  return value;
}
