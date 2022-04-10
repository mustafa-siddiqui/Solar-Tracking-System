/**
 * @file    spi.c
 * @author  Ali Choudhry
 * @brief   Function defitions for SPI communication on PIC18 MCU.
 * @date    04/10/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../inc/spi.h"
//-//
#include <xc.h>

/* initialize master module */
void SPI_Init_Master(void) {
    /* PORT definition for SPI pins*/    
    TRISBbits.TRISB0 = 1;	/* RB0 as input(SDI) */
    TRISBbits.TRISB1 = 0;		/* RB1 as output(SCK) */
    TRISAbits.TRISA5 = 0;		/* RA5 as a output(SS') */
    TRISCbits.TRISC7 = 0;		/* RC7 as output(SDO) */

    /* To initialize SPI Communication configure following Register*/
    CS = 1;
    SSPSTAT = 0x40;		/* Data change on rising edge of clk , BF=0*/
    SSPCON1 = 0x22;		/* Master mode,Serial enable,
                            idle state low for clk, fosc/64 */ 
    PIR1bits.SSPIF = 0;

    /* Disable the ADC channel which are on for multiplexed pin when
    used as an input */    
    ADCON0 = 0;			/* This is for de-multiplexed the 
                            SCL and SDI from analog pins*/
    ADCON1 = 0x0F;		/* This makes all pins as digital I/O */
}

/* initialize slave module */
void SPI_Init_Slave(void) {
    /* PORT definition for SPI pins*/    
    TRISBbits.TRISB0 = 1;	/* RB0 as input(SDI) */
    TRISBbits.TRISB1 = 1;	/* RB1 as output(SCK) */
    TRISAbits.TRISA5 = 1;	/* RA5 as a output(SS') */
    TRISCbits.TRISC7 = 0;	/* RC7 as output(SDO) */

    /* To initialize SPI Communication configure following Register*/
    CS = 1;
    SSPSTAT = 0x40;		/* Data change on rising edge of clk , BF=0*/
    SSPCON1 = 0x24;		/* Slave mode,Serial enable, idle state 
				            high for clk */ 
    PIR1bits.SSPIF = 0;

    /* Disable the ADC channel which are on for multiplexed pin 
    when used as an input */    
    ADCON0 = 0;			/* This is for de-multiplexed the SCL
				            and SDI from analog pins*/
    ADCON1 = 0x0F;		/* This makes all pins as digital I/O */    
}

/* transmit data */
void SPI_Write(unsigned char x) {
    unsigned char data_flush;
    SSPBUF = x;			        /* Copy data in SSBUF to transmit */
    while (!PIR1bits.SSPIF);	/* Wait for complete 1 byte transmission */
    PIR1bits.SSPIF = 0;		    /* Clear SSPIF flag */
    data_flush = SSPBUF;		/* Flush the data */
}

/* read data received */
unsigned char SPI_Read(void) {    
    SSPBUF = 0xff;		    /* Copy flush byte in SSBUF */
    while(!PIR1bits.SSPIF);	/* Wait for complete 1 byte transmission */
    PIR1bits.SSPIF = 0;

    /* Return received byte */ 
    return(SSPBUF);  
}

/* Delay of 1 ms for 8MHz Frequency */
void MSdelay(unsigned int val) {	
     unsigned int i,j;
        for (i = 0; i < val; i++)
            for (j = 0; j < 165; j++);
}
