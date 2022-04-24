/*
 * File:   lcd.c
 * Author: Ali Choudhry
 *
 * Created on April 23, 2022, 11:06 PM
 */


#include <xc.h>
#include <stdio.h>  // sprintf()
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "lcd.h"
//LCD Functions Developed by Circuit Digest.

void lcd_display_driver_enable(){
    EN = 1;
    //for loop that keeps running to give sufficient time for board to start
    int x;
    for (x = 0; x < 1000; x++){;}
    EN = 0;//disable the display
}

void lcd_display_driver_initialize(){       //operates in a mode with 5x7dots, dual line, and no cursor.
    // function Set
    RS = 0;
    RW = 0;
    EN = 0;
    //TRISB = 0x0;
    
    RW = 0;     //set RD5 to 0,  
    RS = 0;    //set RB15 to 0, display read/write
    LATB = 0x3C;              //5x7 dots, and dual line mode
    lcd_display_driver_enable();//
    
    // display on/off
    RW = 0;     //set RD5 to 0,  
    RS = 0;    //set RB15 to 0, display read/write
    LATB = 0xC;
    lcd_display_driver_enable();//
    
     // clear display
    RW = 0;     //set RD5 to 0,  
    RS = 0;    //set RB15 to 0, display read/write
    LATB = 0x1;
    lcd_display_driver_enable();
    
     // entry mode
    RW = 0;     //set RD5 to 0,  
    RS = 0;    //set RB15 to 0, display read/write
    LATB = 0x6;
    lcd_display_driver_enable();      
}

void lcd_display_driver_clear(){
    // clear display
    RW = 0;     //set RD5 to 0,  
    RS = 0;    //set RB15 to 0, display read/write
    LATB = 0x1;
    lcd_display_driver_enable();   
}


void lcd_display_driver_write(char* data, int length){ // function that writes char data on display
    
    int i = 0;// creates int i and sets value to 0
    
    while ( i < length ){// while loop for when i is less than length
        RW = 0; // sets RD5 to 0
        RS = 1; //set RB15 to 0, display read/write
        
        LATB = data[i];// sets LATE to character
        lcd_display_driver_enable(); // calls on the enable function
        
        i = i+1;    //loops that keeps incrementing i until it reaches the length of data
        }
}


void display_driver_use_first_line( void ){
    
    RW = 0;     //set RD5 to 0,  
    RS = 0;    //set RB15 to 0, display read/write
    LATB = 0x80 ;          //we use DD RAM Address Set for first line, 80
    lcd_display_driver_enable();
   
}

void display_driver_use_second_line( void ){
    
    RW = 0;     //set RD5 to 0,  
    RS = 0;    //set RB15 to 0, display read/write
    LATB = 0xC0 ;          //we use DD RAM Address Set, C0
    lcd_display_driver_enable();
}