/*
 * timerone.h
 * Author: Tori Guy
 * Created on: 9/10/19
 */
#ifndef TIMERONE_H_
#define TIMERONE_H_

#include <stddef.h>
#include "Board.h"
#include "sensor_queue.h"
//Driver Header files
#include <ti/drivers/Timer.h>

/* Callback used for toggling the LED. */
void timerQuarterCallback(Timer_Handle myHandle);
void *timerOneThread();


#endif
