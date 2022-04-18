/**
 * @file    main.c
 * @author  Ali Choudhry, Carter Bordeleau, Mahmud Jumaev, Mustafa Siddiqui.
 * @brief   Main logic for the solar tracking system.
 * @date    04/06/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "init.h"
#include "spi.h"
#include "accel.h"
#include "mag.h"
#include "uart.h"
//-//
#include <xc.h>
#include <stdio.h>  // sprintf()
#include <string.h> // memset()
    
#define _XTAL_FREQ 8000000  // 8 MHz

int main(void) {
    // set clock freq to 8 MHz
    OSCCON = 0x72;
    
    // set all pins as digital output
    initPins();
    
    // turn on LEDs to indicate start of init process
    LATDbits.LATD2 = 1;
    LATDbits.LATD3 = 1;
    __delay_ms(1000);

    // initialize UART module
    UART_RX_Init();
    UART_send_str("UART initialized...");
    __delay_ms(1000);

    // initialize PIC18 as master for SPI
    initSPI();
    UART_send_str("SPI initialized...");
    __delay_ms(1000);
    
    // initialize accelerometer for communication
    int status = Mag_Initialize();
    UART_send_str("MAG initialized...");
    __delay_ms(1000);
    
    // test spi communication
    char str[20];
    sprintf(str, "DEVICE ID: %x", MAG_Read(WHO_AM_I));
    UART_send_str(str);
    __delay_ms(1000);
    
    //char str[20];
    //sprintf(str, "STATUS_REG: %x", MAG_Read(STATUS_REG));
    //UART_send_str(str);
    //__delay_ms(1000);
    
    memset(str, 0, sizeof(str));
    sprintf(str, "INT_SOURCE_REG: %x", MAG_Read(INT_SOURCE_REG));
    UART_send_str(str);
    __delay_ms(1000);
    
    memset(str, 0, sizeof(str));
    sprintf(str, "OUTX_L_REG: %x", MAG_Read(OUTX_L_REG));
    UART_send_str(str);
    __delay_ms(1000);
    
    memset(str, 0, sizeof(str));
    sprintf(str, "OUTX_H_REG: %x", MAG_Read(OUTX_H_REG));
    UART_send_str(str);
    __delay_ms(1000);
    
    // turn off LEDs to indicate end of init process
    LATDbits.LATD2 = 0;
    LATDbits.LATD3 = 0;
    _delay(10000);
    
    if (status) {
        UART_send_str("Device ID correct!");
        LATDbits.LATD2 = 1;
        __delay_ms(1000);
        LATDbits.LATD2 = 0;
        __delay_ms(1000);
    }
    
    int sensorData[6] = {0};
    while (1) {
        MAG_Data();
        char dataStr[20];
        sprintf(dataStr, "[%x, %x, %x, %x, %x, %x]", sensorData[0], sensorData[1], sensorData[2], sensorData[3], sensorData[4], sensorData[5]);
        UART_send_str(dataStr);
        __delay_ms(1000);
    }

    return 0;
}