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

/* initialize pins as digitial outputs */
void initPins(void) {
    TRISA = 0x0;
    TRISB = 0x0;
    TRISC = 0x0;
    TRISD = 0x0;
    TRISE = 0x0;
}
