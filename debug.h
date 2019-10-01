/*
 * debug.h
 *
 *  Created on: Sep 9, 2019
 *      Author: Phillip Sullivan
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#define ENTERING_A_TASK 0
#define BEFORE_WHILE 1
#define BEFORE_SENDING_TO_QUEUE 2
#define AFTER_SENDING_TO_QUEUE 3
#define BEFORE_RECIEVING_FROM_QUEUE 4
#define AFTER_RECIEVING_FROM_QUEUE 5
#define ENTERING_ISR 6
#define LEAVING_ISR 7
#define BEFORE_SENDING_TO_QUEUE_ISR 8
#define AFTER_SENDING_TO_QUEUE_ISR 9
#define BEFORE_RECIEVING_FROM_QUEUE_ISR 10
#define AFTER_RECIEVING_FROM_QUEUE_ISR 11
#define STATE_INIT 16
#define STATE_ONE 17
#define STATE_TWO 18
#define STATE_THREE 19
#define STATE_FOUR 20


void configDebugPins();
void* dbgUARTVal(unsigned char outVal);
void* dbgOutputLoc(unsigned int outLoc);
void failure(unsigned int num);


#endif /* DEBUG_H_ */
