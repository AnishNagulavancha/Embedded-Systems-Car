/*
 * command.c
 *
 *  Created on: Nov 12, 2024
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
#include "timerB0.h"
#include "hextobcd.h"
#include "adc.h"
#include "eusci.h"

void init_commands(void);

extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern char IOT_Ring_Rx[16];
extern char iot_TX_buf[16];
extern unsigned int iot_tx;
extern unsigned int iot_rx_wr;
char process_buf[16];
unsigned int read_ptr;
unsigned int buf_ptr;

//read_ptr = 0;
//buf_ptr = 0;
//UCA0IE |= UCRXIE;




