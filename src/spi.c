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
#include "../inc/uart.h"
//-//
#include <xc.h>

/* enable serial port function */
void _SPI_enableIO(void) {
    // keep SCK as input while SSPEN is configured
    // when done, set as output
    _SPI_SCK = 1;
    __delay_ms(1);
    SSPCON1bits.SSPEN = 1;
    __delay_ms(1);
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
    /*  
        => Note from the PIC18 datasheet:
        When the module is enabled and in master mode (CKE, SSPSTAT<6> = 1),
        a small glitch of approximately half a T_CY may be seen on the SCK pin.
        To resolve this, keep the SCK pin as an input while setting SPEN. Then,
        configure the SCK pin as an output (<TRISC3> = 0).

        => glitch is handled in _SPI_enableIO() <=
        => This leads me to think SSPSTAT should be set before SSPCON1 <=
    */

    // bit7 = 1: sample bit => input data sampled at middle (0) or at end (1) of data output time 
    // bit6 = 0: spi clock select bit (CKE) => transmit occurs on transition from 
    //           idle to active clock state (0), opposite for (1)
    // bit0 = 0: buffer full status bit => SSPBUFF is empty 
    SSPSTAT = 0x80;
 
    _SPI_enableIO();
    SSPCON1bits.WCOL = 0;   // write collision detection bit => no collison (0)
    SSPCON1bits.SSPOV = 0;  // recv overflow indicator bit => no overflow (0)
    // SSPEN = 1            // SSPEN is set in _SPI_enableIO(), don't write again
    SSPCON1bits.CKP = 1;    // clock polarity select bit => idle state = high level (1), low level (0)

    // SSPM3:SSPM0: synchronous serial port mode select bits
    // 0010 => spi master mode, clock = f_osc / 64 
    SSPCON1bits.SSPM3 = 0;
    SSPCON1bits.SSPM2 = 0;
    SSPCON1bits.SSPM1 = 1;
    SSPCON1bits.SSPM0 = 0;
    
    // Note: at this stage, SSPCON1 = 0x32;
    
    // when data (8 bits) is received, SSPSTATbits.BF (buffer full bit) &
    // PIR1bits.SSPIF (interrupt flag bit) are set
    // => we will use SSPIF to detect when transfer is complete
    PIR1bits.SSPIF = 0;
}

/* select slave device at start of transmission */
void _SPI_selectSlave(int slave) {
    // Chip Select pin (~CS) needs to pulled *low* in order to select it
    switch (slave) {
        case ACCELEROMETER:
            _SPI_CS1 = 0;
            _SPI_CS2 = 1;
            break;
        case MAGNETOMETER:
            _SPI_CS1 = 1;
            _SPI_CS2 = 0;
            break;
        default:
            // don't select any if incorrect slave
            _SPI_CS1 = 1;
            _SPI_CS2 = 1;
            break;
    }
}

/* unselect slave device at end of transmission */
void _SPI_unselectSlave(int slave) {
    // pull ~CS pin high
    switch (slave) {
        case ACCELEROMETER:
            _SPI_CS1 = 1;
            break;
        case MAGNETOMETER:
            _SPI_CS2 = 1;
            break;
        default:
            // leave CS pins unchanged if incorrect slave
            break;
    }
}

/* send 8 bits from PIC18 */
void _SPI_write(unsigned char data) {
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
