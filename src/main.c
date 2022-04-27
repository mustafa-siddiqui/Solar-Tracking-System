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

#define WHITE_LED LATDbits.LATD2
#define GREEN_LED LATDbits.LATD3

int main(void) {
    // set clock freq to 8 MHz
    OSCCON = 0x72;
    
    // set all pins as digital output
    initPins();
    
    // turn on LEDs to indicate start of init process
    WHITE_LED = 1;
    GREEN_LED = 1;

    // initialize UART module
    UART_RX_Init();
    UART_send_str("UART initialized...");
    __delay_ms(500);

    // initialize PIC18 as master for SPI
    initSPI();
    UART_send_str("SPI initialized...");
    __delay_ms(500);
    
    // initialize accelerometer for communication
    if (Mag_Initialize()) {
        UART_send_str("MAG initialized...");
        __delay_ms(500);
    }
    
    // turn off LEDs to indicate end of init process
    WHITE_LED = 0;
    GREEN_LED = 0;
    _delay(1000);
    
#ifdef LOG_DATA
    int angles[100] = {0};
    while (1) {
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
#endif /* LOG_DATA */

    while (1) {
        GREEN_LED = 1;
        int angle = MAG_Angle();
        char angleStr[10];
        sprintf(angleStr, "Angle: %d", angle);
        UART_send_str(angleStr);
        __delay_ms(10);
        
        // delay before next reading & calc
        __delay_ms(1000);
        
        // signal end of calc and next iteration of loop
        GREEN_LED = 0;
        __delay_ms(1000);
    }

    return 0;
}