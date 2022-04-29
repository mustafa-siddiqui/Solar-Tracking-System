/**
 * @file    main.c
 * @author  Ali Choudhry, Carter Bordeleau, Mahmud Jumaev, Mustafa Siddiqui.
 * @brief   Main logic for the solar tracking system.
 * @date    04/29/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../inc/init.h"
#include "../inc/uart.h"
#include "../inc/spi.h"
#include "../inc/accel.h"
#include "../inc/mag.h"
#include "../inc/gps.h"
#include "../inc/motor.h"
//-//
#include <xc.h>
#include <stdio.h>  // sprintf()
#include <string.h> // memset()
#include <stdlib.h> // abs()
    
#define _XTAL_FREQ              8000000  // 8 MHz
#define ERROR_LIGHT             LATDbits.LATD3
#define NUM_TRIES               5
#define MAX_CYCLES              100
#define ALLOWED_ERROR           2
#define HORIZONTAL_TIME_CONST   5
#define VERTICAL_POWER_CONST    10

/* Logic Control Functions */
int move_vertical_to_angle(int target_angle);
int move_horizontal_to_angle(int target_angle);
void error(void);
void variableMsDelay(int num);
void minuteDelay(int minutes);

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
#ifdef DEBUG
    UART_send_str("UART initialized...\n");
    __delay_ms(500);
#endif /* DEBUG */

    // initialize PIC18 as master for SPI
    initSPI();
#ifdef DEBUG
    UART_send_str("SPI initialized...\n");
    __delay_ms(500);
#endif /* DEBUG */
    
    // initialize accelerometer module
    if (!initAccel()) {
        // try for NUM_TRIES otherwise go into error state
        for (int tries = 0; tries < NUM_TRIES; tries++) {
            if (initAccel())
                break;
            error();
        }
    }
#ifdef DEBUG
    UART_send_str("Accel initialized...\n");
#endif /* DEBUG */
    __delay_ms(500);
    
    /* initialize magnetometer module
    if (!Mag_Initialize()) {
        // try for NUM_TRIES otherwise go into error state
        for (int tries = 0; tries < NUM_TRIES; tries++) {
            if (Mag_Initialize())
                break;
            error();
        }
    }*/ 
#ifdef DEBUG
    UART_send_str("Mag initialized...\n");
    __delay_ms(500);
#endif /* DEBUG */
    
    // initialize motors
    pwm_Init();
#ifdef DEBUG
    UART_send_str("PWM initialized...\n");
    __delay_ms(500);
#endif /* DEBUG */
    
    // turn off LED to indicate end of init process
    ERROR_LIGHT = 0;
    __delay_ms(1000);
    
    float float_angles[2] = {0};
    int int_angles[2] = {0};
    while(1) {
        // get target angles and convert to integer type
        get_target_angles(float_angles);
        int_angles[0] = (int) float_angles[0];
        int_angles[1] = (int) float_angles[1];
    
        if (int_angles[0] < 80){ // if the sun is high enough
            /*
            __delay_ms(1000);
            if (move_horizontal_to_angle(int_angles[1]) ){
                error();
            }
            */
            __delay_ms(1000);
            if (move_vertical_to_angle(int_angles[0]) ) {
                error();
            }
        }
        
        minuteDelay(5);
    }
    
    return 0;
}

/* Move horizontal motor to target azimuth angle */
int move_horizontal_to_angle(int target_angle){
    // validate input angle
    if (target_angle < 90 || target_angle > 270) {
        return 0;
    }
    
    int cycles = 0;
    int current_angle;
    int time;
    int direction;
    int speed = 25;
    int error;
    
    do {
        __delay_ms(200);
         
        current_angle = MAG_Angle();
        
#ifdef DEBUG
        char curr_angle_str[20];
        sprintf(curr_angle_str, "Angle: %d\n", current_angle);
        UART_send_str(curr_angle_str);
#endif /* DEBUG */
        
        // validate the current angle
        if (current_angle < 0 || current_angle > 360) {
            return 1;
        }
        
        error = target_angle - current_angle;
        
        // if within allowed angle stop
        if (abs(error) < ALLOWED_ERROR) {
#ifdef DEBUG
            UART_send_str("*** DONE ***\n");
#endif /* DEBUG */
            return 0;
        }
        
        // determine time
        time = (int) HORIZONTAL_TIME_CONST*abs(error); //multiply this by a constant if necessary
        
        // determine direction
        if (current_angle >= 0 && current_angle <= 90) {
            direction = COUNTER_CLOCKWISE;
        } else if ((current_angle >= 270 && current_angle < 0)) {
            direction = CLOCKWISE;
        } else if (current_angle > target_angle) {
            direction = CLOCKWISE;
        } else if (current_angle < target_angle) {
            direction = COUNTER_CLOCKWISE;
        }
        
        // move the motor
        moveMotor(speed, direction, HORIZONTAL);
        variableMsDelay(time);
        stopMotor(HORIZONTAL); 
        
        cycles ++;
    } while (cycles < MAX_CYCLES);
    
    return 0;
}

/* Move vertical motor to target zenith angle */
int move_vertical_to_angle(int target_angle) {
    // validate input angle
    // fully horizontal = 90
    // fully vertical = 0
    if (target_angle < 10 || target_angle > 65) {
        return 0;
    }
    
    target_angle += 5;
    
    int current_angle = 0; 
    int speed = 0; 
    int time = 50;
    
    int cycles = 0; //clockwise moves down, counterclockwise moves up
    int last_error = 0;
    int change_in_error = 0;
    int error = 0;
    int stall_power = 0;
    int direction;
    
    do {
        __delay_ms(500);
        
        // measure current angle
        current_angle = getCurrentZenith();
        
#ifdef DEBUG
        char str[10];
        sprintf(str, "CA: %d\n", current_angle);
        UART_send_str(str);
#endif /* DEBUG */
        
        // validate the current angle
        if (current_angle < -90 || current_angle > 90) {
            return 1;
        }
        
        // calculate error
        error = target_angle - current_angle;
#ifdef DEBUG
        sprintf(str, "e:%d\n", error);
        UART_send_str(str);
#endif /* DEBUG */
        
        // if within allowed angle stop
        if (abs(error) < ALLOWED_ERROR) {
#ifdef DEBUG
            UART_send_str("*** DONE ***\n");
#endif /* DEBUG */
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
            speed = 100;
            time = abs(error); //want to do something like this to adjust the time
        } else { //up
            speed = VERTICAL_POWER_CONST * abs(current_angle) + stall_power;
            time= abs(error)*10;
        }
        
        // check that speed is within correct range
        if (speed < 0) {
            speed = 0;
        } else if (speed > 500) {  
            speed = 500;
        }
        
        // figure what direction to move
        if (current_angle < 0) {
            direction = CLOCKWISE;
#ifdef DEBUG
            UART_send_str("CLOCKWISE\n");
#endif /* DEBUG */
        } else if (error > 0) { //move clockwise

            direction = CLOCKWISE;
#ifdef DEBUG
            UART_send_str("CLOCKWISE\n");
#endif /* DEBUG */
        } else { //move counterclockwise
            direction = COUNTER_CLOCKWISE;
#ifdef DEBUG
            UART_send_str("COUNTER-CLOCKWISE\n");
#endif /* DEBUG */
        }
        
#ifdef DEBUG
        sprintf(str, "Speed: %d\n", speed);
        UART_send_str(str);
#endif /* DEBUG */
        
        // move the motor
        moveMotor(speed, direction, VERTICAL); 
        variableMsDelay(time);
        stopMotor(VERTICAL);    
        
        last_error = error;
        cycles++;
    } while (cycles < MAX_CYCLES);
    
    return 0;
}

/* enter into error state: flash error LED infinitely */
void error() {
    // keep on flashing error LED until users shuts power
    while(1) {
        ERROR_LIGHT = 1;
        __delay_ms(1000);
        ERROR_LIGHT = 0;
        __delay_ms(1000);
    }
}

/* delay for a variable number of milli-seconds */
void variableMsDelay(int num) {
    for (int i = 0; i < num; i++)
        __delay_ms(1);
}

/* delay for some # of minutes */
void minuteDelay(int min) {
    for (int i = 0; i < min*60; i++)
        __delay_ms(1000);
}
