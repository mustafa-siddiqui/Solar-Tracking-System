/* 
 * File:   uart.h
 * Author: SolarPv-2022
 * Comments: functions for uart transmit and receive
 */
 
#ifndef UART_H
#define	UART_H


#include <xc.h>
#include <pic18.h>
#include <string.h>
#include <stdlib.h>

void __interrupt() UARTRX_ISR();

void UART_RX_Init(void);
void UART_send_char(char );
void UART_send_str(const char *);
void MSdelay(unsigned int);
void UART_RX_Handle(void);

char UART_buffer[100];



#endif	/* UART_H */
