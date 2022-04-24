/*
 * File:   uart.c
 * Author: SeniorDesign-2022
 *
 * Created on April 15, 2022, 2:35 PM
 */


#include "../inc/uart.h"



/*
 * Interrupt that handles an incoming UART character
 */
char UART_Read_char()
{
while(RCIF==0); // see if data on RC7 pin is available 
if(RCSTAbits.OERR)
{ 
CREN = 0;
NOP();
CREN=1;
}
return(RCREG); //read the byte from receive register and return value
}



/*
 * Initialize UART and UART RX interrupt
 */
void UART_RX_Init(void){
    
    // Set the RX-TX pins to be in UART mode, not I/O
    TRISCbits.RC7 = 1;
    TRISCbits.RC6 = 0;
    
    SPBRG = 51; // 9600bps @ 8MHz, 8bit baud rate generator, high speed
    
    RCSTAbits.CREN = 1; // Asyncronous mode continuous receiver
    RCSTAbits.SPEN = 1; // Serial port enabled
    BAUDCONbits.BRG16 = 0; // 8bit baud rate generator
    
    TXSTAbits.SYNC = 0; // Asyncronous mode
    TXSTAbits.BRGH = 1; // High speed
    TXSTAbits.TXEN = 1; // Transmit enable   

    
}

/*
 * Send a single character over UART
 */
void UART_send_char(char c){
    while(!PIR1bits.TXIF); // Wait until TXREG is empty
    TXREG=c;
}

/*
 * Send a null terminating string over UART
 */
void UART_send_str(const char *str) {
    for (char character = *str; character != '\0'; character = *++str) {
        UART_send_char(character);
    }
}







