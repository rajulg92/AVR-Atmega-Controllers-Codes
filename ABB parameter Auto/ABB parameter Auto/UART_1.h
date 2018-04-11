/*
 * UART.h
 *
 * Created: 2014-08-09 12:46:05 PM
 *  Author: Samir
 */ 


#ifndef UART_H_
#define UART_H_

#define BAUD 9600

extern void UART_1_Init();
extern void UART_1_Putc(unsigned char data);
extern void UART_1_Puts(char *str);

#endif /* UART_H_ */