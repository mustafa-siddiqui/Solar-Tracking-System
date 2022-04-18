/**
 * @file    accel.h
 * @author  Mustafa Siddiqui, Ali Choudhry
 * @brief   Header file for magnetometer related functions
 *          needed for calculating the azimuth angle which is
 *          then sent to the motor control module.
 *          => Functions beginning with a '_' (underscore) 
 *             are essentially helper functions not to be
 *             used outside this module <=
 * @date    04/12/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _MAG_H_
#define _MAG_H_

/* Register Maps */
//Hard Iron Registers (Both Read and Write)
#define OFFSET_X_REG_L 0x45     //Compensate for environmental effects
#define OFFSET_X_REG_H 0x46     //Act on magnetic output data
#define OFFSET_Y_REG_L 0x47
#define OFFSET_Y_REG_H 0x48
#define OFFSET_Z_REG_L 0x49
#define OFFSET_Z_REG_H 0x4A

//Device ID Register and Status (Read Only)
#define WHO_AM_I 0x4F   //ID register to indentify device
#define STATUS_REG 0x67 //used to indicate device status

//Configuration Registers (Both Read and Write)
#define CFG_REG_A 0x60  //Configure output data rate and measurement configuration
#define CFG_REG_B 0x61 //Offset cancellation and Low-Pass Filter
#define CFG_REG_C 0x62  //Set to 4-Wire SPI

//Interrupt Configuration Registers (All Read and Write, except Source)
#define INT_CRTL_REG 0x063  //Enable and configure interrupt recognition
#define INT_SOURCE_REG 0x64 //Read only, When interrupt latched is selected, reading this register resets all the bits in this register.
#define INT_THS_L_REG 0x65  //Least significant bits of the threshold value chosen for the interrupt
#define INT_THS_H_REG 0x66  //Meast significant bits of the threshold value chosen for the interrupt

//Output Registers (Read Only)
#define OUTX_L_REG 0x68     //Raw magnetic data output accross all
#define OUTX_H_REG 0x69     //of these registers
#define OUTY_L_REG 0x6A     //
#define OUTY_H_REG 0x6B     //
#define OUTZ_L_REG 0x6C     //
#define OUTZ_H_REG 0x6D     //

//Temperature Sensor Registers (Read Only)
#define TEMP_OUT_L_REG 0x6E     //Temparature Sensor Data
#define TEMP_OUT_H_REG 0x6F

#define MAG_ID (0x00) //Will be used to store device ID of magnetometer

/* Utility Macros to set/clear individual bits in a register */
#define SET(reg, bitNum)   (reg |= (1 << bitNum))       //reg = reg | (1 << bitNum)
#define CLEAR(reg, bitNum) (reg &= ~(1 << bitNum))

unsigned char Create_MagData(int RW, unsigned char address);
signed char MAG_Write(unsigned char address, unsigned char data_transmit);
unsigned char MAG_Read(unsigned char address);
unsigned char Get_MAG_ID(void);
int Mag_Initialize(void);
int MAG_Data(void);

#endif /* _MAG_H_ */