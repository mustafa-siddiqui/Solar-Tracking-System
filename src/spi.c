/**
 * @file    spi.c
 * @author  Mustafa Siddiqui
 * @brief   Function defitions for SPI communication on PIC18 MCU.
 * @date    04/14/2022
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
    _SPI_SCK = 1;
    SSPCON1bits.SSPEN = 1;
    _SPI_SCK = 0;

    // SDI is automatically configured but just in case
    _SPI_SDI = 1;    // input
    _SPI_SDO = 0;    // output
}

/* disable serial port function */
void _SPI_disableIO(void) {
    SSPCON1bits.SSPEN = 0;
    
    // return pins to default state: output
    _SPI_SCK = 0;
    _SPI_SDO = 0;
    _SPI_SDI = 0;
}

/* configure as master */
void initSPI(void) {
    _SPI_enableIO();

    // SSPEN is set in _SPI_enableIO(), don't write again
    SSPCON1bits.WCOL = 0;   // write collision detection bit => no collison
    SSPCON1bits.SSPOV = 0;  // recv overflow indicator bit => no overflow
    SSPCON1bits.CKP = 1;    // clock polarity select bit => idle state = high level (in line with accelerometer)

    // SSPM3:SSPM0: synchronous serial port mode select bits
    // 0010 => spi master mode, clock = f_osc / 64 
    SSPCON1bits.SSPM3 = 0;
    SSPCON1bits.SSPM2 = 0;
    SSPCON1bits.SSPM1 = 1;
    SSPCON1bits.SSPM0 = 0;

    // bit7 = 0: sample bit => input data sampled at middle of data output time
    // bit6 = 0: spi clock select bit => transmit occurs on transition from 
    //           idle to active clock state (falling edge in this case)
    // bit0 = 0: buffer full status bit => SSPBUFF is empty 
    SSPSTAT = 0x00;

    // when data (8 bits) is received, SSPSTATbits.BF (buffer full bit) &
    // PIR1bits.SSPIF (interrupt flag bit) are set
    // => we will use SSPIF to detect when transfer is complete
    PIR1bits.SSPIF = 0;
}

/* select slave device */
void _SPI_selectSlave(int slave) {
    switch (slave) {
        case ACCELEROMETER:
            _SPI_CS1 = 1;
            _SPI_CS2 = 0;
            break;
        case MAGNETOMETER:
            _SPI_CS1 = 0;
            _SPI_CS2 = 1;
            break;
        default:
            // don't select any if incorrect slave
            _SPI_CS1 = 0;
            _SPI_CS2 = 0;
            break;
    }
}

/* send 8 bits from PIC18 */
void _SPI_write(unsigned char data, int slave) {
    // select appropriate slave device
    _SPI_selectSlave(slave);

    // transfer data to SSPBUF register and wait for transmission
    // to complete
    SSPBUF = data;
    while (!PIR1bits.SSPIF) {}

    // clear interrupt flag
    PIR1bits.SSPIF = 0;
}

/* read n bits of data; n = length*8 */
void _SPI_read(unsigned char* data, int length) {
    unsigned int complete = 0;
    unsigned int numBytes = 0;
    int i = length - 1;

    while (!complete) {
        // if data is there in SSPBUF
        // TODO: might need to factor in interrupt bit as well
        if (SSPSTATbits.BF) {
            data[i--] = SSPBUF; 
            numBytes++;

            if (numBytes == length)
                complete = 1;
        }
    }

    // TODO: NOT SURE atm if overflow bit needs to be cleared
    // clear buffer full status bit
    SSPSTATbits.BF = 0;
}
