/*
 * movement.h
 *
 *  Created on: Sep 29, 2024
 *      Author: anish
 */

#define NONE ('N')
#define STRAIGHT ('L')
#define CIRCLE ('C')
#define TRIANGLE ('T')
#define FIGURE ('F')
#define TIMER ('P')
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')
#define PROJ6 ('6')
#define WHEEL_COUNT_TIME (20)
#define RIGHT_COUNT_TIME (7)
#define LEFT_COUNT_TIME (8)
#define RIGHT_COUNT_TIME_CIRCLE (2)
#define LEFT_COUNT_TIME_CIRCLE (10)
#define RIGHT_COUNT_TIME_TRIANGLE (2)
#define LEFT_COUNT_TIME_TRIANGLE (2)
#define RIGHT_COUNT_TIME_TRIANGLE_STRAIGHT (11)
#define LEFT_COUNT_TIME_TRIANGLE_STRAIGHT (10)
#define RIGHT_COUNT_TIME_TRIANGLE_TURN (5)
#define RIGHT_COUNT_TIME_FIGURE (2)
#define LEFT_COUNT_TIME_FIGURE (18)
#define TRAVEL_DISTANCE (30)
#define TRAVEL_DISTANCE_CIRCLE (70)
#define TRAVEL_DISTANCE_TRIANGLE (6)
#define TRAVEL_DISTANCE_FIGURE (23)
#define WAITING2START (20)

unsigned int Last_Time_Sequence;
unsigned int cycle_time;
volatile unsigned int time_change;
char event;
char state;
unsigned int left_motor_count;
unsigned int right_motor_count;
unsigned int segment_count;
unsigned int delay_start;
unsigned int repeat;
unsigned int count;
unsigned int instruction;



void Forward_OFF(void);
void Forward_ON(void);
void Reverse_ON(void);
void Forward_Left(void);
void Turn(void);
void Forward_Right(void);
void Turn_CW(void);
void Turn_CCW(void);
void Wheels_OFF(void);
void Spin_CW();
void Spin_CCW();

