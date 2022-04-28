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
#include "../inc/motor.h"
//-//
#include <xc.h>
#include <stdio.h>  // sprintf()
#include <string.h> // memset()
#include <stdlib.h>
    
#define _XTAL_FREQ 8000000  // 8 MHz
#define MAX_CYCLES 20
#define ALLOWED_ERROR 2
#define UP_POWER
#define ERROR_LIGHT LATDbits.LATD3

int move_vertical_to_angle(int angle);
void error(void);
void variableDelay(int num);

int main(void) {
    // set clock freq to 8 MHz
    OSCCON = 0x72;
    
    // set all pins as digital output
    initPins();
    
    // turn on LEDs to indicate start of init process
    ERROR_LIGHT = 1;
    __delay_ms(1000);

    // initialize UART module
    UART_RX_Init();
    UART_send_str("UART initialized...");
    __delay_ms(1000);

    // initialize PIC18 as master for SPI
    initSPI();
    UART_send_str("SPI initialized...");
    __delay_ms(1000);
    
    if (!initAccel()) {
        error();
    }
    UART_send_str("Accel initialized...");
    
    //init motors
    pwm_Init();
    
    // turn off LEDs to indicate end of init process
    ERROR_LIGHT = 0;
    __delay_ms(1000);
    
    if (move_vertical_to_angle(0)){
        error();
    }
    
    char str[20];
    int angle;
    while (1) {
        angle = getCurrentZenith();
        sprintf(str, "Angle: %d", angle);
        UART_send_str(str);
        __delay_ms(10);
    }

    return 0;
}


int move_vertical_to_angle(int target_angle) {
   
    // validate input angle
    if (target_angle < 0 || target_angle > 80) {
        return 1;
    }
    
    int current_angle; 
    int speed = 0; 
    int time = 50;
    
    int cycles = 0; //clockwise moves down, counterclockwise moves up
    int last_error = 0;
    int change_in_error = 0;
    int error = 0;
    int stall_power = 0;
    int direction;
    
    do {
        __delay_ms(200);
        
        // measure current angle
        current_angle = getCurrentZenith();
        
        // validate the current angle
        char str[10];
        sprintf(str, "CA:%d", current_angle);
        UART_send_str(str);
        
        if (current_angle < -90 || current_angle > 90) {
            return 1;
        
        }
        
        // calculate error
        error = target_angle - current_angle;
        sprintf(str, "e:%d", error);
        UART_send_str(str);
        
        // if within allowed angle stop
        if (abs(error) < ALLOWED_ERROR) {
            UART_send_str("DONE");
            return 0;
        }
                
        // calculate the change in error
        if (last_error != 0) {
            change_in_error = error - last_error;
            
            if (change_in_error == 0) { // if we haven't moved
                stall_power += 10; // increase power
            } else {
                stall_power = 0;
            }
        }
       
        /* speed formula */
        // check if we are moving up or down, adjust formula as necessary
        if (abs(target_angle)> abs(current_angle)) { //down
            speed = 20;
            //time = error*2; //want to do something like this to adjust the time
        } else { //up
            speed = 5*abs(current_angle) + stall_power;
            //time= error*2;
        }
        
        // check that speed is within correct range
        if (speed < 0) {
            speed = 0;
        } else if (speed > 500) {  
            speed = 500;
        }
        
        // figure what direction to move
        if (error > 0) { //move clockwise

            direction = CLOCKWISE;
            UART_send_str("CLOCKWISE");
            
        } else { //move counterclockwise
            
            direction = COUNTER_CLOCKWISE;
            UART_send_str("COUNTERCLOCKWISE");
        }
        
        sprintf(str, "s:%d", speed);
        UART_send_str(str);
        //move the motor
        moveMotor(speed, direction, VERTICAL);
        //moveVerticalMotor(speed, direction);
        variableDelay(time);
        //stopVerticalMotor();
        stopMotor(VERTICAL);    
        
        
        
        last_error = error;
        cycles++;
        
        
    } while (cycles < MAX_CYCLES);
    
    
    return 0;
    
}

void error() {
    // keep on flashing error LED until users shuts power
    while(1) {
        ERROR_LIGHT = 1;
        __delay_ms(500);
        ERROR_LIGHT = 0;
        __delay_ms(500);
    }
}

void variableDelay(int num) {
    // delay for num ms
    for (int i = 0; i < num; i++) {
        __delay__ms(1);
    }
}
