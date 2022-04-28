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
#include "lcd_slave.h"
//-//
#include <xc.h>
#include <stdio.h>  // sprintf()
#include <string.h>
#include <stdlib.h> // memset()
#include <math.h>
#include <string.h>
#include <stdint.h> // int16_t
    
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
    
    int16_t sensorDataMag[4] = {0};
    while (1) {
        MAG_Data(sensorDataMag);
        LATDbits.LATD3 = 1;
        
        char dataStr2[20];
        int length = sprintf(dataStr2, "%d\n", sensorDataMag[3]);
        
        if (LCD_Write(dataStr2, length) < 0) {
            UART_send_str("T_Err [to_PIC32]\n");
        }
        UART_send_str(dataStr2);
        
        __delay_ms(1000);
        LATDbits.LATD3 = 0;
        __delay_ms(1000);
    }//
      
    return 0;
}