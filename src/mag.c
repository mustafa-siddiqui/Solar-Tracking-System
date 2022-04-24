/**
 * @file    mag.c
 * @author  Mustafa Siddiqui
 * @brief   Function definitions for magnetometer module.
 * @date    04/12/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "spi.h"
#include "mag.h"
#include "uart.h"
//-//
#include <xc.h>
#include <stdio.h>  // sprintf()
#include <stdlib.h>
#include <math.h>

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
    _SPI_selectSlave(MAGNETOMETER);
    
    signed char status1 = _SPI_write(RW_Address);
    signed char status2 = _SPI_write(data_transmit);

    // end transmission by setting ~CS high
    _SPI_unselectSlave(MAGNETOMETER);
    
    // write not successful; collision occurred somewhere
    if ((status1 != 0) || (status2 != 0)) {
        return -1;
    }
    return 0;
}

/* read from register on magnetometer */
//Read/Write bit of 1 is to read
unsigned char MAG_Read(unsigned char address) {
    // to store received data
    unsigned char recieved_data = 0x00;

    // first byte consists of R/W and address of register to read
    unsigned char RW_Address = Create_MagData(1, address);
    
    /* Change it to MAGNETOMETER */
    // start transmission by pulling ~CS low
    _SPI_selectSlave(MAGNETOMETER);
    
    // write on MOSI line & receive on MISO line
    _SPI_write(RW_Address);
    recieved_data = _SPI_read();
    
    // end transmission by setting ~CS high
    _SPI_unselectSlave(MAGNETOMETER); 
    
    return recieved_data;
}

/* get device ID */
unsigned char Get_MAG_ID(void) {
    return MAG_Read(WHO_AM_I);
}

void MAG_Data(int* sensorData) {
    // TODO
    //OUT Registers provide us with Magnetometer raw data
    unsigned char X1 = MAG_Read(OUTX_L_REG);    //L--> Low Bits
    unsigned char X2 = MAG_Read(OUTX_H_REG);    //H--> High Bits
    unsigned char Y1 = MAG_Read(OUTY_L_REG);
    unsigned char Y2 = MAG_Read(OUTY_H_REG);
    unsigned char Z1 = MAG_Read(OUTZ_L_REG);
    unsigned char Z2 = MAG_Read(OUTZ_H_REG);
    
    // combine high and low values into one number
    // bits represent signed 2's complement format
    float x = (X2 << 8) | X1;
    float y = (Y2 << 8) | Y1;
    float z = (Z2 << 8) | Z1;
    // populate var
    memset(sensorData, 0, 4);
    sensorData[0] = x;
    sensorData[1] = y;
    sensorData[2] = z;
    
    //__delay_ms(1000);
    float Angle = 0;
    float divider = (y/x); 
    Angle = atan(divider);
    Angle = Angle*(180/PI);// + DECLINATION;
    
    if (Angle > 360){
        Angle = Angle - 360;
    }
    else if (Angle < 0){
        Angle = Angle + 360;
    }
    //__delay_ms(1000);
    sensorData[3] = Angle;
}
/*
int MAG_Angle(void) {
    int sensorData[4] = {0};
    MAG_Data(sensorData);
    
    //Convert data into angle form
    double Angle;
    Angle = atan2( (double)sensorData[0], (double)sensorData[1] );
    Angle = Angle*(180/PI);
    
    if (Angle > 360){
        Angle = Angle - 360;
    }
    else if (Angle < 0){
        Angle = Angle + 360;
    }
    return Angle;
    /*
    If D is greater than 337.25 degrees or less than 22.5 degrees --> North
    If D is between 292.5 degrees and 337.25 degrees --> North-West
    If D is between 247.5 degrees and 292.5 degrees --> West
    If D is between 202.5 degrees and 247.5 degrees --> South-West
    If D is between 157.5 degrees and 202.5 degrees --> South
    If D is between 112.5 degrees and 157.5 degrees --> South-East
    If D is between 67.5 degrees and 112.5 degrees --> East
    If D is between 0 degrees and 67.5 degrees --> North-East
    */
//}

/* initialize magnetometer module */
int Mag_Initialize(void) {
    
    MAG_Write(CFG_REG_A, 0x00);
    //Set magnetometer to enable temperature compensation, normal mode,
    //Don't reset registers, high-resolution mode, 100Hz output, and
    //continuous measurement mode
    
    MAG_Write(CFG_REG_C, 0x36);
    //Default interrupts, inhibit I2C (SPI only), avoid reading incorrect data,
    //Don't invert high and low bits of data, 4-Wire SPI mode, enable self-test,
    //Default data-ready
    
    // return 0 if incorrect device id
    if (Get_MAG_ID() != WHO_AM_I_VAL) 
        return 0;
    
    return 1;
}