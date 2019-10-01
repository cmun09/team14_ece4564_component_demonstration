/*
 * sensor_queue.h
 *
 *  Created on: Sep 9, 2019
 *      Author: grantr6
 */

#ifndef SENSOR_QUEUE_H_
#define SENSOR_QUEUE_H_

#include "FreeRTOS.h"
#include "queue.h"

int readMsgFromQ1(int* type, int* value);
int sendTimeMsgToQ1(int inttimeVal);
int sendSensorMsgToQ1(int mmDist);

int sendTimeMsgToQ1FromISR(int inttimeVal,int sender);
int sendSensorMsgToQ1FromISR(int mmDist,int sender);

#endif /* SENSOR_QUEUE_H_ */
