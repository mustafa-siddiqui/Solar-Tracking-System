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

#define _XTAL_FREQ 4000000  // 4 MHz

int main(void) {
    // set all pins as digital output
    initPins();
    //__delay_ms(10);
    _delay(10000);
    // initialize PIC18 as master for SPI
    initSPI();

    // initialize accelerometer for communication
    int status = initAccel();

    // turn off LEDs at start
    LATDbits.LATD2 = 0;
    LATDbits.LATD3 = 0;
    while (1) {
        if (status) {
            LATDbits.LATD2 = 1;
        }
    }

    return 0;
}
