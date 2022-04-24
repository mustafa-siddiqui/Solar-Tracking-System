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
#include "mag.h"
#include "uart.h"
#include "accel.h"
#include "azimuth.h"
#include "lcd.h"
//-//
#include <xc.h>
#include <stdio.h>  // sprintf()
#include <string.h>
#include <stdlib.h> // memset()
#include <math.h>
#include <string.h>
    
#define _XTAL_FREQ 8000000  // 8 MHz

int main(void) {
    // set clock freq to 8 MHz
    OSCCON = 0x72;
    
    // set all pins as digital output
    initPins();
    
    // turn on LEDs to indicate start of init process
    LATDbits.LATD2 = 1;
    LATDbits.LATD3 = 1;
    __delay_ms(1500);
    LATDbits.LATD2 = 0;
    LATDbits.LATD3 = 0;
/*
    // initialize UART module
    UART_RX_Init();
    UART_send_str("UART initialized...");
    __delay_ms(1000);

    // initialize PIC18 as master for SPI
    initSPI();
    UART_send_str("SPI initialized...");
    __delay_ms(1000);
    
    // initialize accelerometer for communication
    int status1 = Mag_Initialize();
    UART_send_str("MAG initialized...");
    __delay_ms(1000);
    
    int status2 = initAccel();
    UART_send_str("Accel initialized...");
    __delay_ms(1000);
    
    // turn off LEDs to indicate end of init process
    LATDbits.LATD2 = 0;
    LATDbits.LATD3 = 0;
    _delay(1000);
    
    if (status1) {
        UART_send_str("Mag ID correct!");
        LATDbits.LATD2 = 1;
        __delay_ms(1000);
        LATDbits.LATD2 = 0;
        __delay_ms(1000);
    }
    if (status2) {
        UART_send_str("Accel ID correct!");
        LATDbits.LATD2 = 1;
        __delay_ms(1000);
        LATDbits.LATD2 = 0;
        __delay_ms(1000);
    }
    
    */
        char tt[] = "hello"; 
        lcd_display_driver_initialize();                   /* Initialize 16x2 LCD */
        __delay_ms(3000);
        LATDbits.LATD2 = 1;
        display_driver_use_first_line();
        lcd_display_driver_write( tt, 5 );
        
        while(1);
    
    
    /*
    int sensorDataMag[4] = {0};
    int sensorDataAccel[3] = {0};
    while (1) {
        MAG_Data(sensorDataMag);
        char dataStr[20];
        sprintf(dataStr, "Mag Data: [%d, %d, %d, %d]", sensorDataMag[0], sensorDataMag[1], sensorDataMag[2], sensorDataMag[3]);
        UART_send_str(dataStr);
        
        _ACCEL_getCurrentReading(sensorDataAccel);
        char dataStr2[20];
        sprintf(dataStr2, "Accel Data: [%d, %d, %d]", sensorDataAccel[0], sensorDataAccel[1], sensorDataAccel[2]);
        UART_send_str(dataStr2);
        
        //int peepee = GetAzimuth();
        //char dataStr[20];
        //sprintf(dataStr, "[%d]", peepee);
        //UART_send_str(dataStr);
        __delay_ms(1000);
    }//
     */ 
    return 0;
}