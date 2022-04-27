/**
 * @file    init.c
 * @author  Mustafa Siddiqui
 * @brief   Functions for initialization process for PIC18 MCU.
 * @date    04/06/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../inc/init.h"
#include <xc.h>

/* initialize pins as digital outputs and set to 0 */
void initPins(void) {
    // set as digital output
    TRISA = 0x0;
    TRISB = 0x0;
    TRISC = 0x0;
    TRISD = 0x0;
    TRISE = 0x0;
    
    // set to 0
    LATA = 0x0;
    LATB = 0x0;
    LATC = 0x0;
    LATD = 0x0;
    LATE = 0x0;
    
}
