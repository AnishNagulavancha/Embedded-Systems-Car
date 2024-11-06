/*
 * timerB0.h
 *
 *  Created on: Oct 1, 2024
 *      Author: anish
 *
 */

#ifndef TIMERB0_H_
#define TIMERB0_H_

#define DIVS (0x0030)
#define DIVS_L (0x0030)
#define DIVS0 (0x0010)
#define DIVS0_L (0x0010)
#define DIVS1 (0x0020)
#define DIVS1_L (0x0020)
#define DIVS_0 (0x0000) // /1
#define DIVS_1 (0x0010) // /2
#define DIVS_1_L (0x0010)
#define DIVS_2 (0x0020) // /4
#define DIVS_2_L (0x0020)
#define DIVS_3 (0x0030) // /8
#define DIVS_3_L (0x0030)
#define DIVS__1 (0x0000) // /1
#define DIVS__2 (0x0010) // /2
#define DIVS__2_L (0x0010)
#define DIVS__4 (0x0020) // /4
#define DIVS__4_L (0x0020)
#define DIVS__8 (0x0030) // /8
#define DIVS__8_L (0x0030)


#define TB0CCR0_INTERVAL (10000)
#define TB0CCR1_INTERVAL (50000)
#define TB0CCR2_INTERVAL (50000)
#define SW1_DEBOUNCE_TIME (50000)
#define SW2_DEBOUNCE_TIME (50000)

unsigned int Time_Sequence;



#endif /* TIMERB0_H_ */
