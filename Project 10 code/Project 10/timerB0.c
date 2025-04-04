/*
 * timerB0.c
 *
 *  Created on: Oct 1, 2024
 *      Author: anish
 */


#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"
#include "movement.h"
#include "switches.h"
#include "timerB0.h"
#include "pwm.h"

extern volatile unsigned char update_display;
extern volatile unsigned int time_change;
unsigned int time_update;
unsigned int hex;
unsigned int backlight;
unsigned int counter;
unsigned int wait;
unsigned int time;
unsigned int overflow_ctr;
extern unsigned int DAC_data;
extern unsigned int sw1_position;
extern unsigned int sw2_position;
unsigned int boot;


void Init_Timers(void) {
    Init_Timer_B0();
    Init_Timer_B3();

}

    //------------------------------------------------------------------------------
// Timer B0 initialization sets up both B0_0, B0_1-B0_2 and overflow
void Init_Timer_B0(void) {
    TB0CTL = TBSSEL__SMCLK; // SMCLK source
    TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
    TB0CTL |= MC__CONTINOUS; // Continuous up
    TB0CTL |= ID__2; // Divide clock by 2
    TB0EX0 |= TBIDEX__8; // Divide clock by an additional 8
    TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
    TB0CCTL0 |= CCIE; // CCR0 enable interrupt
    // TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
    // TB0CCTL1 |= CCIE; // CCR1 enable interrupt
    // TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
    // TB0CCTL2 |= CCIE; // CCR2 enable interrupt
    TB0CTL &= ~TBIE; // Disable Overflow Interrupt
    TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}

//------------------------------------------------------------------------------

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
//------------------------------------------------------------------------------
// TimerB0 0 Interrupt handler
//----------------------------------------------------------------------------
//...... Add What you need happen in the interrupt ......

    ADCCTL0 |= ADCSC;
    ADCCTL0 |= ADCENC;


    wait++;
    time++;
    boot++;

    counter++;
    if (counter >= 10) {
        update_display = 1;
        counter = 0;
    }
    if (DAC_data > 1200) {
        DAC_data = DAC_data - 200;
        SAC3DAT = DAC_data;
    }

    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0
}
//----------------------------------------------------------------------------

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
    switch(__even_in_range(TB0IV,14)){
    case 0: break;
    case 2:
        TB0CCTL1 &= ~CCIE;
        P4IFG &= ~SW1;
        P4IE |= SW1;
        TB0CCTL0 |= CCIE;
    break;
    case 4:
        TB0CCTL2 &= ~CCIE;
        P2IFG &= ~SW2;
        P2IE |= SW2;
        TB0CCTL0 |= CCIE;
    break;
    case 14: // overflow
    default: break;
    }
//----------------------------------------------------------------------------
}

void Init_Timer_B3(void) {
    //------------------------------------------------------------------------------
    // SMCLK source, up count mode, PWM Right Side
    // TB3.1 P6.0 LCD_BACKLITE
    // TB3.2 P6.1 R_FORWARD
    // TB3.3 P6.2 R_REVERSE
    // TB3.4 P6.3 L_FORWARD
    // TB3.5 P6.4 L_REVERSE
    //------------------------------------------------------------------------------
     TB3CTL = TBSSEL__SMCLK; // SMCLK
     TB3CTL |= MC__UP; // Up Mode
     TB3CTL |= TBCLR; // Clear TAR

     PWM_PERIOD = WHEEL_PERIOD; // PWM Period [Set this to 50005]

     TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
     LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM duty cycle

     TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
     RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle

     TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
     LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle

     TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
     RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle

     TB3CCTL5 = OUTMOD_7; // CCR5 reset/set
     LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
    //------------------------------------------------------------------------------
}





