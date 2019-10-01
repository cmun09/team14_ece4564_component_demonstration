/*
 * sensor_queue.c
 *
 *  Created on: Sep 9, 2019
 *      Author: grantr6
 */

#include "sensor_queue.h"

QueueHandle_t xQueue1 = 0;

struct data_packet{
    int data_type;
    int data;
    int sender;
};

void createQueue() {
    xQueue1 = xQueueCreate(10, sizeof(struct data_packet));
}


int sendTimeMsgToQ1FromISR(int timeVal, int sender){

    if (xQueue1 == 0) {
        createQueue();
    }
    struct data_packet data = {0, timeVal, sender};
    BaseType_t success = xQueueSendFromISR(xQueue1, &data, 0);

    if (success == pdTRUE) {
        return 1;
    }
    else {
        return 0;
    }
}
int sendTimeMsgToQ1(int timeVal){

    if (xQueue1 == 0) {
        createQueue();
    }
    struct data_packet data = {0, timeVal};
    BaseType_t success = xQueueSend(xQueue1, &data, 0);

    if (success == pdTRUE) {
        return 1;
    }
    else {
        return 0;
    }
}


int sendSensorMsgToQ1(int mmDist){
    if (xQueue1 == 0) {
        createQueue();
    }
    struct data_packet data = {1, mmDist};
    BaseType_t success = xQueueSend(xQueue1, &data, 0);

    if (success == pdTRUE) {
        return 1;
    }
    else {
        return 0;
    }
}

int sendSensorMsgToQ1FromISR(int mmDist, int sender){
    if (xQueue1 == 0) {
        createQueue();
    }
    struct data_packet data = {1, mmDist, sender};
    BaseType_t success = xQueueSendFromISR(xQueue1, &data, 0);

    if (success == pdTRUE) {
        return 1;
    }
    else {
        return 0;
    }
}

int readMsgFromQ1(int* type, int* value) {
    if (xQueue1 == 0) {
        return 0;
    }

    struct data_packet return_data;

    BaseType_t success = xQueueReceive(xQueue1, &return_data, 0);

    if (success == pdFALSE) {
        *type = -1;
        *value = -1;
        return 0;
    }
    else{
        *type = return_data.data_type;
        *value = return_data.data;
        return 1;
    }

}

