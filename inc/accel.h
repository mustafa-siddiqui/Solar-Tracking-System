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

/* Register Maps */
#define DEVID       (*((volatile unsigned char*) 0x00))  /* device ID => stores fixed value of 0xE5 */
#define BW_RATE     (*((volatile unsigned char*) 0x2C))  /* data rate and power mode control */
#define POWER_CTL   (*((volatile unsigned char*) 0x2D))  /* power-saving features control */
#define DATA_FORMAT (*((volatile unsigned char*) 0x31))  /* data format control */
#define DATA_X0     (*((volatile unsigned char*) 0x32))  /* x-axis data 0 */
#define DATA_X1     (*((volatile unsigned char*) 0x33))  /* x-axis data 1 */
#define DATA_Y0     (*((volatile unsigned char*) 0x34))  /* y-axis data 0 */
#define DATA_Y1     (*((volatile unsigned char*) 0x35))  /* y-axis data 1 */
#define DATA_Z0     (*((volatile unsigned char*) 0x36))  /* z-axis data 0 */
#define DATA_Z1     (*((volatile unsigned char*) 0x37))  /* z-axis data 1 */

/* Utility Macros to set/clear indvidual bits in a register */
#define SET(reg, bitNum)   (reg |= (1 << bitNum))
#define CLEAR(reg, bitNum) (reg &= ~(1 << bitNum))

/**
 * @brief   Get the device ID of the accelerometer sensor.
 *          Should receive a fixed value of 0xE5 or 229
 * @param   NULL
 * @return  device ID of sensor
 */
unsigned char _getDeviceID(void);

/**
 * @brief   Enable measurement mode on the accelerometer to
 *          enable sensor functions and read values.
 * @param   NULL
 * @return  NULL
 */
void _enableMeasureMode(void);

/**
 * @brief   Enable standby mode to disable measurement mode &
 *          prevent any measurements from being made. Current
 *          consumption is reduced to 0.1 µA.
 * @param   NULL
 * @return  NULL
 */
void _enableStandbyMode(void);

/**
 * @brief   Get current x, y, and z axis readings from the
 *          accelerometer.
 * @param   NULL
 * @return  an integer array of length = 3
 *          => format: [x, y, z]
 */
int* _getCurrentReading(void);

/**
 * @brief   Intialize accelerometer module to start taking
 *          measurement readings.
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
