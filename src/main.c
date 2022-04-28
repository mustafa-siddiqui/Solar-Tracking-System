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
#include <stdlib.h>
    
#define _XTAL_FREQ 8000000  // 8 MHz
#define MAX_CYCLES 20
#define ALLOWED_ERROR 2

int move_vertical_to_angle(int angle);

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
    
    // turn off LEDs to indicate end of init process
    LATDbits.LATD2 = 0;
    LATDbits.LATD3 = 0;
    __delay_ms(1000);
    
    while (1) {
        ;
    }

    return 0;
}


int move_vertical_to_angle(int target_angle) {
    //validate input angle
    if (target_angle > 0 || target_angle < 80) {
        return 1;
    }
    
    int current_angle = getCurrentZenith();
    
    //validate the current angle
    if (current_angle < -90 || current_angle > 90) {
        return 1;
        
    }
    int speed = 5; //start at 1%
    
    int cycles = 0; //clockwise moves down, counterclockwise moves up
    do {
        int error = target_angle - current_angle;
        
        // if within allowed angle stop
        if (abs(error) > ALLOWED_ERROR) {
            return 0;
        }
        //figure what direction to move
        if (error > 0) { //move down
            
        } else { //move up
            
        }
        
        
        //measure again
        current_angle = getCurrentZenith();
        //validate the current angle
        if (current_angle < -90 || current_angle > 90) {
            return 1;
        
        }
        
        cycles++;
        
        
    } while (cycles < MAX_CYCLES);
    
    
    
    
}