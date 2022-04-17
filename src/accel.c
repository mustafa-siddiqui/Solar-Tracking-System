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
#include "../inc/uart.h"
//-//
#include <xc.h>
#include <stdio.h>  // sprintf()

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
    // latter byte is data to be written
    unsigned char dataByte_1 = _ACCEL_createDataByte1(0, 0, addr);
    unsigned char dataByte_2 = data;

    // start transmission by pulling ~CS low
    _SPI_selectSlave(ACCELEROMETER);
    
    //_SPI_write(dataByte_1);
    //_SPI_write(dataByte_2);
    int status1 = WriteSPI(dataByte_1);
    int status2 = WriteSPI(data);

    // end transmisson by setting ~CS high
    _SPI_unselectSlave(ACCELEROMETER);
    
    if (status1 || status2) {
        UART_send_str("SPI write not successful");
    }

}

/* read from register on accelerometer */
unsigned char _ACCEL_readFromRegister(unsigned char addr) {
    // to store received data
    unsigned char data[1] = {0x00};

    // first byte consists of R/W, MB, and address of register to read
    unsigned char dataByte_1 = _ACCEL_createDataByte1(1, 0, addr);

    // start transmission by pulling ~CS low
    _SPI_selectSlave(ACCELEROMETER);
    
    // write on MOSI line & receive on MISO line
    //_SPI_write(dataByte_1);
    WriteSPI(dataByte_1);
    //_SPI_read(data, 1);
    data[0] = ReadSPI(0x00);
    
    // end transmission by setting ~CS high
    _SPI_unselectSlave(ACCELEROMETER); 
    
    return data[0];
}

/* get device ID */
unsigned char _ACCEL_getDeviceID(void) {
    return _ACCEL_readFromRegister(_ADDR_DEVID);
}

/* get current x, y, z axis readings */
int* _ACCEL_getCurrentReading(void) {
    // TODO
    return NULL;
}

/* initialize accelerometer module */
int initAccel(void) {
    // enable measurement mode (set bit D3)
    _ACCEL_writeToRegister(_ADDR_POWER_CTL, 0x04);
    
    // data_format reg:
    //      select 4-wire mode -> clear bit D6
    //      full_res mode -> set bit D3
    //      justify bit -> left-justified (MSB) mode (1), right-justified (0)
    //      range (bits D1 & D0) specify the range -- 2g, 4g, 8g, or 16g (4g selected)
    // => [self_test, spi, int_invert, 0, full_res, justify, range<1:0>]
    //unsigned char reg_dataFormat = 0x0D;
    _ACCEL_writeToRegister(_ADDR_DATA_FORMAT, 0x0D);

    // select rate_bits in bw_rate to set output data rate
    // => default value is 0x0A which translates to 100 Hz, let's keep it at that atm
    
    // return 0 if incorrect device id
    if (_ACCEL_getDeviceID() != DEVID) 
        return 0;
    
    return 1;
}

/* get zenith angle value */
int getCurrentZenith(void) {
    // TODO
    return 0;
}
