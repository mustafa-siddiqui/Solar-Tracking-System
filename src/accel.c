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
#include "../inc/spi.h"     // _SPI_*() functions
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
    unsigned char data[1] = {0x00};
    // first byte consists of R/W, MB, and address of register to read
    unsigned char dataByte_1 = _ACCEL_createDataByte1(1, 0, addr);

    // write on MOSI line
    _SPI_write(dataByte_1, ACCELEROMETER);

    // receive from MISO line
    _SPI_read(data, 1);

    return data[0];
}

/* get device ID */
unsigned char _ACCEL_getDeviceID(void) {
    return _ACCEL_readFromRegister(_ADDR_DEVID);
}

/* enable measurement mode */
void _ACCEL_enableMeasureMode(void) {
    // set bit D3 of POWER_CTL register
    // read from register, modify the specific bit, and write
    // => this is done so that other bits are not changed in the process
    unsigned char reg = _ACCEL_readFromRegister(_ADDR_POWER_CTL);
    SET(reg, 2);
    _ACCEL_writeToRegister(_ADDR_POWER_CTL, reg);
}

/* enable standby mode */
void _ACCEL_enableStandbyMode(void) {
    // clear bit D3 of POWER_CTL register
    unsigned char reg = _ACCEL_readFromRegister(_ADDR_POWER_CTL);
    CLEAR(reg, 2);
    _ACCEL_writeToRegister(_ADDR_POWER_CTL, reg);
}

/* get current x, y, z axis readings */
int* _ACCEL_getCurrentReading(void) {
    // TODO
}

/* initialize accelerometer module */
int initAccel(void) {
    // set internal frequency to 8 MHz for SPI communication
    // NOTE: might need to reduce it to 4 MHz as accelerometer doesn't support that much speed
    //OSCCON = 0x72;
    _ACCEL_enableMeasureMode();
    // TODO: set more registers as required
    // data_format reg:
    //      select 4-wire mode -> bit D6 in data_format reg 
    //      (full_res -- bit D3 -- might need to be set also)
    //      range (bits D1 & D0) specify the range -- 2g, 4g, etc
    // => [self_test, spi, int_invert, 0, full_res, justify, range<1:0>]
    unsigned char reg_dataFormat = 0x0C;
    _ACCEL_writeToRegister(_ADDR_DATA_FORMAT, reg_dataFormat);

    // max SPI clock speed = 5 MHz => CONFLICT with current 8 MHz
    // multiple byte reads possible -> set MB bit in first byte transfer
    // select rate_bits in bw_rate to set output data rate
    //      => default value is 0x0A which translates to 100 Hz, let's keep it at that atm

    // return 1 if correctly read register
    if (_ACCEL_getDeviceID() & 0xE5) 
        return 1;
    
    return 0;
}

/* get zenith angle value */
int getCurrentZenith(void) {
    // TODO
}
