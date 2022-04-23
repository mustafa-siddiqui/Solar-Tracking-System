/**
 * @file    motor.c
 * @author  Mahmud Jumaev
 * @brief   Function definitions for motor control.
 * @date    04/12/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "motor.h"
#include "uart.h"    // added for debugging
//-//
#include <xc.h>
#include <stdio.h>  // sprintf()
#include <math.h>   // sqrt()
#include <stdlib.h>
#include <pic18.h>
#include <string.h>


#define _XTAL_FREQ 8000000
//#pragma config FOSC = HS
//#pragma config WDT = OFF
//#pragma config PBADEN = OFF
//#pragma config LVP = OFF
//#pragma config PWRT = ON
//#pragma config DEBUG=OFF

//#define DUTY_CYCLE_CONST 5


#define SET(reg, bitNum)   (reg |= (1 << bitNum))
#define CLEAR(reg, bitNum) (reg &= ~(1 << bitNum))

void pwm_Init(float f) {
    TRISCbits.RC2=0; //Setting RC2/CCP1 pin as an output
    PORTCbits.RC2=0;
    
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
void set_duty_cycle(int fraction, float f, int dir) {
    UART_send_str("calling set duty cycle function");
    TRISCbits.RC1=0; //Setting RC2/CCP1 pin as an output
    PORTCbits.RC1=dir;
    //char foscVal[10];
    //sprintf(foscVal, "fosc Val: %lf", fosc);
    //UART_send_str(foscVal);
    
    int Prescale=16;
    //float f=8e3;
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