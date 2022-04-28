/**
 * @file    motor.c
 * @author  Mahmud Jumaev
 * @brief   Function definitions for motor control.
 * @date    04/12/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../inc/motor.h"
//-//
// TODO: remove unused libraries
#include <xc.h>
#include <stdio.h>  // sprintf()
#include <math.h>   // sqrt()
#include <stdlib.h>
#include <string.h>

// no delays should be called in this function
// initialization should happen as quick as possible to 
// minimize initial jerk to motor
void pwm_Init(void) {
    TRISCbits.RC2 = 0;      // Setting RC2/CCP1 pin as an output
    PORTCbits.RC2 = 0;
    TRISDbits.RD4 = 0;      // Setting RD4/ECCP1 pin as an output
    PORTDbits.RD4 = 0;
    
    float f = 8e3;          // PWM runs at 8kHz
    int Prescale = 16;      // Timer 2 Prescale set to 16
    float PWM_Period = 1/f; // Calculating the PWM period
    
    
    // Calculating the value for PR2 register PR2 = (PWM_Period/(4*Tosc*Prescale))-1;
    int num = (int)(PWM_Period * _XTAL_FREQ)/(4 * Prescale) - 1;
    PR2 = (unsigned char)(num);                   //10^(-3)/(4*(1/(8*10^6))*16)-1;
    T2CON = 0b00000111;                           // Prescalar set to 16. THe datasheet explains how the bits are set   
}

/*
 accepts values between 0-500. 500 corresponds to 100% duty cycle
 */
void moveVerticalMotor(int fraction, int dir) {
    TRISCbits.RC1 = 0;                  // Setting RC1 as output
    PORTCbits.RC1 = (unsigned char)dir; // Select the direction of motor rotation (inputs: 1 or 0)
    
    int Prescale = 16;      // Timer 2 Prescale value 16
    float f = 8e3;          // PWM runs at 8kHz
    float PWM_Period = 1/f;
    
    // desired_CCPR = PWM_Period*(fraction/100)/(Tosc*Prescale)= PWM_Period*(fraction)*fosc/(100*Prescale);
    int desired_CCPR = (int)((PWM_Period * fraction * _XTAL_FREQ)/(100 * Prescale));
    
    // Based on the calculations of desired_CCPR, set the 8 most significant bits (MSB) to CCPR1L
    CCPR1L = (desired_CCPR & 0xFC ) >> 2;
    
    // Default value for CCP1CON
    CCP1CON = 0b00101100;
    
    // Set the 2 least significant bits (LSB) of desired_CCPR to bits 4 and 5 of CCP1CON.
    if( desired_CCPR & 0b1){
        SET(CCP1CON, 4);
    } else {
        CLEAR(CCP1CON, 4);
    }
    
    if (desired_CCPR & 0b10) {
        SET(CCP1CON, 5); 
    } else {
        CLEAR(CCP1CON, 5); 
    }
}

void moveHorizontalMotor(int fraction, int dir) {
    TRISDbits.RD5 = 0;                  // Setting RD5 as output pin
    PORTDbits.RD5 = (unsigned char)dir; // Select the direction of motor rotation (inputs: 1 or 0)
    
    float f = 8e3;          // PWM runs at 8kHz
    int Prescale = 16;      // Timer 2 Prescalar selected as 16
    float PWM_Period = 1/f; // Calculating the PWM period
    
    // desired_CCPR = PWM_Period*(fraction/100)/(Tosc*Prescale)= PWM_Period*(fraction)*fosc/(100*Prescale);
    int desired_CCPR = (int)((PWM_Period * fraction * _XTAL_FREQ)/(100 * Prescale));
    
    // Based on the calculations of desired_CCPR, set the 8 most significant bits (MSB) to ECCPR1L
    ECCPR1L = (desired_CCPR & 0xFC ) >> 2;
    
    // Default value for ECCP1CON
    ECCP1CON = 0b00101100;
    
    // Set the 2 least significant bits (LSB) of desired_CCPR to bits 4 and 5 of ECCP1CON.
    if( desired_CCPR & 0b1){
        SET(ECCP1CON, 4);
    } else {
        CLEAR(ECCP1CON, 4);
    }
    
    if (desired_CCPR & 0b10) {
        SET(ECCP1CON, 5); 
    } else {
        CLEAR(ECCP1CON, 5); 
    }
}

// Set the duty cycle to 0% to stop the Horizontal Motor
void stopVerticalMotor(void){
    moveVerticalMotor(0, 1);
}

// Set the duty cycle to 0% to stop the Horizontal Motor
void stopHorizontalMotor(void){
    moveHorizontalMotor(0, 1); 
}

/* move one of the motors in specified direction */
void moveMotor(int dutyCycle, int dir, int motorNum) {
    switch (motorNum) {
        case HORIZONTAL:
            moveHorizontalMotor(dutyCycle, dir);
            break;
        case VERTICAL:
            moveVerticalMotor(dutyCycle, dir);
            break;
        default:
            // do nothing if incorrect motor
            break;
    }
}

/* stop one of the motors */
void stopMotor(int motorNum) {
    switch (motorNum) {
        case HORIZONTAL:
            stopHorizontalMotor();
            break;
        case VERTICAL:
            stopVerticalMotor();
            break;
        default:
            // do nothing if incorrect motor
            break;
    }
}
