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

/* enable serial port function */
void _SPI_enableIO(void) {
    // keep SCK as input while SSPEN is configured
    // when done, set as output
    SCK = 1;
    SSPCON1bits.SSPEN = 1;
    SCK = 0;

    // SDI is automatically configured but just in case
    SDI = 1;    // input
    SDO = 0;    // output
}

/* disable serial port function */
void _SPI_disableIO(void) {
    SSPCON1bits.SSPEN = 0;
    
    // return pins to default state: output
    SCK = 0;
    SDO = 0;
    SDI = 0;
}

/* configure as master */
void _SPI_init(void) {
    // SSPEN is set in _SPI_enableIO(), don't write again
    SSPCON1bits.WCOL = 0;   // write collision detection bit => no collison
    SSPCON1bits.SSPOV = 0;  // recv overflow indicator bit => no overflow
    SSPCON1bits.CKP = 0;    // clock polarity select bit => idle state = low level 

    // SSPM3:SSPM0: synchronous serial port mode select bits
    // 0010 => spi master mode, clock = f_osc / 64 
    SSPCON1bits.SSPM3 = 0;
    SSPCON1bits.SSPM2 = 0;
    SSPCON1bits.SSPM1 = 1;
    SSPCON1bits.SSPM0 = 0;

    // bit7 = 0: sample bit => input data sampled at middle of data output time
    // bit6 = 1: spi clock select bit => transmit occurs on transition from 
    //           active to idle clock state
    // bit0 = 0: buffer full status bit => SSPBUFF is empty 
    SSPSTAT = 0x40;

    // when data (8 bits) is received, SSPSTATbits.BF (buffer full bit) &
    // PIR1bits.SSPIF (interrupt flag bit) are set
    // => we will use SSPIF to detect when transfer is complete
    PIR1bits.SSPIF = 0;
}

void _SPI_selectSlave(int slave) {
    switch (slave) {
        case ACCELEROMETER:
            CS1 = 1;
            CS2 = 0;
            break;
        case MAGNETOMETER:
            CS1 = 0;
            CS2 = 1;
            break;
        default:
            // don't select any if incorrect slave
            CS1 = 0;
            CS2 = 0;
            break;
    }
}
