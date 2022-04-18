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
#define _SPI_SDI TRISCbits.TRISC4  /* serial data in */
#define _SPI_SDO TRISCbits.TRISC5  /* serial data out */
#define _SPI_SCK TRISCbits.TRISC3  /* serial clock */

#define _SPI_CS1 LATEbits.LATE2  /* chip select 1 */
#define _SPI_CS2 LATDbits.LATD6  /* chip select 2 */

/* macros to choose slave */
#define ACCELEROMETER   1
#define MAGNETOMETER    2

/* in order to use built-in delay functions defined in 'xc.h' */
#define _XTAL_FREQ 8000000

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
void initSPI(void);

/**
 * @brief   Selects slave given that the PIC18 is in master mode by pulling
 *          ~CS low. Should be set at start of transmission for the duration
 *          of the transmission. Must be unselected at the end of the
 *          transmission. => see _SPI_unselectSlave()
 *          => DOES NOT configure slave device <=
 * @param   slave: accelerometer or magnetometer
 * @return  NULL
 */
void _SPI_selectSlave(int slave);

/**
 * @brief   Unselects slave. Should be used at the end of transmission.
 * @param   slave: accelerometer or magnetometer
 * @return  NULL
 */
void _SPI_unselectSlave(int slave);

/**
 * @brief   Transmit 1 byte of data to already selected slave device.
 *          => source: https://openlabpro.com/guide/spi-module-in-pic18f4550/
 * @param   data: 1 byte
 * @return  0 if successful write, -1 if collision occurred
 */
signed char _SPI_write(unsigned char data_out);

/**
 * @brief   Receive 1 byte of data from slave device
 *          => source: https://openlabpro.com/guide/spi-module-in-pic18f4550/
 * @param   NULL
 * @return  byte read
 */
unsigned char _SPI_read(void);

#endif /* _SPI_H_*/
