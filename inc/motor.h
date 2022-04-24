/* 
 * File:   motor.h
 * Author: mahmudjumaev
 *
 * Created on April 21, 2022, 10:10 PM
 */

#ifndef MOTOR_H
#define	MOTOR_H

#define _XTAL_FREQ 8000000

//#define DUTY_CYCLE_CONST 5

#define SET(reg, bitNum)   (reg |= (1 << bitNum))
#define CLEAR(reg, bitNum) (reg &= ~(1 << bitNum))

// TODO: Add function declarations with comment blocks

#endif	/* MOTOR_H */

