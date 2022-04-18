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
    int status = initAccel();
    UART_send_str("ACCEL initialized...");
    __delay_ms(1000);
    
    // test spi communication
    char str[20];
    sprintf(str, "POWER_CTL: %x", _ACCEL_readFromRegister(_ADDR_POWER_CTL));
    UART_send_str(str);
    __delay_ms(1000);
    
    memset(str, 0, sizeof(str));
    sprintf(str, "BW_RATE: %x", _ACCEL_readFromRegister(_ADDR_BW_RATE));
    UART_send_str(str);
    __delay_ms(1000);
    
    memset(str, 0, sizeof(str));
    sprintf(str, "DATA_FORMAT: %x", _ACCEL_readFromRegister(_ADDR_DATA_FORMAT));
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
    
    int sensorData[3] = {0};
    while (1) {
        _ACCEL_getCurrentReading(sensorData);
        char dataStr[20];
        sprintf(dataStr, "[%x, %x, %x]", sensorData[0], sensorData[1], sensorData[2]);
        UART_send_str(dataStr);
        __delay_ms(1000);
    }

    return 0;
}
