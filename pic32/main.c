/**
 * @file    explorerBoard.c
 * @author  Mustafa Siddiqui
 * @brief   Code to initialize and run PIC32 on 16/32 Explorer Board
 *          in slave mode to receive data from PIC18.
 * @date    04/27/22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <xc.h>
#include <sys/attribs.h>
#include <stdlib.h>         // atoi()
#include <stdio.h>          // sprintf()

#include "lcd_display_driver.h"

/*  Configure CPU and peripheral bus clock to run at 80MHz
    f_crystal = 8MHz
    f_CPU = (f_crystal * FPLLMUL) / (FPLLIDIV * FPLLODIV)
    f_peripheral = f_CPU / FPBDIV
 */
#pragma config POSCMOD = HS         // primary oscillator mode: high speed crystal
#pragma config FNOSC = PRIPLL       // oscillator selection: primary with PLL
#pragma config FPLLMUL = MUL_20     // PLL multiplier - multiply by 20
#pragma config FPLLIDIV = DIV_2     // PLL input divider - divide by 2
#pragma config FPLLODIV = DIV_1     // PLL output divider - divide by 1
#pragma config FPBDIV = DIV_1       // peripheral bus clock - divide by 1

/* Global var to store angle received from pic18 */
volatile int angle = 0;

// Read a character array sent to slave SPI4 from master SPI.
// Character Array is passed by reference; the function doesn't return anything.
void readSPI4(char* msg, unsigned int length) {
    char data;
    unsigned int complete = 0;
    unsigned int numBytes = 0;

    while (!complete) {
        // if data is available
        if (SPI4STATbits.SPIRBF) {
            data = SPI4BUF;
            if (data == '\n') {
                complete = 1;
            }
            else {
                msg[numBytes] = data;
                numBytes++;

                // rollover if char array not big enough
                if (numBytes > length) {
                    numBytes = 0;
                }

                // clear overflow bit to enable receival
                SPI4STATbits.SPIROV = 0;
            }
        }
    }
    msg[numBytes] = '\0';

    // clear status bits (receive buffer full & overflow)
    SPI4STATbits.SPIRBF = 0;
    SPI4STATbits.SPIROV = 0;
}

// Initialize SPI4 as slave in 8 bit mode.
// Initialize SPI interrupt upon receival.
// Configure bits:
// SDI4 -> p49 (RF4)
// SDO4 -> p50 (RF5)
// SCK4 -> p39 (RF13)
// => probably need to set the clock same as the master
void SPI4_slaveInit(void) {
    // SDO4 as output, rest are input by default
    TRISFbits.TRISF5 = 0;
    
    __builtin_disable_interrupts();

    // clear the rx buffer and overflow
    SPI4BUF;
    SPI4STATbits.SPIROV = 0;
    
    // clock polarity and clock edge
    SPI4CONbits.CKP = 0;
    SPI4CONbits.CKE = 0;
    
    // 8 bit mode
    SPI4CONbits.MODE32 = 0;
    SPI4CONbits.MODE16 = 0;
    
    // set up SPI interrupt
    INTCONbits.MVEC = 1;            // enable multi-vector interrupts
    IFS1bits.SPI4RXIF = 0;          // clear receive status bit
    IEC1bits.SPI4RXIE = 1;          // enable interrupt on receive
    IPC8bits.SPI4IP = 6;            // priority = 6
    IPC8bits.SPI4IS = 3;            // sub-priority = 3
    
    // turn on SPI module in slave mode
    SPI4CONbits.MSTEN = 0;
    SPI4CONbits.ON = 1;
    
    __builtin_enable_interrupts();
}

// SPI4 ISR: Receive string from PIC18 MCU.
void __ISR(_SPI_4_VECTOR, IPL6SOFT) SPI4_ISR(void) {
    // check if receival
    if (IFS1bits.SPI4RXIF) {
        // receive data from pic18
        char receiveMsg[10];
        readSPI4(receiveMsg, 10);

        // convert str to int
        angle = atoi(receivedMsg);

        // clear receive status bit
        IFS1bits.SPI4RXIF = 0;
    }
}

// Initialize Timer 2 interrupt to trigger at 15 Hz to update LCD Display
void TMR2_init(void) {
    INTCONbits.MVEC = 1;        // enable multi-vector interrupts
    __builtin_disable_interrupts();
    
    // Timer 2 interrupt is configured to trigger at a frequency of 15 Hz
    // PR2 = ((1/15)/(12.5 ns)) / 256 = 20834
    PR2 = 20834;
    TMR2 = 0;                   // start at 0
    T2CONbits.ON = 1;           // enable timer 2
    T2CONbits.TCKPS = 0b111;    // or 0x7; pre-scalar set to 256
    IPC2bits.T2IP = 2;          // priority = 2
    IPC2bits.T2IS = 0;          // sub-priority = 0
    IFS0bits.T2IF = 0;          // clear status flag
    IEC0bits.T2IE = 1;          // enable interrupt
    
    __builtin_enable_interrupts();
}

// TMR2 ISR: Converts the x and y coordinates into a string and
// displays them on the first line of the LCD display.
void __ISR(_TIMER_2_VECTOR, IPL2SOFT) TMR2_ISR(void) {
    // convert coordinates into string
    char angleDisplay[20] = {};
    int length = sprintf(angleDisplay, "Angle: %d", angle);

    // display on lcd display
    lcd_display_driver_clear();
    delay_1us(5000);            // 5 ms delay
    display_driver_use_first_line();

    lcd_display_driver_write(angleDisplay, length);

    // clear status flag
    IFS0bits.T2IF = 0;
}

// Main function: Initialize the LCD, SPI 4 as slave, 
// and Timer 2. Receive data from pic18 and display 
// on LCD display.
int main(void) {
    // disable debugging through JTAG
    DDPCONbits.JTAGEN = 0;

    // set pins as digital outputs
    TRISFbits.TRISF1 = 0;       // p88
    TRISFbits.TRISF0 = 0;       // p87

    // set up LCD Display
    lcd_display_driver_set_tristate();
    lcd_display_driver_initialize();

    // initialization
    SPI4_slaveInit();
    TMR2_init();

    // infinite loop
    while (1) {
        ;
    }

    return 0;
}
