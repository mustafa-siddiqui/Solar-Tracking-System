/* 
 * File:   motor.h
 * Author: mahmudjumaev
 *
 * Created on April 21, 2022, 10:10 PM
 */

#ifndef MOTOR_H
#define	MOTOR_H

#define _XTAL_FREQ 8000000

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



void pwm_Init(void);

/**
 * @brief   Move motor which controls horizontal motion in the
 *          specified direction with a specified duty cycle.
 */
void moveHorizontalMotor(int dutyCycle, int dir);

/**
 * @brief   Move motor which controls vertical motion in the
 *          specified direction with a specified duty cycle.
 */
void moveVerticalMotor(int dutyCycle, int dir);

/**
 * @brief   Stop the Vertical Motor by setting the Duty Cycle
 *          to 0%.
 */
void stopVerticalMotor(void);

/**
 * @brief   Stop the Horizontal Motor by setting the Duty Cycle
 *          to 0%.
 */
void stopHorizontalMotor(void);

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
void moveMotor(int dutyCycle, int dir, int motorNum);

#endif	/* MOTOR_H */

