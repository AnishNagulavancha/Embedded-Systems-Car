/*
 * movement.c
 *
 *  Created on: Sep 29, 2024
 *      Author: anish
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "movement.h"
#include "switches.h"
#include "timerb0.h"
#include "adc.h"
#include "pwm.h"

extern volatile char display_changed;
extern char display_line[4][11];
unsigned int p6;
extern int average;
extern unsigned int wait;

void project_7(void) {
    switch (p6) {
        case 0:
            if (ADC_Right_Det < 400 && ADC_Left_Det < 400) {
                Forward_ON();
            } else {
                Wheels_OFF();
                p6 = 1;
                wait = 0;
            }
            break;

        case 1:
            if (wait >= 25) {
                wait = 0;
                p6 = 2;
            }
            break;
        case 2:
            if (ADC_Right_Det < 400 && ADC_Left_Det < 400) {
                Turn();
            } else {
                Wheels_OFF();
                p6 = 3;
                wait = 0;
            }
            break;
        case 3:
            if (wait >= 25) {
                wait = 0;
                p6 = 4;
            }
            break;

        case 4:
            if (ADC_Right_Det > 400 && ADC_Left_Det < 400) {
                Forward_Left();

            }
            else if (ADC_Right_Det < 400 && ADC_Left_Det > 400) {
                Forward_Right();

            }
            else if (ADC_Right_Det < 400 && ADC_Left_Det < 400) {
                Turn_CW();
            }
            else {
                Forward_ON();
            }

            break;
        default:break;
    }

}
void end_case(void){
 Forward_OFF();
 state = WAIT;
 event = NONE;
}

void Forward_OFF(void){
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_FORWARD;
}

void Forward_ON(void){
    RIGHT_FORWARD_SPEED = RIGHT_WHEEL_SLOW; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = LEFT_WHEEL_SLOW; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
}
void Forward_Left(void){
    RIGHT_FORWARD_SPEED = RIGHT_WHEEL_FORWARD_LEFT; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
}
void Forward_Right(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = LEFT_WHEEL_FORWARD_RIGHT; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
}
void Wheels_OFF(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
}
void Reverse_ON(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = RIGHT_WHEEL_REVERSE; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = LEFT_WHEEL_REVERSE; // P6.4 Left Reverse PWM OFF
}
void Turn(void){
    RIGHT_FORWARD_SPEED = RIGHT_WHEEL_TURN; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = LEFT_WHEEL_TURN; // P6.4 Left Reverse PWM OFF
}
void Turn_CW(void){
    RIGHT_FORWARD_SPEED = RIGHT_WHEEL_CW; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
}
void Turn_CCW(void){
    RIGHT_FORWARD_SPEED = RIGHT_WHEEL_CCW; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF

}
void Spin_CW(void){
    P6OUT |= R_FORWARD;
    P6OUT &= ~L_FORWARD;
    P6OUT &= ~R_REVERSE;
    P6OUT |= L_REVERSE;
}
void Spin_CCW(void){
    P6OUT &= ~R_FORWARD;
    P6OUT |= L_FORWARD;
    P6OUT |= R_REVERSE;
    P6OUT &= ~L_REVERSE;
}



