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
    
    int angles[100] = {0};
    while (1) {
        /*
        int angle = MAG_Angle();
        char dataStr[20];
        sprintf(dataStr, "Angle: %d", angle);
        UART_send_str(dataStr);
        __delay_ms(1000);
        */
        LATDbits.LATD3 = 1;
        for (int i = 0; i < 100; i++) {
            angles[i] = MAG_Angle();
            __delay_ms(100);
        }
        LATDbits.LATD3 = 0;
        
        // 5 min delay
        for (int i = 0; i < 300; i++) {
            __delay_ms(1000);
        }
        
        // signal UART transmission
        LATDbits.LATD2 = 1;
        char str[20];
        for (int i = 0; i < 100; i++) {
            memset(str, 0, 20);
            sprintf(str, "Angle %d: %d", i, angles[i]);
            UART_send_str(str);
        }
    }
    
    return 0;
}