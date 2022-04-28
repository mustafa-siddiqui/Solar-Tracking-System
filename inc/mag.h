/**
 * @file    mag.h
 * @author  Ali Choudhry, Mustafa Siddiqui
 * @brief   Header file for magnetometer related functions
 *          needed for calculating the azimuth angle which is
 *          then sent to the logic control module.
 * @date    04/12/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _MAG_H_
#define _MAG_H_

#include <stdint.h> // int16_t

/* Register Maps */
// Hard Iron Registers (Both Read and Write)
#define OFFSET_X_REG_L 0x45     // Compensate for environmental effects
#define OFFSET_X_REG_H 0x46     // Act on magnetic output data
#define OFFSET_Y_REG_L 0x47
#define OFFSET_Y_REG_H 0x48
#define OFFSET_Z_REG_L 0x49
#define OFFSET_Z_REG_H 0x4A

// Device ID Register and Status (Read Only)
#define WHO_AM_I        0x4F    // ID register to indentify device
#define WHO_AM_I_VAL    0x40    // Content of WHO_AM_I register
#define STATUS_REG      0x67    // used to indicate device status

// Configuration Registers (Both Read and Write)
#define CFG_REG_A 0x60      // Configure output data rate and measurement configuration
#define CFG_REG_B 0x61      // Offset cancellation and Low-Pass Filter
#define CFG_REG_C 0x62      // Set to 4-Wire SPI

// Output Registers (Read Only)
// Raw magnetic data output across all of these registers
#define OUTX_L_REG 0x68
#define OUTX_H_REG 0x69
#define OUTY_L_REG 0x6A
#define OUTY_H_REG 0x6B
#define OUTZ_L_REG 0x6C
#define OUTZ_H_REG 0x6D

#define DECLINATION -11.25 //magnetic declination of rochester

// Utility Macros to set/clear individual bits in a register
#define SET(reg, bitNum)   (reg |= (1 << bitNum))
#define CLEAR(reg, bitNum) (reg &= ~(1 << bitNum))

// Number of axis readings the sensor reads
#define NUM_AXIS        3 

// Number of readings to average
#define NUM_READINGS    10

// Offsets -- hardcoded
int16_t xAxisOffset = 380;
int16_t yAxisOffset = 160;

// Calibrate to 0
int16_t initialOff = 0;

// create data packet to write to magnetometer
// RW: read/write
// address: address of register
unsigned char Create_MagData(int RW, unsigned char address);

// write to register
// address: address of register
// data_transmit: data to write to register
signed char MAG_Write(unsigned char address, unsigned char data_transmit);

// read from register
// address: address of register to read from
unsigned char MAG_Read(unsigned char address);

// read content stored in WHO_AM_I register
unsigned char Get_MAG_ID(void);

// intiliaze magnetometer: CFG_REG_A and CFG_REG_C
int Mag_Initialize(void);

// get current sensor reading [x,y,z]
void MAG_Data(int16_t* sensorData);

// get average of NUM_READINGS sensor readings
void MAG_AvgData(int32_t* avgData);

// calculate azimuth angle in degrees
// angle = [0, 360)
int MAG_Angle(void);

#endif /* _MAG_H_ */
