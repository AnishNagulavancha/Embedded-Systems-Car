/*
 * eusci.c
 *
 *  Created on: Oct 29, 2024
 *      Author: anish
 */


#include  "msp430.h"
#include  <string.h>
#include  "LCD.h"
#include  "ports.h"
#include "switches.h"
#include "timerB0.h"
#include "adc.h"
#include "eusci.h"

char IOT_Ring_Rx[16];
char iot_TX_buf[16];
unsigned int iot_tx;
unsigned int iot_rx_wr;
char usb_Ring_Rx[16];
char usb_TX_buf[16];
unsigned int usb_tx;
unsigned int usb_rx_wr;
extern int Framcmd;


void Init_Serial_UCA0(char speed){
//------------------------------------------------------------------------------
// TX error (%) RX error (%)
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
// 8000000 4800 1 104 2 0xD6 -0.08 0.04 -0.10 0.14
// 8000000 9600 1 52 1 0x49 -0.08 0.04 -0.10 0.14
// 8000000 19200 1 26 0 0xB6 -0.08 0.16 -0.28 0.20
// 8000000 57600 1 8 10 0xF7 -0.32 0.32 -1.00 0.36
// 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
// 8000000 460800 0 17 0 0x4A -2.72 2.56 -3.76 7.28
//------------------------------------------------------------------------------
// Configure eUSCI_A0 for UART mode
 UCA0CTLW0 = 0;
 UCA0CTLW0 |= UCSWRST ; // Put eUSCI in reset
 UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
 UCA0CTLW0 &= ~UCMSB; // MSB, LSB select
 UCA0CTLW0 &= ~UCSPB; // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
 UCA0CTLW0 &= ~UCPEN; // No Parity
 UCA0CTLW0 &= ~UCSYNC;
 UCA0CTLW0 &= ~UC7BIT;
 UCA0CTLW0 |= UCMODE_0;
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
// 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
// UCA?MCTLW = UCSx + UCFx + UCOS16
 UCA0BRW = 4 ; // 115,200 baud
 UCA0MCTLW = 0x5551 ;
 UCA0CTLW0 &= ~UCSWRST ; // release from reset
 UCA0TXBUF = 0x00; // Prime the Pump
 UCA0IE |= UCRXIE; // Enable RX interrupt
//------------------------------------------------------------------------------
}

void Init_Serial_UCA1(char speed){
//------------------------------------------------------------------------------
// TX error (%) RX error (%)
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
// 8000000 4800 1 104 2 0xD6 -0.08 0.04 -0.10 0.14
// 8000000 9600 1 52 1 0x49 -0.08 0.04 -0.10 0.14
// 8000000 19200 1 26 0 0xB6 -0.08 0.16 -0.28 0.20
// 8000000 57600 1 8 10 0xF7 -0.32 0.32 -1.00 0.36
// 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
// 8000000 460800 0 17 0 0x4A -2.72 2.56 -3.76 7.28
//------------------------------------------------------------------------------
// Configure eUSCI_A0 for UART mode
 UCA1CTLW0 = 0;
 UCA1CTLW0 |= UCSWRST ; // Put eUSCI in reset
 UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
 UCA1CTLW0 &= ~UCMSB; // MSB, LSB select
 UCA1CTLW0 &= ~UCSPB; // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
 UCA1CTLW0 &= ~UCPEN; // No Parity
 UCA1CTLW0 &= ~UCSYNC;
 UCA1CTLW0 &= ~UC7BIT;
 UCA1CTLW0 |= UCMODE_0;
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
// 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
// UCA?MCTLW = UCSx + UCFx + UCOS16
 UCA1BRW = 4 ; // 115,200 baud
 UCA1MCTLW = 0x5551 ;
 UCA1CTLW0 &= ~UCSWRST ; // release from reset
 UCA1TXBUF = 0x00; // Prime the Pump
 UCA1IE |= UCRXIE; // Enable RX interrupt
//------------------------------------------------------------------------------
}

#pragma vector = EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){ //This interrupt is the interrupt relating to serial communication port UCA0
 //------------------------------------------------------------------------------
 // Interrupt name: eUSCI_A0_ISR
 // Description: This interrupt transmits and receives through UCA0
 //------------------------------------------------------------------------------
 char usb_receive;
// int temp_char;
// int i;
 switch(__even_in_range(UCA1IV,0x08)){
 case 0: break; //Vector 0 - no interrupt
 case 2:{ // Vector 2 – Rx0IFG
 usb_receive = UCA1RXBUF;
 usb_Ring_Rx[usb_rx_wr++] = usb_receive; // Add to Ring Buffer
 UCA0TXBUF = usb_receive;
 if(usb_rx_wr >= sizeof(usb_Ring_Rx)){
 usb_rx_wr = BEGINNING;
 }

// UCA1IE |= UCTXIE;
 }break;
 case 4:{ // Vector 4 – Tx0IFG
     if(usb_TX_buf[usb_tx] == '\n') {
         UCA1TXBUF = usb_TX_buf[usb_tx];
         usb_TX_buf[usb_tx] = 0x00;
         UCA1IE &= ~UCTXIE;
         usb_tx = 0;
         break;
     }
     else {
         UCA1TXBUF = usb_TX_buf[usb_tx];
         usb_TX_buf[usb_tx] = 0x00;
     }
     usb_tx++;
 }break;
 default: break;
 }
}//------------------------------------------------------------------------------

#pragma vector = EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){ //This interrupt is the interrupt relating to serial communication port UCA0
 //------------------------------------------------------------------------------
 // Interrupt name: eUSCI_A0_ISR
 // Description: This interrupt transmits and receives through UCA0
 //------------------------------------------------------------------------------
 char iot_receive;
// int temp_char;
// int i;
 switch(__even_in_range(UCA0IV,0x08)){
 case 0: break; //Vector 0 - no interrupt
 case 2:{ // Vector 2 – Rx0IFG
 iot_receive = UCA0RXBUF;
 IOT_Ring_Rx[iot_rx_wr++] = iot_receive; // Add to Ring Buffer
 if(iot_rx_wr >= sizeof(IOT_Ring_Rx)){
 iot_rx_wr = BEGINNING;
 }
// UCA1IE |= UCTXIE;
 UCA1TXBUF = iot_receive;

 }break;
 case 4:{ // Vector 4 – Tx0IFG
if(iot_TX_buf[iot_tx] == '\n') {
    UCA0TXBUF = iot_TX_buf[iot_tx];
    iot_TX_buf[iot_tx] = 0x00;
    UCA0IE &= ~UCTXIE;
    iot_tx = 0;
    break;
}
else {
    UCA0TXBUF = iot_TX_buf[iot_tx];
    iot_TX_buf[iot_tx] = 0x00;
}
iot_tx++;

 }
 break;
 default: break;
 }
}//------------------------------------------------------------------------------




