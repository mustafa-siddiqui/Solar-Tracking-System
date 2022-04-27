/**
 * @file    accel.h
 * @author  Mustafa Siddiqui
 * @brief   Header file for accelerometer related functions
 *          needed for calculating the zenith angle which is
 *          then sent to the motor control module.
 *          => Functions beginning with a '_' (underscore) 
 *             are essentially helper functions not to be
 *             used outside this module <=
 * @date    04/12/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _ACCEL_H_
#define _ACCEL_H_

#include <stdint.h> // uint8_t, int16_t

/* Register Maps */
#define _ADDR_DEVID       0x00  /* device ID => stores fixed value of 0xE5 */
#define _ADDR_BW_RATE     0x2C  /* data rate and power mode control */
#define _ADDR_POWER_CTL   0x2D  /* power-saving features control */
#define _ADDR_DATA_FORMAT 0x31  /* data format control */
#define _ADDR_DATA_X0     0x32  /* x-axis data 0 */
#define _ADDR_DATA_X1     0x33  /* x-axis data 1 */
#define _ADDR_DATA_Y0     0x34  /* y-axis data 0 */
#define _ADDR_DATA_Y1     0x35  /* y-axis data 1 */
#define _ADDR_DATA_Z0     0x36  /* z-axis data 0 */
#define _ADDR_DATA_Z1     0x37  /* z-axis data 1 */

/* Constant value stored in DEVID register of accelerometer */
#define DEVID   (0xE5)

/* Utility Macros to set/clear indvidual bits in a register */
#define SET(reg, bitNum)   (reg |= (1 << bitNum))
#define CLEAR(reg, bitNum) (reg &= ~(1 << bitNum))

/* Utility macro to square a value */
#define SQUARE(num) (num * num)

/* Number of axis readings given by the sensor */
#define NUM_AXIS        3

/* Number of readings to average when reading sensor data */
/* 0.1 sec worth of data given a 100 Hz data rate */
#define NUM_READINGS    10

/**
 * @brief   Create first data byte that is transmitted over SPI for
 *          accelerometer. byte => [R/W, MB, A5-A0]
 * @param   RW: 1 or 0 => read (1)/write (0) bit 
 * @param   MB: 1 or 0 => multiple byte transmission bit
 * @param   addr: address of register to access 
 * @return  content of the data byte to tranmsit
 */
unsigned char _ACCEL_createDataByte1(int RW, int MB, unsigned char addr);

/**
 * @brief   Write to a specified register on the accelerometer over SPI.
 * @param   addr: address of the register to write to
 * @param   data: data to write in the register
 * @return  0 if successful, -1 if collision occured 
 */
signed char _ACCEL_writeToRegister(unsigned char addr, unsigned char data);

/**
 * @brief   Read from a specified register on the accelerometer over SPI.
 * @param   addr: address of the register to read from
 * @return  content of the specified register 
 */
unsigned char _ACCEL_readFromRegister(unsigned char addr);

/**
 * @brief   Get the device ID of the accelerometer sensor.
 *          Should receive a fixed value of 0xE5 or 229
 * @param   NULL
 * @return  device ID of sensor
 */
unsigned char _ACCEL_getDeviceID(void);

/**
 * @brief   Get current x, y, and z axis readings from the
 *          accelerometer.
 *          =>  pass by reference is used to prevent memory leaks
 *              with issues stemming from allocating memory for 
 *              pointers inside functions and returning those pointers
 * @param   sensorData: pointer to an integer array to hold [x,y,z] values
 *          => better to have it initialized to size = 3
 * @return  NULL
 */
void _ACCEL_getCurrentReading(int16_t *sensorData);

/**
 * @brief   Get data equivalent to average of NUM_READINGS.
 * @param   avgData: pointer to an integer array to hold [x,y,z] values
 * @return  NULL
 */
void _ACCEL_getAvgReading(int *avgData);

/**
 * @brief   Configure accelerometer's SPI module as slave
 *          in line with PIC18's configurations as master.
 *          Enables sensor measurements to be transmitted 
 *          from the accelerometer.
 * @param   NULL
 * @return  1 if successfully initialized, 0 if not
 */
int initAccel(void);

/**
 * @brief   Get current zenith angle by calculating it from
 *          x, y, and z axis accelerometer readings.
 * @param   NULL
 * @return  Integer value for the Zenith angle
 */
int getCurrentZenith(void);

#endif /* _ACCEL_H_ */
