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

enum dir {
    CLOCKWISE,
    COUNTER_CLOCKWISE
};

enum motorNum {
    HORIZONTAL,
    VERTICAL
};

// TODO: Add comment blocks

void pwm_Init(float f);
void set_duty_cycle(int fraction, float f, int dir);

// TODO: Implement these functions - @mjumaev

/**
 * @brief   Move motor which controls horizontal motion in the
 *          specified direction with a specified duty cycle.
 */
void moveHorizontalMotor(int dutyCycle, float freq, int dir);

/**
 * @brief   Move motor which controls vertical motion in the
 *          specified direction with a specified duty cycle.
 */
void moveVerticalMotor(int dutyCycle, float freq, int dir);

/**
 * @brief   Move one of the motors to control horizontal or 
 *          vertical motion in a specified direction with a
 *          specified duty cycle.
 *          => primary func to be called from main()
 * @param   dutyCycle: value from 0 - 100
 * @param   freq: frequency - required for pwm calculations
 * @param   dir: direction => CLOCKWISE or COUNTER_CLOCKWISE 
 * @param   motorNum: which motor to move => HORIZONTAL or VERTICAL
 * @return  NONE
 */
void moveMotor(int dutyCycle, float freq, int dir, int motorNum);

#endif	/* MOTOR_H */

