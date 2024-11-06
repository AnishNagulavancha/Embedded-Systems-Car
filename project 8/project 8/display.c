/*
 * Description:display.c makes the display turn on.
 *
 *  Created on: Sep 18, 2024
 *      Author: anish
 */

#include  "msp430.h"
#include  "LCD.h"
#include  "macros.h"

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}
