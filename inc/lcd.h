/**
 * @file    accel.h
 * @author  Mustafa Siddiqui, Ali Choudhry
 * @brief   Header file for LCD related functions
 *          => Functions beginning with a '_' (underscore) 
 *             are essentially helper functions not to be
 *             used outside this module <=
 * @date    04/12/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _LCD_H_
#define _LCD_H_

#define _XTAL_FREQ 8000000  // 8 MHz

#define RS LATA1                   /* PORTD 0 pin is used for Register Select */
#define EN LATA0                   /* PORTD 1 pin is used for Enable */
#define RW LATA2
//#define TRANSMIT LATB                 /* PORTB is used for transmitting data to LCD */

//#define LCD_Port TRISB              
//#define LCD_Control TRISA


void lcd_display_driver_enable();
void lcd_display_driver_initialize();
void lcd_display_driver_clear();
void lcd_display_driver_write( char* data, int length );
void display_driver_use_first_line( void );
void display_driver_use_second_line( void );

#endif /* _LCD_H_ */