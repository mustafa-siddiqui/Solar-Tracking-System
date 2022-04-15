/**
 * @file    main.c
 * @author  Ali Choudhry, Carter Bordeleau, Mahmud Jumaev, Mustafa Siddiqui.
 * @brief   Main logic for the solar tracking system.
 * @date    04/06/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../inc/init.h"
#include "../inc/spi.h"
#include "../inc/accel.h"
//-//
#include <xc.h>

#define _XTAL_FREQ 8000000  // 4 MHz

int main(void) {
    // set clock freq to 8 MHz
    OSCCON = 0x72;
    
    // set all pins as digital output
    initPins();

    
    // initialize PIC18 as master for SPI
    initSPI();
    LATDbits.LATD2 = 1;
    LATDbits.LATD3 = 1;
    _delay(1000000);
    _delay(1000000);
    
    // initialize accelerometer for communication
    int status = initAccel();
    //int status = 1;
    //unsigned char dataRecv = 0x22;
    // turn off LEDs at start
    LATDbits.LATD2 = 0;
    LATDbits.LATD3 = 0;
    _delay(1000000);
    _delay(1000000);
    
    while (1) {
        if (status) {
            LATDbits.LATD2 = 1;
        }
        /*
        _delay(1000000);
        //LATDbits.LATD3 = 0;
        _delay(1000000);
        _SPI_write(0x00, ACCELEROMETER);
        _SPI_read(&dataRecv, 1);
        _delay(1000000);
        //LATDbits.LATD3 = 0;
        if (!(dataRecv & 0x22)) {
            status = 0;
        }
        */
    }

    return 0;
}
