/**
 * @file   uart.h
 * @brief  Functions for UART transmission & receival.
 * @author Carter Bordeleau
 * @date   04/23/2022
 */
 
#ifndef UART_H
#define	UART_H

#include <xc.h>
#include <pic18.h>
#include <string.h>
#include <stdlib.h>

// fn declarations
char UART_Read_char(void);
void UART_RX_Init(void);
void UART_send_char(char );
void UART_send_str(const char *);

char UART_buffer[100];

#endif	/* UART_H */
