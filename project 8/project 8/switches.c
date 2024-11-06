/*
 * switches.c
 *
 *  Created on: Sep 29, 2024
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
#include "eusci.h"

extern char display_line[4][11];
extern char display_changed;
extern volatile unsigned int update_display;
extern int backlight;
extern char IOT_Ring_Rx[11];
extern unsigned int iot_rx_wr;

    #pragma vector=PORT4_VECTOR
    __interrupt void switchP4_interrupt(void){
        // Switch 1
        if (P4IFG & SW1) {
            P4IFG &= ~SW1; // IFG SW1 cleared
            P4IE &= ~SW1;
            IOT_Ring_Rx[0] = ' ';
            IOT_Ring_Rx[1] = ' ';
            IOT_Ring_Rx[2] = ' ';
            IOT_Ring_Rx[3] = ' ';
            IOT_Ring_Rx[4] = ' ';
            IOT_Ring_Rx[5] = ' ';
            IOT_Ring_Rx[6] = ' ';
            IOT_Ring_Rx[7] = ' ';
            IOT_Ring_Rx[8] = ' ';
            IOT_Ring_Rx[9] = ' ';
            IOT_Ring_Rx[10] = '\0';
            iot_rx_wr = 0;
            TB0CCR1 = TB0R + SW1_DEBOUNCE_TIME;
            TB0CCTL1 |= CCIE;
            TB0CCTL0 &= ~CCIE;
            sw1_position = 1;

            count_debounce_SW1 = 0;
        }
    }
    #pragma vector=PORT2_VECTOR
    __interrupt void switchP2_interrupt(void){
        // Switch 2
        if (P2IFG & SW2) {
            P2IFG &= ~SW2; // IFG SW2 cleared
            P2IE &= ~SW2;
            IOT_Ring_Rx[0] = ' ';
            IOT_Ring_Rx[1] = ' ';
            IOT_Ring_Rx[2] = ' ';
            IOT_Ring_Rx[3] = ' ';
            IOT_Ring_Rx[4] = ' ';
            IOT_Ring_Rx[5] = ' ';
            IOT_Ring_Rx[6] = ' ';
            IOT_Ring_Rx[7] = ' ';
            IOT_Ring_Rx[8] = ' ';
            IOT_Ring_Rx[9] = ' ';
            IOT_Ring_Rx[10] = '\0';
            iot_rx_wr = 0;
            TB0CCR2 = TB0R + SW2_DEBOUNCE_TIME;
            TB0CCTL2 |= CCIE;
            TB0CCTL0 &= ~CCIE;
            sw2_position = 1;
            count_debounce_SW2 = 0;
        }
    }


////------------------------------------------------------------------------------




