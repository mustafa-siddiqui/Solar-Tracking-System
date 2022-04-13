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
#include "../inc/spi.h"     // SPI_*() functions
//-//
#include <xc.h>             // OSCCON

/* get device ID */
unsigned char _getDeviceID(void) {
    // select register to read
    SPI_Write(DEVID);

    // read register and return value
    return SPI_Read();    
}

/* enable measurement mode */
void _enableMeasureMode(void) {
    // set bit D3 of POWER_CTL register
    unsigned char D3Set = SET(POWER_CTL, 3);
    SPI_Write(D3Set);
}

/* enable standby mode */
void _enableStandbyMode(void) {
    // clear bit D3 of POWER_CTL register
    unsigned char D3Clear = CLEAR(POWER_CTL, 3);
    SPI_Write(D3Clear);
}

/* get current x, y, z axis readings */
int* _getCurrentReading(void) {
    // TODO
}

/* initialize accelerometer module */
int initAccel(void) {
    // set internal frequency to 8 MHz for SPI communication
    OSCCON = 0x72;
    _enableMeasureMode();
    // TODO: set more registers as required

    // return 0 if cannot correctly read register
    if (_getDeviceID() != 0xE5) 
        return 0;
    
    return 1;
}

/* get zenith angle value */
int getCurrentZenith(void) {
    // TODO
}
