/*
 * Description: This file contains led.c and allows for the LEDs to either turn on or off.
 *
 *  Created on: Sep 18, 2024
 *      Author: anish
 */

#include  "msp430.h"
#include  "LCD.h"
#include  "macros.h"


void Init_LEDs(void){
//------------------------------------------------------------------------------
// LED Configurations
//------------------------------------------------------------------------------
// Turns on both LEDs
  P1OUT &= ~RED_LED;
  P6OUT &= ~GRN_LED;
//------------------------------------------------------------------------------
}
