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
extern unsigned int time;
extern char IOT_Ring_Rx[16];
extern char iot_TX_buf[16];
char nameIP[16];
char nameSSID[16];
unsigned int ip_ptr;
unsigned int ssid_ptr;
extern unsigned int iot_tx;
extern unsigned int iot_rx_wr;
char process_buf[16];
unsigned int read_ptr;
unsigned int buf_ptr;
unsigned int transmit;
unsigned int input;
unsigned int Framcmd;
extern char usb_Ring_Rx[16];
extern char usb_TX_buf[16];
extern unsigned int boot;
char state;
char execute1;
char execute2;
char execute3;
char execute4;
char execute5;
unsigned int stateIP;
unsigned int stateSSID;
unsigned int move;
char move_buf[3][16];
unsigned int move_ptr;
unsigned int move_ptr1;
unsigned int padnum;
unsigned int control;
unsigned int timex;



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
  Init_Serial_UCA1();
  Init_Serial_UCA0();
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
  time = 0;
  read_ptr = 0;
  buf_ptr = 0;
  transmit = 0;
  UCA0IE |= UCRXIE;
  boot = 0;
  state = NONE;
  execute1 = FALSE;
  execute2 = FALSE;
  execute3 = FALSE;
  execute4 = FALSE;
  execute5 = FALSE;
  stateIP = 0;
  stateSSID = 0;
  ip_ptr = 0;
  ssid_ptr = 0;
  move = 0;
  move_ptr = 0;
  padnum = 0;
  control = NONE;
  timex = 25;

//------------------------------------------------------------------------------
// Beginning of the "While" Operating System
//------------------------------------------------------------------------------


  while(ALWAYS) {                       // Can the Operating system run
         Display_Process();                 // Update Display
         P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF

         if(boot > 50) {
             P3OUT |= IOT_EN;
             P3OUT |= IOT_LINK_GRN;
         }

         if(boot > 400 && !execute1) {
             strcpy(iot_TX_buf, "AT+SYSSTORE=0\r\n");
             UCA0IE |= UCTXIE;
             execute1 = TRUE;
         }
         if (boot > 650 && !execute2) {
             strcpy(iot_TX_buf, "AT+CIPMUX=1\r\n");
             UCA0IE |= UCTXIE;
             execute2 = TRUE;
         }
         if (boot > 900 && !execute5) {
             strcpy(iot_TX_buf, "AT+CIPSERVER=1,8852\r\n");
             UCA0IE |= UCTXIE;
             execute5 = TRUE;
         }
         if (boot > 1150 && !execute3) {
             strcpy(iot_TX_buf, "AT+CWJAP?\r\n");
             state = SSID;
             UCA0IE |= UCTXIE;
             execute3 = TRUE;
         }
         if (state == SSID && boot > 1400 && !execute4) {
             strcpy(iot_TX_buf, "AT+CIFSR\r\n");
             state = IP;
             UCA0IE |= UCTXIE;
             execute4 = TRUE;
         }
         if (boot > 1650) {
             if (read_ptr != iot_rx_wr) {
                 if(!move && IOT_Ring_Rx[read_ptr++] == '^') {
                     move = 1;
                 }
                 else if(move == 1) {
                     if(IOT_Ring_Rx[read_ptr] != '\r'){
                     move_buf[move_ptr][move_ptr1++] = IOT_Ring_Rx[read_ptr++];
                 }
                     else {
                         move = 2;
                     }
                 }
             }

             if(read_ptr >= sizeof(IOT_Ring_Rx)){
                read_ptr = BEGINNING;
             }

             if (move == 2) {
                 if (move_buf[move_ptr][0] == '0' && move_buf[move_ptr][1] == '0' && move_buf[move_ptr][2] == '0' && move_buf[move_ptr][3] == '0') {
                     switch(move_buf[move_ptr][4]) {
                     case 'F':
                         Forward_ON();
                         strcpy(display_line[3], "F  ");
                         display_changed = TRUE;
                         control = 'F';
                         time = 0;
                         break;
                     case 'B':
                         Reverse_ON();
                         strcpy(display_line[3], "R  ");
                         display_changed = TRUE;
                         control = 'B';
                         time = 0;
                         break;
                     case 'L':
                         Turn_CCW();
                         strcpy(display_line[3], "TL  ");
                         display_changed = TRUE;
                         control = 'L';
                         time = 0;
                         break;
                     case 'R':
                         Turn_CW();
                         strcpy(display_line[3], "TR  ");
                         display_changed = TRUE;
                         control = 'R';
                         time = 0;
                         break;
                     case 'S':
                         Wheels_OFF();
                         strcpy(display_line[3], "S  ");
                         display_changed = TRUE;
                         break;
                     case 'X':
                         project_7();
                         strcpy(display_line[3], "P7  ");
                         display_changed = TRUE;
                         control = 'X';
                         break;
                     case 'C':
                         strcpy(display_line[0], "          ");
                         strcpy(display_line[1], "   ANISH  ");
                         strcpy(display_line[2], "   NAGU   ");
                         strcpy(display_line[3], "C  ");
                         display_changed = TRUE;
                         break;
                     case 'P':
                         strcpy(display_line[0], "ARRIVED 0");
                         padnum++;
                         display_line[0][9] = padnum + 0x30;
                         display_changed = TRUE;
                         control = 'P';
                         break;
                     case 'Y':
                         Forward_ON();
                         strcpy(display_line[3], " BL Exit ");
                         strcpy(display_line[3], "Y  ");
                         display_changed = TRUE;
                         control = 'Y';
                         time = 0;
                         break;
                     case 'Z':
                         control = 'Z';
                         break;
                     default:
                         Wheels_OFF();
                         break;
                     }
                 }
                 if (move_ptr++ >= 2) {
                     move_ptr = 0;
                 }
                 move_ptr1 = 0;
                 move = 0;
             }
         }

         switch(state) {
             case IP:
                 if (read_ptr != iot_rx_wr) {
                     if(!stateIP && IOT_Ring_Rx[read_ptr++] == '"') {
                         stateIP = 1;
                     }
                     else if(stateIP == 1) {
                         if(IOT_Ring_Rx[read_ptr] != '"'){
                         nameIP[ip_ptr++] = IOT_Ring_Rx[read_ptr++];
                     }
                         else {
                             stateIP = 2;
                         }
                     }
                 }

                 if(read_ptr >= sizeof(IOT_Ring_Rx)){
                    read_ptr = BEGINNING;
                 }

                 if (stateIP == 2) {
                     strcpy(display_line[0], " Waiting  ");
                     strcpy(display_line[1], "for input ");
                     strncpy(display_line[2],nameIP, 10);
                     display_line[3][4] = nameIP[10];
                     display_line[3][5] = nameIP[11];
                     display_line[3][6] = nameIP[12];
                     display_changed = TRUE;
                     state = NONE;
                 }
             break;
             default: break;
             case SSID:
                 if (read_ptr != iot_rx_wr) {
                     if(!stateSSID && IOT_Ring_Rx[read_ptr++] == '"') {
                         stateSSID = 1;
                     }
                     else if(stateSSID == 1) {
                         if(IOT_Ring_Rx[read_ptr] != '"'){
                         nameSSID[ssid_ptr++] = IOT_Ring_Rx[read_ptr++];
                     }
                         else {
                             stateSSID = 2;
                         }
                     }
                 }
                 if(read_ptr >= sizeof(IOT_Ring_Rx)){
                    read_ptr = BEGINNING;
                 }
                 if (stateSSID == 2) {
                     strncpy(display_line[0],nameSSID, 10);
                     display_changed = TRUE;
                 }
             break;

         }

         switch(control) {
         case 'F':
             if (time < timex) {
                 Forward_ON();
             }
             else {
                 Wheels_OFF();
                 control = NONE;
             }
             break;
         case 'B':
             if (time < timex) {
                 Reverse_ON();
             }
             else {
                 Wheels_OFF();
                 control = NONE;
             }
             break;
         case 'L':
             if (time < timex) {
                 Turn_CCW();
             }
             else {
                 Wheels_OFF();
                 control = NONE;
             }
             break;
         case 'R':
             if (time < timex) {
                 Turn_CW();
             }
             else {
                 Wheels_OFF();
                 control = NONE;
             }
             break;
         case 'X':
             project_7();

             break;
         case 'Y':
             if (time < (timex*4)) {
                 Forward_ON();
             }
             else {
                 Wheels_OFF();
                 strcpy(display_line[3], " BL Stop ");
                 display_changed = 1;
                 control = NONE;
             }
             break;
         case 'Z':
             HEXtoBCD(ADC_Left_Det);
             adc_line(2,3);
             HEXtoBCD(ADC_Right_Det);
             adc_line(3,3);
             break;
         default: break;
         }



  }



//------------------------------------------------------------------------------




}


