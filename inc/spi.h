/**
 * @file    spi.h
 * @author  Ali Choudhry & Mustafa Siddiqui
 * @brief   Header file for SPI communication on PIC18 MCU.
 * @date    04/10/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _SPI_H_
#define _SPI_H_

#define CS  LATA5 /* double check if CS is LATA5, I think it is RE2; A5 is SS (for slave mode operations) */
#define LED LATD
/* arbitrary pins selected for now */
#define SS1 LATD5
#define SS2 LATD6

/* slave select macros */
#define ACCELEROMETER   1
#define MAGNETOMETER    2

/**
 * @brief 
 * @param
 * @return
 */
void SPI_Init_Master(void);

/**
 * @brief 
 * @param   int slave: select which slave to communicate with.
 * @return
 */
void SPI_Init_Slave(int slave);

/**
 * @brief 
 * @param
 * @return
 */
void SPI_Write(unsigned char x);

/**
 * @brief 
 * @param
 * @return
 */
unsigned char SPI_Read(void);

/* => replace this to use in-built _delay() function <=
      can make a function like delay_1ms() if specific delay needed
*/
/**
 * @brief 
 * @param
 * @return
 */
void MSdelay(unsigned int val);

#endif /* _SPI_H_*/
