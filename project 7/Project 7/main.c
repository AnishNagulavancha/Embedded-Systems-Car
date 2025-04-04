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
extern unsigned int wait;
unsigned int p6;
unsigned int center;
unsigned int x;
unsigned int y;
unsigned int z;


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
  Init_DAC();
// Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], "          ");
  strcpy(display_line[1], "          ");
  strcpy(display_line[2], "          ");
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
  wait = 0;
  center = 0;
  x = 140;
  y = 450;
  z = 250;
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

         HEXtoBCD(wait);
         adc_line(1,3);
         HEXtoBCD(ADC_Left_Det);
         adc_line(2,3);
         HEXtoBCD(ADC_Right_Det);
         adc_line(3,3);

         center = ((ADC_Left_Det + ADC_Right_Det)/2);

         if (sw1_position == 1){
             sw1_position = 0;
             switch(scan) {
             case 0:
                 strcpy(display_line[3],"SCANWHITE");
                 white_val = ADC_Left_Det;
                 update_display = 1;
                 scan = 1;
             break;
             case 1:
                 strcpy(display_line[3],"SCANBLACK");
                 black_val = ADC_Left_Det;
                 update_display = 1;
                 scan = 2;
             break;
             case 2:
                 strcpy(display_line[3],"AVERAGE  ");
                 average = ((white_val + black_val) >> 1);
                 update_display = 1;
                 scan = 3;
                 break;
            default: break;
             }

         }
         if (sw2_position == 1){
             strcpy(display_line[3]," Project 7 ");
                 switch (p6) {
                     case 0:
                         if (ADC_Right_Det < x) {
                             Forward_ON();
                             strcpy(display_line[3], "  forward ");
                             display_changed = 1;
                         } else {
                             Wheels_OFF();
                             wait = 0;
                             p6 = 1;
                             strcpy(display_line[3], " stop0 ");
                             display_changed = 1;
                         }
                         break;

                     case 1:
                         if (wait >= 10) {
                             wait = 0;
                             p6 = 2;
                             Wheels_OFF();
                             strcpy(display_line[3], "  stop1 ");
                             display_changed = 1;
                         }
                         break;
                     case 2:
                         if (ADC_Right_Det < y && ADC_Left_Det < y) {
                             Turn();
                             strcpy(display_line[3], " turn ");
                             display_changed = 1;
                         } else {
                             Wheels_OFF();
                             p6 = 3;
                             wait = 0;
                             strcpy(display_line[3], " turn ");
                             display_changed = 1;

                         }
                         break;
                     case 3:
                         if (wait >= 10) {
                             Wheels_OFF();
                             wait = 0;
                             p6 = 4;
                             strcpy(display_line[3], "  stop3 ");
                             display_changed = 1;
                         }
                         break;

                     case 4:
                         if (wait < 2000) {
                             if (ADC_Left_Det < z && ADC_Right_Det < z) {
                                 Reverse_ON();
                                 strcpy(display_line[3], "Stcircle");
                                 display_changed = 1;
                             }
                             else if (ADC_Right_Det < center && ADC_Left_Det > center) {
                                 strcpy(display_line[3], "Rigcircle");
                                 display_changed = 1;
                                 Forward_Left();
                             }
                             else if (ADC_Left_Det < center && ADC_Right_Det > center) {
                                 strcpy(display_line[3], "Lefcircle");
                                 display_changed = 1;
                                 Forward_Right();
                             }

                         }
                         else {
                             Wheels_OFF();
                             strcpy(display_line[3], "  stop4 ");
                             display_changed = 1;
                             wait = 0;
                             p6 = 5;
                         }
                         break;
                     case 5:
                         if (wait >= 15) {
                             Wheels_OFF();
                             strcpy(display_line[3], "  stop5 ");
                             display_changed = 1;
                             wait = 0;
                             p6 = 6;
                         }
                         break;

                     case 6:
                         if (wait < 4) {
                             Turn();
                             strcpy(display_line[3], "  finaltu ");
                             display_changed = 1;
                         } else {
                             Wheels_OFF();
                             strcpy(display_line[3], "  finaltu ");
                             display_changed = 1;
                             p6 = 7;
                             wait = 0;
                         }
                         break;

                     case 7:
                         if (wait >= 10) {
                             Wheels_OFF();
                             strcpy(display_line[3], "  stop3 ");
                             display_changed = 1;
                             wait = 0;
                             p6 = 8;
                         }
                         break;
                     case 8:
                         if (wait < 42) {
                             Forward_ON();
                             strcpy(display_line[3], "  final ");
                             display_changed = 1;
                         } else {
                             Wheels_OFF();
                             strcpy(display_line[3], "  done ");
                             display_changed = 1;
                         }
                         break;
                     default:break;
                 }

             }
             update_display = 1;
         }

  }


//------------------------------------------------------------------------------






