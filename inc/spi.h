/**
 * @file    spi.h
 * @author  Mustafa Siddiqui
 * @brief   Header file for SPI communication on PIC18 MCU.
 * @date    04/14/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _SPI_H_
#define _SPI_H_

/* Serial Port Pins */
#define SDI TRISCbits.TRISC4  /* serial data in */
#define SDO TRISCbits.TRISC5  /* serial data out */
#define SCK TRISCbits.TRISC3  /* serial clock */

#define CS1 LATEbits.LATE2  /* chip select 1 */
#define CS2 LATDbits.LATD6  /* chip select 2 */

/* macros to choose slave */
#define ACCELEROMETER   1
#define MAGNETOMETER    2

/**
 * @brief   Enable SPI IO i.e. basically enable the serial port on the PIC18.
 * @param   NULL
 * @return  NULL
 */
void _SPI_enableIO(void);

/**
 * @brief   Disable serial port function.
 * @param   NULL
 * @return  NULL
 */
void _SPI_disableIO(void);

/**
 * @brief   Initialize the SPI module on the PIC18. 
 *          Master mode, clock polarity, data input sample phase, clock edge,
 *          and clock rate are set.
 * @param   NULL
 * @return  NULL
 */
void _SPI_init(void);

/**
 * @brief   Selects slave given that the PIC18 is in master mode.
 *          => DOES NOT configure slave device <=
 * @param   slave: accelerometer or magnetometer
 * @return  NULL
 */
void _SPI_selectSlave(int slave);

#endif /* _SPI_H_*/
