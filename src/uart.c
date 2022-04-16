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
void __interrupt() UARTRX_ISR() {
    char a;
    if(RCIF==1)                     //Polling for reception interrupt
    {
       
        a = RCREG;   //save received character
        strncat(UART_buffer, &a, 1);  //add it to the buffer
        if (a=='\0' || a==0 || a=='\r') {
            UART_RX_Handle(); 
        }
        RCIF = 0; //reset interrupt status flag
    }  
    
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
    
    //clear buffer
    memset(UART_buffer,0,sizeof(UART_buffer));
    
    IPR1bits.RCIP = 1; // Receive interrupt high priority
    PIE1bits.RCIE = 1; // UART receive interrupt enabled
    PIR1bits.RCIF=0; //set flag to 0
    
    
    RCIE=1;     
    PEIE=1;
    
    GIE = 1;  //enable global interrupts
    
    
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

void MSdelay(unsigned int val){	/* Delay of 1 ms for 8MHz Frequency */
     unsigned int i,j;
        for(i=0;i<val;i++)
            for(j=0;j<165;j++);
}


/*
 *Define what should happen when the RX buffer is full
 */
void UART_RX_Handle() {
    //do something with UART buffer
    UART_send_str(UART_buffer); //for now just send it back over UART
    //clear buffer
    memset(UART_buffer,0,sizeof(UART_buffer));
}






