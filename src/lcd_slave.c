/**
 * @file    mag.c
 * @author  Mustafa Siddiqui
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

/* write to register on Magnetometer */
signed char LCD_Write(char* data_transmit, int length) {
    // First byte is R/W bit, and address we write to
    // Second byte is data to be written
    //unsigned char RW_Address = Create_MagData(0, address);
    
    /* CHANGE it to MAGNETOMETER */
    // start transmission by pulling ~CS low
    _SPI_selectSlave(ACCELEROMETER);
    
    //signed char status1 = _SPI_write(RW_Address);
    int i = 0;
    while (length >= 0) {
        int status = _SPI_write(data_transmit[i]);
        
        // write not successful; collision occurred somewhere
        if (status != 0) {
            // end transmission by setting ~CS high
            _SPI_unselectSlave(ACCELEROMETER);
            return -1;
        }
        
        i++;
        length--;
    }
    
    // end transmission by setting ~CS high
    _SPI_unselectSlave(ACCELEROMETER);
    
    return 0;
}