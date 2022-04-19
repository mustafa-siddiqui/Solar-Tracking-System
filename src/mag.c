/**
 * @file    mag.c
 * @author  Mustafa Siddiqui
 * @brief   Function definitions for accelerometer module.
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
    unsigned char X1 = MAG_Read(OUTX_L_REG);    //L--> Least Significant Bit
    unsigned char X2 = MAG_Read(OUTX_H_REG);    //H--> Most Significant Bit
    unsigned char Y1 = MAG_Read(OUTY_L_REG);
    unsigned char Y2 = MAG_Read(OUTY_H_REG);
    unsigned char Z1 = MAG_Read(OUTZ_L_REG);
    unsigned char Z2 = MAG_Read(OUTZ_H_REG);
    
    // combine high and low values into one number
    // bits represent signed 2's complement format
    int x = (X2 << 8) | X1;
    int y = (Y2 << 8) | Y1;
    int z = (Z2 << 8) | Z1;
    
    //Convert data from hexadecimal to decimal
    double XA = strtol(x, NULL, 16);
    double YA = strtol(y, NULL, 16);
    double ZA = strtol(z, NULL, 16);
    
        // populate var
    memset(sensorData, 0, 4);
    sensorData[0] = XA;
    sensorData[1] = YA;
    sensorData[2] = ZA;
    
    //Convert data into angle form
    double Angle;
    Angle = atan2( (double)YA, (double)XA );
    Angle = Angle*(180/PI);
    
    if (Angle > 360){
        Angle = Angle - 360;
    }
    else if (Angle < 0){
        Angle = Angle + 360;
    }
    sensorData[3] = Angle;
    /*
    If D is greater than 337.25 degrees or less than 22.5 degrees ? North
    If D is between 292.5 degrees and 337.25 degrees ? North-West
    If D is between 247.5 degrees and 292.5 degrees ? West
    If D is between 202.5 degrees and 247.5 degrees ? South-West
    If D is between 157.5 degrees and 202.5 degrees ? South
    If D is between 112.5 degrees and 157.5 degrees ? South-East
    If D is between 67.5 degrees and 112.5 degrees ? East
    If D is between 0 degrees and 67.5 degrees ? North-East
     */
    /*
    char Direction;
    if(Angle>337.25 || Angle<22.5){
        Direction = "North";
    }
    else if(Angle>292.5 && Angle<337.25){
        Direction = "NorthWest";
    }
    else if(Angle>247.5 && Angle<292.5){
        Direction = "West";
    }
    else if(Angle>202.5 && Angle<247.5){
        Direction = "SouthWest";
    }
    else if(Angle>157.5 && Angle<202.5){
        Direction = "South";
    }
    else if(Angle>112.5 && Angle<157.5){
        Direction = "SouthEast";
    }
    else if(Angle>67.5 && Angle<112.5){
        Direction = "East";
    }
    else if(Angle>0 && Angle<67.5){
        Direction = "NorthEast";
    }
    return Direction;
     */
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