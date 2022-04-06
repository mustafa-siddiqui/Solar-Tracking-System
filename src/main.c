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
//-//
#include <xc.h>

int main(void) {
    // set all pins as digital output
    initPins();

    while (1) {
        // turn on LEDs and delay
        LATDbits.LATD2 = 1;
        LATbits.LATD3 = 1;
        _delay(1000);

        // turn off LEDs and delay
        LATDbits.LATD2 = 0;
        LATDbits.LATD3 = 0;
        _delay(1000);

    }

    return 0;
}
