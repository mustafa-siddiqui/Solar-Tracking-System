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
#include "../inc/uart.h"    // added for debugging
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
void pwm_Init(float f) {
    TRISCbits.RC2=0; //Setting RC2/CCP1 pin as an output
    PORTCbits.RC2=0;
    TRISDbits.RD4=0; //Setting RD4/ECCP1 pin as an output
    PORTDbits.RD4=0;
    //float Tosc=1/(8*10^(6));
    int Prescale=16;
    //float f = 8e3;
    float PWM_Period = 1/f;
    //PR2=(PWM_Period/(4*Tosc*Prescale))-1;
 
    //int num = (10^(-3)*8*10^6)/(4*16)-1;
    
    int num = (int)(PWM_Period * _XTAL_FREQ)/(4 * Prescale) - 1;
    PR2=(unsigned char)(num);//10^(-3)/(4*(1/(8*10^6))*16)-1;
    char prStr[15];
    sprintf(prStr, "PR2: %x", PR2);
    UART_send_str(prStr);
    __delay_ms(100);
    //PR2=0b01111100; //PR2=(PWM2_Period/(4*Tosc*Prescale))-1=10^(-3)/(4*(1/8*10^6)*16)-1=124
    T2CON=0b00000111; //Prescalar set to 16
    
}

/*
 accepts values between 0-100
 */
void motor1_duty_cycle(int fraction, int dir) {
    UART_send_str("calling set duty cycle function");
    TRISCbits.RC1=0; //Setting RC2/CCP1 pin as an output
    PORTCbits.RC1=dir;
    //char foscVal[10];
    //sprintf(foscVal, "fosc Val: %lf", fosc);
    //UART_send_str(foscVal);
    
    int Prescale=16;
    float f=8e3; //PWM runs at 8kHz
    float PWM_Period=1/f;
    
    //int desired_CCPR = PWM_Period*(fraction/100)/(Tosc*Prescale)= PWM_Period*(fraction)*fosc/(100*Prescale);
    int desired_CCPR = (int)((PWM_Period * fraction * _XTAL_FREQ)/(100 * Prescale));
    //Duty_cycle/(Tosc*Prescalar)=10^(-3)*0.5/(1/(8/4*10^6)*16)=250 (0011111010) = 500*0.5 //check this formula
    //CCPR1L=0b00111110;
    
    // debug statement
    char prStr[15];
    sprintf(prStr, "CCPR: %x", desired_CCPR);
    UART_send_str(prStr);
    
    CCPR1L = (desired_CCPR & 0xFC )>> 2;
    CCP1CON = 0b00101100;
    
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

void motor2_duty_cycle(int fraction, int dir) {
    UART_send_str("calling set duty cycle function");
    TRISDbits.RD5=0; //Setting RC2/CCP1 pin as an output
    PORTDbits.RD5=dir;
    
    float f=8e3; //PWM runs at 8kHz
    int Prescale=16;
    
    float PWM_Period=1/f;
    
    //int desired_CCPR = PWM_Period*(fraction/100)/(Tosc*Prescale)= PWM_Period*(fraction)*fosc/(100*Prescale);
    int desired_CCPR = (int)((PWM_Period * fraction * _XTAL_FREQ)/(100 * Prescale));
    //Duty_cycle/(Tosc*Prescalar)=10^(-3)*0.5/(1/(8/4*10^6)*16)=250 (0011111010) = 500*0.5 //check this formula
    //CCPR1L=0b00111110;
    
    // debug statement
    char prStr[15];
    sprintf(prStr, "CCPR: %x", desired_CCPR);
    UART_send_str(prStr);
    
    ECCPR1L = (desired_CCPR & 0xFC )>> 2;
    ECCP1CON = 0b00101100;
    
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

void moveHorizontalMotor(int dutyCycle, float freq, int dir) {
    // TODO: implement this function
}

void moveVerticalMotor(int dutyCycle, float freq, int dir) {
    // TODO: implement this function
}

/* move one of the motors in specified direction */
void moveMotor(int dutyCycle, int dir, int motorNum) {
    switch (motorNum) {
        case HORIZONTAL:
            moveHorizontalMotor(dutyCycle, freq, dir);
            break;
        case VERTICAL:
            moveVerticalMotor(dutyCycle, freq, dir);
            break;
        default:
            // do nothing if incorrect motor
            break;
    }
}
