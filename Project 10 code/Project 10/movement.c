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
unsigned int center;
unsigned int x;
unsigned int y;
unsigned int z;
unsigned int start;


void project_7(void) {
    start = 1;
    x = 175;
    y = 500;
    z = 250;
    center = ((ADC_Left_Det + ADC_Right_Det)>>2);
    switch (p6) {
    case 0:
        wait = 0;
        p6 = 1;
    case 1:
            if (wait >= 335 && (ADC_Right_Det < x)) {
                strcpy(display_line[3], "BL Start");
                display_changed = 1;
                wait = 0;
                p6 = 2;
                }
            else {
               ARC();
               strcpy(display_line[3], " BL Start");
               display_changed = 1;
                    }
        break;

    case 2:
        if (ADC_Right_Det < x) {
            Forward_2();
            strcpy(display_line[3], "BLStart1");
            display_changed = 1;
        }
        else {
            wait = 0;
            p6 = 3;
            Wheels_OFF();
            strcpy(display_line[3], "BLStart2");
            display_changed = 1;
        }
        break;
    case 3:
        if (wait <= 300) {
            Wheels_OFF();
            strcpy(display_line[3], "  Intercept ");
            display_changed = 1;
        }
        else {
            p6 = 4;
            wait = 0;
        }
        break;

    case 4:
        if (wait <= 15) {
            Turn();
            strcpy(display_line[3], " BL Turn ");
            display_changed = 1;
        } else {
            Wheels_OFF();
            p6 = 5;
            wait = 0;
            strcpy(display_line[3], " BL Turn ");
            display_changed = 1;

        }
        break;
    case 5:
        if (wait <= 300) {
            Wheels_OFF();
            strcpy(display_line[3], " check1 ");
            display_changed = 1;
        }
        else {
            wait = 0;
            p6 = 6;
            strcpy(display_line[3], "  check2 ");
            display_changed = 1;

        }
        break;

    case 6:
        if (wait < 1000) {
            if (ADC_Left_Det < z && ADC_Right_Det < z) {
                Reverse_2();
                strcpy(display_line[3], " BL Travel");
                display_changed = 1;
            }
            else if (ADC_Right_Det < center && ADC_Left_Det > center) {
                strcpy(display_line[3], " BL Travel");
                display_changed = 1;
                Forward_Left();
            }
            else if (ADC_Left_Det < center && ADC_Right_Det > center) {
                strcpy(display_line[3], " BL Travel");
                display_changed = 1;
                Forward_Right();
            }
        }
        else {
           wait = 0;
           p6 = 7;
           Wheels_OFF();
           strcpy(display_line[3], " BL Circle");
        }
           break;
    case 7:
            if (ADC_Left_Det < z && ADC_Right_Det < z) {
                Reverse_2();
                strcpy(display_line[3], " BL Circle");
                display_changed = 1;
            }
            if (ADC_Right_Det < center && ADC_Left_Det > center) {
                strcpy(display_line[3], " BL Circle");
                display_changed = 1;
                Forward_Left();
            }
            else if (ADC_Left_Det < center && ADC_Right_Det > center) {
                strcpy(display_line[3], " BL Circle");
                display_changed = 1;
                Forward_Right();
            }

        break;
    default: break;
}
}

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
    RIGHT_REVERSE_SPEED = RIGHT_WHEEL_SLOW; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = LEFT_WHEEL_SLOW; // P6.4 Left Reverse PWM OFF
}
void Turn(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = LEFT_WHEEL_TURN; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = RIGHT_WHEEL_TURN; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
}
void Turn_CW(void){
    RIGHT_FORWARD_SPEED = RIGHT_WHEEL_CW; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
}
void Turn_CCW(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = LEFT_WHEEL_CCW; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF

}
void Forward_2(void) {
    RIGHT_FORWARD_SPEED = RIGHT_WHEEL2; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = LEFT_WHEEL2; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
}
void ARC(void) {
    RIGHT_FORWARD_SPEED = RIGHT_WHEEL_ARC; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = LEFT_WHEEL_ARC; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF
}
void Reverse_2(void) {
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM ON amount
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM ON amount
    RIGHT_REVERSE_SPEED = RIGHT_WHEEL_REVERSE; // P6.3 Right Reverse PWM OFF
    LEFT_REVERSE_SPEED = LEFT_WHEEL_REVERSE; // P6.4 Left Reverse PWM OFF
}



