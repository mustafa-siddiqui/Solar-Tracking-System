/*
 * File:   SPI_test.c
 * Author: Ali Choudhry, Carter Bordeleau, Mahmud Jumaev, Mustafa Siddiqui.
 *
 * Created on April 4, 2022, 9:54 PM
 * From   https://www.electronicwings.com/pic/pic18f4550-spi
 */

#include <xc.h>
#include <pic18.h>
//#include "Configurations_Header_File.h"

//#define CS LATA5
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

//These are all registers which we are able to read and write to in the Magnetometer
//These are environmental compensation registers. We probably don't need them
#define OFFSET_X_REG_L 0x45
#define OFFSET_X_REG_H 0x46
#define OFFSET_Y_REG_L 0x47
#define OFFSET_Y_REG_H 0x48
#define OFFSET_Z_REG_L 0x49
#define OFFSET_Z_REG_H 0x4A

#define WHO_AM_I 0x4F           //Cannot be written to, read only, identifies the sensor

//These are registers used for initialization of sensor
#define CFG_REG_A 0x60
#define CFG_REG_B 0x61
#define CFG_REG_C 0x61

#define INT_CRTL_REG 0x63   //interrupt register, probably won't be needed
#define INT_THS_L_REG 0x65 //interrupt
#define INT_THS_H_REG 0x66 //interrupt

void SPI_Init_Master(){
    /* PORT definition for SPI pins*/    
    TRISCbits.TRISC4 =1;	/* RC4 as input(SDI) */
    TRISCbits.TRISC3=0;		/* RC3 as output(SCK) */
    TRISEbits.TRISE2=0;		/* RE2 as a output(SS) */
    TRISCbits.TRISC5=0;		/* RC5 as output(SDO) */

    /* To initialize SPI Communication configure following Register*/
    LATAbits.LATA5 = 1;
    SSPSTAT=0x40;		/* Data change on rising edge of clk , BF=0*/
    SSPCON1=0x22;		/* Master mode,Serial enable,
                            idle state low for clk, fosc/64 */ 
    PIR1bits.SSPIF=0;

    /* Disable the ADC channel which are on for multiplexed pin when
    used as an input */    
    ADCON0=0;			/* This is for de-multiplexed the 
                            SCL and SDI from analog pins*/
    ADCON1=0x0F;		/* This makes all pins as digital I/O */
}

void SPI_Write(unsigned char x){
    unsigned char data_flush;
    SSPBUF=x;			/* Copy data in SSBUF to transmit */
    while(!PIR1bits.SSPIF);	/* Wait for complete 1 byte transmission */
    PIR1bits.SSPIF=0;		/* Clear SSPIF flag */
    data_flush=SSPBUF;		/* Flush the data */
}

unsigned char SPI_Read(){    
    SSPBUF=0xFF;		/* Copy flush byte in SSBUF */
    while(!PIR1bits.SSPIF);	/* Wait for complete 1 byte transmission */
    PIR1bits.SSPIF=0;
    return(SSPBUF);		/* Return received byte */   
}

void MSdelay(unsigned int val){	/* Delay of 1 ms for 8MHz Frequency */
     unsigned int i,j;
        for(i=0;i<val;i++)
            for(j=0;j<165;j++);
}



unsigned char CFG_A_initialize = 0b10000001; //We set initialization register to compensate for temperature, high power mode,
                              //default state, output frequency of 10Hz, and single measurement mode
unsigned char CFG_B_initialize = 0b00000000; //We don't need this register, used for bandwidth filters and interrupts
unsigned char CFG_C_initialize = 0b00110100; // No interrupts, enable SPI mode, ignore innaccurate readings, no self-test, no data-ready mode

void mag_initialize(void){      //for magnetometer, first write bit is 0
    SPI_Write(0);
    SPI_Write(CFG_REG_A);
    SPI_Write(CFG_A_initialize);
    
    SPI_Write(0);
    SPI_Write(CFG_REG_C);
    SPI_Write(CFG_C_initialize);
}

void main(){
    TRISBbits.TRISB7 = 0;
    TRISDbits.TRISD2 = 0;
    LATBbits.LATB7 = 1;
    OSCCON = 0x72;		/* Use internal frequency 8 MHz */    
    //INTCON2bits.RBPU=0;		/* Enable internal Pull-up of PORTC */
    SPI_Init_Master();		/* Initialize SPI communication */
    MSdelay(10);
    mag_initialize();
    
   while(1)
    {
        LATEbits.LATE2 = 0;     
            SPI_Read();	
            SPI_Read();	
            SPI_Read();
            
            MSdelay(1000);
            LATDbits.LATD2 = 1;
        }
        LATEbits.LATE2 = 1;
}