/**
 * @file    accel.c
 * @author  Mustafa Siddiqui
 * @brief   Function definitions for accelerometer module.
 * @date    04/12/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../inc/accel.h"
//-//
//#include <xc.h>

/* get device ID */
unsigned char _getDeviceID(void) {
    // read data from DEVID register
    // => Not sure how to exactly do that atm <=
    return DEVID;
}

/* enable measurement mode */
void _enableMeasureMode(void) {
    // set bit D3 of POWER_CTL register
    SET(POWER_CTL, 3);
}

/* enable standby mode */
void _enableStandbyMode(void) {
    // clear bit D3 of POWER_CTL register
    CLEAR(POWER_CTL, 3);
}

/* get current x, y, z axis readings */
int* _getCurrentReading(void) {
    // TODO
}

/* initialize accelerometer module */
void initAccel(void) {
    // TODO
}

/* get zenith angle value */
int getCurrentZenith(void) {
    // TODO
}
