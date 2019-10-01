/*
 * timerTWO.h
 * Author: Tori Guy
 * Created on: 9/10/19
 */
#ifndef TIMERTWO_H_
#define TIMERTWO_H_

#include <stddef.h>
#include "sensor_queue.h"
#include "Board.h"

//Driver Header files
#include <ti/drivers/Timer.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/ADC.h>
/* Callback used for toggling the LED. */
void timer55Callback(Timer_Handle myHandle);
int sensorReadingmm(int sensorRead);
void *timerTwoThread();

#endif
