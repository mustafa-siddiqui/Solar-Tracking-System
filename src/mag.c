/**
 * @file    mag.c
 * @author  Ali Choudhry
 * @brief   Function definitions for magnetometer module.
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
#include <stdlib.h>
#include <math.h>

// Function that creates byte of data to be transmitted from PIC to Magnetometer
// First input is Read/Write Bit
// Second input is register address for magnetometer
// SPI Write Protocol:
// Bit 0 --> Read/Write Bit  --> 0 is write
// Bits 1-7 --> Address of register in magnetometer
// Bits 8-15 --> Data to be transmitted, MSb first
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
// Read/Write bit of 1 is to read
unsigned char MAG_Read(unsigned char address) {
    // to store received data
    unsigned char recieved_data = 0x00;

    // first byte consists of R/W and address of register to read
    unsigned char RW_Address = Create_MagData(1, address);
    
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

/* get x,y,z sensor reading from magnetometer */
void MAG_Data(int16_t* sensorData) {
    //OUT Registers provide us with Magnetometer raw data
    unsigned char X1 = MAG_Read(OUTX_L_REG);    //L--> Low Bits
    unsigned char X2 = MAG_Read(OUTX_H_REG);    //H--> High Bits
    unsigned char Y1 = MAG_Read(OUTY_L_REG);
    unsigned char Y2 = MAG_Read(OUTY_H_REG);
    unsigned char Z1 = MAG_Read(OUTZ_L_REG);
    unsigned char Z2 = MAG_Read(OUTZ_H_REG);
    
    // combine high and low values into one number
    // bits represent signed 2's complement format
    int16_t x = (X2 << 8) | X1;
    int16_t y = (Y2 << 8) | Y1;
    int16_t z = (Z2 << 8) | Z1;

    // populate var
    memset(sensorData, 0, 3);
    sensorData[0] = x;
    sensorData[1] = y;
    sensorData[2] = z;
}

/*
    D = (337, 22]   => N
    D = (22, 67]    => NE
    D = (67, 122]   => E
    D = (112, 157]  => SE
    D = (157, 202]  => S
    D = (202, 247]  => SW
    D = (247, 292]  => W
    D = (292, 337]  => NW
    where D = angle
*/
int MAG_Angle(void) {
    // read [x,y,z] sensor reading
    int16_t sensorData[3] = {0};
    MAG_Data(sensorData);

    // handle corner cases before calc
    if (sensorData[1] == 0 && sensorData[0] < 0) {
        return 180;
    }
    else if (sensorData[1] == 0 && sensorData[0] > 0) {
        return 0;
    }

    // calculate angle from x,y,z readings
    // arctan(y/x)
    // need to use atan2(x, y)
    // => see: https://arduino.stackexchange.com/questions/18625/converting-three-axis-magnetometer-to-degrees
    float angle = atan2((float)sensorData[0], (float)sensorData[1]);

    // determine compass heading
    if (sensorData[1] > 0) {
        // when y > 0
        return 90 - (int)(angle * (180/M_PI));
    } 
    else {
        // when y < 0
        return 270 - (int)(angle * (180/M_PI));
    }

    // shouldn't ever reach this
    return -1;  
}

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