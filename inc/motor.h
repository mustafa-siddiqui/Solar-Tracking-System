/* 
 * @file    motor.h
 * @brief   Header file for motor control module.
 * @author  Mahmud Jumaev, Mustafa Siddiqui
 * @date    4/27/22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MOTOR_H
#define	MOTOR_H

#define _XTAL_FREQ 8000000

/* macros to set specific bits in a register */
#define SET(reg, bitNum)   (reg |= (1 << bitNum))
#define CLEAR(reg, bitNum) (reg &= ~(1 << bitNum))

/* to select direction for a motor */
enum dir {
    CLOCKWISE,
    COUNTER_CLOCKWISE
};

/* to select a motor */
enum motorNum {
    HORIZONTAL,
    VERTICAL
};

/**
 * @brief   Initialize the PWM module to enable duty cycle
 *          to be set and motors be moved.
 */
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
 * @param   dir: direction => CLOCKWISE or COUNTER_CLOCKWISE 
 * @param   motorNum: which motor to move => HORIZONTAL or VERTICAL
 * @return  NONE
 */
void moveMotor(int dutyCycle, int dir, int motorNum);

/**
 * @brief   Stop one of the motors -- horizontal or vertical.
 *          => primary func to be called from main()
 * @param   motorNum: which motor to stop => HORIZONTAL or VERTICAL
 * @return  NONE
 */
void stopMotor(int motorNum);

#endif	/* MOTOR_H */

