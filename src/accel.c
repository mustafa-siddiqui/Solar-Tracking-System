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

/* create first data byte for SDI line for accelerometer */
unsigned char _ACCEL_createDataByte1(int RW, int MB, unsigned char addr) {
    unsigned char dataByte1 = 0x0;
    dataByte1 |= addr;

    // set read/write bit
    if (RW == 1)
        SET(dataByte1, 7);
    else
        CLEAR(dataByte1, 7);

    // set multiple byte transmission bit
    if (MB == 1)
        SET(dataByte1, 6);
    else
        CLEAR(dataByte1, 6);

    return dataByte1;
}

/* write to register on accelerometer */
void _ACCEL_writeToRegister(unsigned char addr, unsigned char data) {
    // first byte consists of R/W, MB, and address bits (of the register to write to)
    unsigned char dataByte_1 = _ACCEL_createDataByte1(0, 0, addr);

    // latter byte is data to be written
    unsigned char dataByte_2 = data;

    _SPI_write(dataByte_1, ACCELEROMETER);
    _SPI_write(dataByte_2, ACCELEROMETER);
}

/* read from register on accelerometer */
unsigned char _ACCEL_readFromRegister(unsigned char addr) {
    // first byte consists of R/W, MB, and address of register to read
    unsigned char dataByte_1 = _ACCEL_createDataByte1(1, 0, addr);

    // dummy data for transmission
    unsigned char dataByte_2 = 0x00;

    // write on MOSI line
    _SPI_write(dataByte_1, ACCELEROMETER);
    _SPI_write(dataByte_2, ACCELEROMETER);

    // receive from MISO line
    int length = 1;
    unsigned char data[length];
    _SPI_read(data, length);

    return data[0];
}

/* get device ID */
unsigned char _getDeviceID(void) {
    return _ACCEL_readFromRegister(_ADDR_DEVID);
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
    // data_format reg:
    //      select 4-wire mode -> bit D6 in data_format reg 
    //      (full_res -- bit D3 -- might need to be set also)
    //      range (bits D1 & D0) specify the range -- 2g, 4g, etc
    // max SPI clock speed = 5 MHz => CONFLICT with current 8 MHz
    // multiple byte reads possible -> set MB bit in first byte transfer
    // select rate_bits in bw_rate to set output data rate

    // return 0 if cannot correctly read register
    if (_getDeviceID() != 0xE5) 
        return 0;
    
    return 1;
}

/* get zenith angle value */
int getCurrentZenith(void) {
    // TODO
}
