/*
 * UART.h
 *
 * Created: 2014-08-09 12:46:05 PM
 *  Author: Samir
 */ 


#ifndef UART_H_0
#define UART_H_0

#define BAUD 9600

extern void UART_0_init(void);
extern void UART_0_putc(unsigned char data);
extern void UART_0_puts(char *str);

#endif /* UART_H_ */