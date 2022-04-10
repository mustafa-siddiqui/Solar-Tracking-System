/**
 * @file    spi.h
 * @author  Ali Choudhry
 * @brief   Header file for SPI communication on PIC18 MCU.
 * @date    04/10/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _SPI_H_
#define _SPI_H_

#define CS LATA5
#define LED LATD

/**
 * @brief 
 * @param
 * @return
 */
void SPI_Init_Master(void);

/**
 * @brief 
 * @param
 * @return
 */
void SPI_Init_Slave(void);

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
