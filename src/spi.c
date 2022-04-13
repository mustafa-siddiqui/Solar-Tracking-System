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
    TRISCbits.TRISC4 = 1;	/* RC4 as input(SDI) */
    TRISCbits.TRISC3 = 0;		/* RC3 as output(SCK) */
    TRISAbits.TRISA5 = 0;		/* RA5 as a output(SS) */
    TRISCbits.TRISC5 = 0;		/* RC5 as output(SDO) */

    /* To initialize SPI Communication configure following Register*/
    CS = 1; /* CHECK: double-check if this is indeed the right pin for master operation. I think it should be the CS pin. */
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
void SPI_Init_Slave(int slave) {
    /* PORT definition for SPI pins*/    
    TRISCbits.TRISC4 = 1;	/* RC4 as output(SDI) */
    TRISCbits.TRISC3 = 1;		/* RC3 as input(SCK) */
    TRISAbits.TRISA5 = 1;		/* RA5 as input(SS) */
    TRISCbits.TRISC5 = 0;		/* RC5 as input(SDO) */

    /* To initialize SPI Communication configure following Register*/
    CS = 1; // doubtful if this is needed in slave initialization (considering this is CS and not SS)
    switch (slave) {
        case ACCELEROMETER:
            SS1 = 1;
            SS2 = 0;
            break;
        case MAGNETOMETER:
            SS1 = 0;
            SS2 = 1;
            break;
        default:
            // do not initialize if incorrect slave
            SS1 = 0;
            SS2 = 0;
            break;
    }

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
    SSPBUF = x;			/* Copy data in SSBUF to transmit */
    while(!PIR1bits.SSPIF);	/* Wait for complete 1 byte transmission */
    PIR1bits.SSPIF = 0;		/* Clear SSPIF flag */
    data_flush = SSPBUF;		/* Flush the data */
}

/* read data received */
unsigned char SPI_Read(void) {    
    SSPBUF = 0xff;		/* Copy flush byte in SSBUF */
    while(!PIR1bits.SSPIF);	/* Wait for complete 1 byte transmission */
    PIR1bits.SSPIF = 0;
    return(SSPBUF);		/* Return received byte */   
}

/* Delay of 1 ms for 8MHz Frequency */
void MSdelay(unsigned int val) {
     unsigned int i,j;
        for(i=0;i<val;i++)
            for(j=0;j<165;j++);
}
