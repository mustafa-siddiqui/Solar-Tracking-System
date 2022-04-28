/* 
 * File:   lcd_slave.h
 * Author: SeniorDesign-2022
 *
 * Created on April 27, 2022, 8:06 PM
 */


#ifndef _lcd_slave_H_
#define _lcd_slave_H_


#include <stdint.h> // int16_t

signed char LCD_Write(char* data_transmit, int length);

#endif /* _lcd_slave_H_ */