/**
 * @file    mag.c
 * @author  Mustafa Siddiqui
 * @brief   Function definitions for accelerometer module.
 * @date    04/12/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */



#include "../inc/spi.h"
#include "../inc/mag.h"
#include "../inc/uart.h"
//-//
#include <xc.h>
#include <stdio.h>  // sprintf()

//Function that creates byte of data to be transmitted from PIC to Magnetometer
//First input is Read/Write Bit
//Second input is register address of Magnetometer
//Third input is multiple byte bit
//SPI Write Protocol:
//Bit 0 --> Read/Write Bit  --> 0 is write
//Bits 1-7 --> Address of register in Magnetometer
//Bits 8-15 --> Data to be transmitted, MSb first
//Multiple Bytes of data may be sent by adding a second byte to the data transmit
unsigned char Create_MagData(int RW, unsigned char address){
    unsigned char Data_Transmit = 0x0;
    Data_Transmit |= address;

    // set read/write bit
    if (RW == 1)
        SET(Data_Transmit, 7);
    else
        CLEAR(Data_Transmit, 7);

    return Data_Transmit;
}

/* write to register on Magnetometer */
signed char MAG_Write(unsigned char address, unsigned char data_transmit) {
    // First byte is R/W bit, and address we write to
    // Second byte is data to be written
    unsigned char RW_Address = Create_MagData(0, address);
    
    /* CHANGE it to MAGNETOMETER */
    // start transmission by pulling ~CS low
    _SPI_selectSlave(ACCELEROMETER);
    
    signed char status1 = _SPI_write(RW_Address);
    signed char status2 = _SPI_write(data_transmit);

    // end transmission by setting ~CS high
    _SPI_unselectSlave(ACCELEROMETER);
    
    // write not successful; collision occurred somewhere
    if ((status1 != 0) || (status2 != 0)) {
        return -1;
    }
    return 0;
}

/* read from register on accelerometer */
//Read/Write bit of 1 is to read
unsigned char MAG_Read(unsigned char address) {
    // to store received data
    unsigned char recieved_data = 0x00;

    // first byte consists of R/W and address of register to read
    unsigned char RW_Address = Create_MagData(1, address);
    
    /* Change it to MAGNETOMETER */
    // start transmission by pulling ~CS low
    _SPI_selectSlave(ACCELEROMETER);
    
    // write on MOSI line & receive on MISO line
    _SPI_write(RW_Address);
    recieved_data = _SPI_read();
    
    // end transmission by setting ~CS high
    _SPI_unselectSlave(ACCELEROMETER); 
    
    return recieved_data;
}

/* get device ID */
unsigned char Get_MAG_ID(void) {
    return MAG_Read(MAG_ID);
}

void MAG_Data(int* sensorData) {
    // TODO
    //OUT Registers provide us with Magnetometer raw data
    unsigned char X1 = MAG_Read(OUTX_L_REG);
    unsigned char X2 = MAG_Read(OUTX_H_REG);
    unsigned char Y1 = MAG_Read(OUTY_L_REG);
    unsigned char Y2 = MAG_Read(OUTY_H_REG);
    unsigned char Z1 = MAG_Read(OUTZ_L_REG);
    unsigned char Z2 = MAG_Read(OUTZ_H_REG);
    
    // combine high and low values into one number
    // bits represent signed 2's complement format
    int x = (X1 << 8) | X2;
    int y = (Y1 << 8) | Y2;
    int z = (Z1 << 8) | Z2;

    // populate var
    memset(sensorData, 0, 3);
    sensorData[0] = x;
    sensorData[1] = y;
    sensorData[2] = z;
}

/* initialize accelerometer module */
int Mag_Initialize(void) {
    
    MAG_Write(CFG_REG_A, 0x80);
    //Set magnetometer to enable temperature compensation, normal mode,
    //Don't reset registers, high-resolution mode, 100Hz output, and
    //single measurement mode
    
    MAG_Write(CFG_REG_C, 0x36);
    //Default interrupts, inhibit I2C (SPI only), avoid reading incorrect data,
    //Don't invert high and low bits of data, 4-Wire SPI mode, enable self-test,
    //Default data-ready
    
    // return 0 if incorrect device id
    if (Get_MAG_ID() != WHO_AM_I_VAL) 
        return 0;
    
    return 1;
}