/**
 * @file    mag.c
 * @author  Ali Choudhry
 * @brief   Function definitions for magnetometer module.
 * @date    04/12/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#define DEBUG

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
    unsigned char Data_Transmit = 0x00;
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
    memset(sensorData, 0, NUM_AXIS * sizeof(sensorData[0]));
    sensorData[0] = x;
    sensorData[1] = y;
    sensorData[2] = z;
}

// get average of NUM_READINGS
void MAG_AvgData(int32_t* avgData) {
    memset(avgData, 0, NUM_AXIS * sizeof(avgData[0]));
    int16_t sensorData[NUM_AXIS] = {0};
    
    // O(NUM_READINGS * NUM_AXIS) = constant time
    for (int i = 0; i < NUM_READINGS; i++) {
        MAG_Data(sensorData);
        for (int j = 0; j < NUM_AXIS; j++) {
            avgData[j] += sensorData[j];
        }
    }
    
    // O(NUM_AXIS) = constant time
    for (int c = 0; c < NUM_AXIS; c++) {
        avgData[c] /= NUM_READINGS;
    }
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
    int16_t sensorData[NUM_AXIS] = {0};
    //int32_t sensorData[NUM_AXIS] = {0};
    MAG_Data(sensorData);
    
#ifdef DEBUG
    // send angle data to Raspberry Pi
    char debugStr[20];
    sprintf(debugStr, "[x: %d, y: %d, z: %d]\n", sensorData[0], sensorData[1], sensorData[2]);
    UART_send_str(debugStr);
    __delay_ms(10);
#endif /* DEBUG */

    // calculate angle from x,y,z readings
    // quadrant-aware arctan(y/x)
    // => see: https://arduino.stackexchange.com/questions/18625/converting-three-axis-magnetometer-to-degrees
    float angle = atan2(sensorData[1], sensorData[0]);
    
#ifdef DEBUG
    char rawAngle[20];
    sprintf(rawAngle, "Raw angle: %f\n", angle);
    UART_send_str(rawAngle);
    __delay_ms(100);
#endif /* DEBUG */
    
    // convert from rad to deg
    int angleDegrees = (int)(angle * (180/M_PI));
    
#ifdef DEBUG
    char degStr[20];
    sprintf(degStr, "%d degrees\n", angleDegrees);
    UART_send_str(degStr);
#endif /* DEBUG */
    
    // keep between 0-180
    // => change to keep btw 0-360
    //angleDegrees = (angleDegrees + 180) % 180;
    
    // 360 = 0 degrees
    if (angleDegrees == 360)
        return 0;

    return angleDegrees;  
}

//  TODO:
//  - change ODR to 100Hz and see diff in readings -> no change, maybe a little more accurate
//  - maybe change measurement mode to single reading? -> need to see how it works

/* initialize magnetometer module */
int Mag_Initialize(void) {
    // high-res mode, 10Hz output data rate, continuous measurement mode
    // => turn-on time: ~9.4 ms 
    MAG_Write(CFG_REG_A, 0x00);
    __delay_ms(10);
    
    // SPI mode only, avoid reading incorrect data pin set, 4 wire SPI mode
    // disable self-test, data-ready pin off since continuous mode
    MAG_Write(CFG_REG_C, 0x34);
    
    // return 0 if incorrect device id
    if (Get_MAG_ID() != WHO_AM_I_VAL) 
        return 0;
    
    return 1;
}