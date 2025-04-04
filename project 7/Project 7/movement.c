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

//void project_7(void) {
//    switch (p6) {
//        case 0:
//            if (ADC_Right_Det < 150 && ADC_Left_Det < 150) {
//                Forward_ON();
//                strcpy(display_line[3], "  forward ");
//                display_changed = 1;
//            } else {
//                Wheels_OFF();
//                p6 = 1;
//                wait = 0;
//                strcpy(display_line[3], "  forward0 ");
//                display_changed = 1;
//            }
//            break;
//
//        case 1:
//            if (wait >= 5) {
//                wait = 0;
//                p6 = 2;
//                strcpy(display_line[3], "  stop1 ");
//                display_changed = 1;
//            }
//            break;
//        case 2:
//            if (ADC_Right_Det < 200 && ADC_Left_Det < 200) {
//                Turn();
//
//                strcpy(display_line[3], " turn ");
//                display_changed = 1;
//            } else {
//                Wheels_OFF();
//                p6 = 3;
//                wait = 0;
//                strcpy(display_line[3], " turn ");
//                display_changed = 1;
//            }
//            break;
//        case 3:
//            if (wait >= 5) {
//                wait = 0;
//                p6 = 4;
//                strcpy(display_line[3], "  stop3 ");
//                display_changed = 1;
//            }
//            break;
//
//        case 4:
//            if (ADC_Right_Det > 200 && ADC_Left_Det < 200) {
//                strcpy(display_line[3], "  circle ");
//                display_changed = 1;
//                Forward_Right();
//
//
//            }
//            else if (ADC_Right_Det < 200 && ADC_Left_Det > 200) {
//                strcpy(display_line[3], "  circle ");
//                display_changed = 1;
//                Forward_Left();
//
//            }
//            else {
//                Forward_2();
//                strcpy(display_line[3], "  circle ");
//                display_changed = 1;
//            }
//            if (wait > 500) {
//                wait = 0;
//                p6 = 5;
//                Wheels_OFF();
//                strcpy(display_line[3], "  stop4 ");
//                display_changed = 1;
//            }
//            break;
//        case 5:
//            if (wait < 10) {
//                Turn();
//                strcpy(display_line[3], "  turn ");
//                display_changed = 1;
//                wait = 0;
//                p6 = 6;
//            }
//            break;
//        case 6:
//            if (wait < 5) {
//                Forward_ON();
//                wait = 0;
//                strcpy(display_line[3], "  final ");
//                display_changed = 1;
//            }
//            else {
//                Wheels_OFF();
//                strcpy(display_line[3], "  final ");
//                display_changed = 1;
//            }
//            break;
//
//        default:break;
//    }
//
//}
void end_case(void){
 Forward_OFF();
 state = WAIT;
 event = NONE;
}

void Forward_ON(void){
    RIGHT_FORWARD_SPEED = RIGHT_WHEEL_SLOW; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = LEFT_WHEEL_SLOW; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
}
void Forward_2(void) {
    RIGHT_FORWARD_SPEED = RIGHT_WHEEL2; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = LEFT_WHEEL2; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
}
void Forward_Left(void){
    RIGHT_FORWARD_SPEED = RIGHT_WHEEL_FORWARD_LEFT; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED =  WHEEL_OFF; // P6.2 Left Forward PWM ON amount
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


