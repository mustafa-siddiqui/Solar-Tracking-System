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
#include "../inc/mag.h"
#include "../inc/uart.h"
//-//
#include <xc.h>
#include <stdio.h>  // sprintf()
#include <string.h> // memset()
#include <stdlib.h>
#include <math.h>
    
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
    
    // turn off LEDs to indicate end of init process
    LATDbits.LATD2 = 0;
    LATDbits.LATD3 = 0;
    _delay(1000);
    
    if (status) {
        UART_send_str("Device ID correct!");
        LATDbits.LATD2 = 1;
        __delay_ms(1000);
        LATDbits.LATD2 = 0;
        __delay_ms(1000);
    }
    
    char str[20];
    sprintf(str, "CFG_REG_A: %x", MAG_Read(CFG_REG_A));
    UART_send_str(str);
    __delay_ms(1000);
    
    memset(str, 0, sizeof(str));
    sprintf(str, "CFG_REG_C: %x", MAG_Read(CFG_REG_C));
    UART_send_str(str);
    __delay_ms(1000);
    
    int sensorData[4] = {0};
    while (1) {
        MAG_Data(sensorData);
        char dataStr[20];
        sprintf(dataStr, "[%d, %d, %d, %d]", sensorData[0], sensorData[1], sensorData[2], sensorData[3]);
        UART_send_str(dataStr);
        __delay_ms(1000);
    }

    return 0;
}