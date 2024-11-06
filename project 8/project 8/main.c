//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
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


#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_StateMachine(void);

  // Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;
unsigned int wheel_move;
char forward;
extern unsigned int ready;
unsigned int white_val;
unsigned int black_val;
unsigned int scan;
unsigned int average;
extern unsigned int time;
extern char IOT_Ring_Rx[11];
extern char iot_TX_buf[11];
extern unsigned int iot_tx;
extern unsigned int iot_rx_wr;
char process_buf[11];
unsigned int clear;

void main(void){

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
  PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings
  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
  Init_ADC();
  Init_Serial_UCA1();
// Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], "  Waiting ");
  strcpy(display_line[1], "          ");
  strcpy(display_line[2], "  115,200 ");
  strcpy(display_line[3], "          ");
  display_changed = TRUE;
//  Display_Update(0,0,0,0);

  wheel_move = 0;
  forward = TRUE;
  event = NONE;
  state = WAIT;
  choice = NONE;
  instruction = 0;
  sw1_position = 0;
  sw2_position = 0;
  scan = 0;
  time = 0;
  clear = 1;
//------------------------------------------------------------------------------
// Beginning of the "While" Operating System
//------------------------------------------------------------------------------


  while(ALWAYS) {                       // Can the Operating system run
//      if(Last_Time_Sequence != Time_Sequence){
//          Last_Time_Sequence = Time_Sequence;
//          cycle_time++;
//          time_change = 1;
//       }

//         switch(event){
//             case STRAIGHT: // Straight
//                 Run_Straight();
//                 break; //
//             case TIMER:
//                 Run_Timer();
//                 break;
//             case CIRCLE: // Circle
//                 Run_Circle();
//                 break; //
//             case TRIANGLE:
//                 Run_Triangle();
//                 break;
//             case FIGURE:
//                 Run_Figure();
//             default: break;
//       }
//       Carlson_StateMachine();            // Run a Time Based State Machine
//       Switches_Process();                // Check for switch state change
         Display_Process();                 // Update Display
         P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF

         if (iot_tx != iot_rx_wr) {
             if (IOT_Ring_Rx[iot_rx_wr] == '\n') {
                 UCA0IE &= ~UCTXIE;
                 strcpy(display_line[0], " Received ");
                 strcpy(display_line[3], process_buf);
             }
         }
         else {
             process_buf[iot_rx_wr] = IOT_Ring_Rx[iot_rx_wr];
         }

         if (sw1_position == 1){
             if (IOT_Ring_Rx[iot_rx_wr] == '\n') {
                 UCA0IE &= ~UCTXIE;
                 iot_tx = 0;
             }
             else {
                 strcpy(display_line[0], " Transmit ");
                 strcpy(display_line[1], iot_TX_buf);
                 strcpy(display_line[2], "          ");
                 strcpy(display_line[3], "          ");

                 display_changed = TRUE;
             }


             }
         if (sw2_position == 1){
             UCA1BRW = 17;
             UCA1MCTLW = 0x4A00;

             strcpy(display_line[0], "          ");
             strcpy(display_line[1], "          ");
             strcpy(display_line[2], "  460,800 ");
             strcpy(display_line[3], "          ");

             display_changed = TRUE;

         }

         if (clear) {
             process_buf[0] = ' ';
             process_buf[1] = ' ';
             process_buf[2] = ' ';
             process_buf[3] = ' ';
             process_buf[4] = ' ';
             process_buf[5] = ' ';
             process_buf[6] = ' ';
             process_buf[7] = ' ';
             process_buf[8] = ' ';
             process_buf[9] = ' ';
             process_buf[10] = '\0';

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

             clear = 0;
         }

  }


//------------------------------------------------------------------------------




}


