/**
 * @file    main.c
 * @author  Ali Choudhry, Carter Bordeleau, Mahmud Jumaev, Mustafa Siddiqui.
 * @brief   Main logic for the solar tracking system.
 * @date    04/06/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../inc/init.h"
#include "../inc/spi.h"
#include "../inc/accel.h"
#include "../inc/uart.h"
//-//
#include <xc.h>
#include <stdio.h>  // sprintf()
    
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
    int status = initAccel();
    UART_send_str("ACCEL initialized...");
    __delay_ms(1000);
    
    // test spi communication
    char strInit[20];
    sprintf(strInit, "POWER_CTL: %x", _ACCEL_readFromRegister(_ADDR_POWER_CTL));
    UART_send_str(strInit);
    __delay_ms(1000);
    
    memset(strInit, 0, sizeof(strInit));
    sprintf(strInit, "BW_RATE: %x", _ACCEL_readFromRegister(_ADDR_BW_RATE));
    UART_send_str(strInit);
    __delay_ms(1000);
    
    unsigned char dataFormatReg = _ACCEL_readFromRegister(_ADDR_DATA_FORMAT);
    char str_dataFormat[20];
    sprintf(str_dataFormat, "DATA_FORMAT: %x", dataFormatReg);
    UART_send_str(str_dataFormat);
    __delay_ms(1000);
    
    // turn off LEDs to indicate end of init process
    LATDbits.LATD2 = 0;
    LATDbits.LATD3 = 0;
    _delay(10000);
    
    if (status) {
        UART_send_str("Device ID correct.");
        LATDbits.LATD2 = 1;
        //unsigned char deviceID = _ACCEL_getDeviceID();
        //char str_deviceID[20];
        //sprintf(str_deviceID, "DEVID: %x", deviceID);
        //UART_send_str(str_deviceID);
        __delay_ms(1000);
        LATDbits.LATD2 = 0;
        __delay_ms(1000);
    }
    
    while (1) {
        unsigned char xVal_0 = _ACCEL_readFromRegister(_ADDR_DATA_X0);
        unsigned char xVal_1 = _ACCEL_readFromRegister(_ADDR_DATA_X1);
        char xData1[10];
        char xData2[10];
        sprintf(xData1, "x0 = %x", xVal_0);
        sprintf(xData2, "x1 = %x", xVal_1);
        UART_send_str(xData1);
        __delay_ms(1000);
        UART_send_str(xData2);
    }

    return 0;
}
