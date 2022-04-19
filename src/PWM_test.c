/* 
 * File:   PWM_test.c
 * Author: SeniorDesign-2022
 *
 * Created on April 19, 2022, 1:14 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <pic18.h>
#include <string.h>
#include <stdlib.h>

#include "uart.h"


#define CS LATA5
#define LED LATD

#pragma config OSC = IRCIO67       // Oscillator Selection bits (EC oscillator, port function on RA6)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = BOHW     // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (VBOR set to 2.1V)

/* disable watchdog timer */
// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config BBSIZ = 1024     // Boot Block Size Select bits (1K words (2K bytes) Boot Block)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

//#pragma config FOSC = HS
//#pragma config WDT = OFF
//#pragma config PBADEN = OFF
//#pragma config LVP = OFF
//#pragma config PWRT = ON
//#pragma config DEBUG=OFF

//#define DUTY_CYCLE_CONST 5


#define SET(reg, bitNum)   (reg |= (1 << bitNum))
#define CLEAR(reg, bitNum) (reg &= ~(1 << bitNum))

void pwm_Init() {
    TRISCbits.RC2=0; //Setting RC2/CCP1 pin as an output
    PORTCbits.RC2=0;
    PR2=0b01111100; //PR2=(PWM2_Period/(4*Tosc*Prescale))-1=10^10^(-3)/4*(1/8*10^6)*16)=124
    T2CON=0b00000111;
    
}

/*
 accepts values between 0-100
 */
void set_duty_cycle(int fraction) {
    UART_send_str("calling set duty cycle function");
    
    int desired_CCPR = 5*fraction;
    //Duty_cycle/(Tosc*Prescalar)=10^(-3)*0.5/(1/(8*10^6)*16)=250 (0011111010) = 500*0.5
    //CCPR1L=0b00111110;
    CCPR1L = (desired_CCPR & 0xFC )>> 2;
    CCP1CON=0b00101100;
    
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

void main(void) {
    
    OSCCON = 0x72;		/* Use internal frequency 8 MHz */   
    
    pwm_Init();
    //set_duty_cycle(50);
    UART_RX_Init();
    UART_send_str("Starting");
    //CCPR1L=0b00111110;
    //CCP1CON=0b00101100;
    //CCP1CONbits.DC1B0 = 0;
    //CCP1CONbits.DC1B1 = 1;.
    //CCP1CONbits.CCP1M
    set_duty_cycle(100);
    
    while(1)
    {
        
       ;
        
    }
    

}

