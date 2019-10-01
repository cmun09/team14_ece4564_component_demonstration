#include <ti/drivers/GPIO.h>

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <pthread.h>

#include "sensor_state.h"
#include "Board.h"
#include "sensor_queue.h"
#include "debug.h"

enum state{Init, WaitingForTime1, WaitingForTime2, WaitingForTime3, WaitingForTime4};


struct state_var{
    int curTime;
    int sensorTotal;
    int sensorCount;
    int sensorAvg;
    enum state cur_state;
};

int state_machine(struct state_var* stat, int* timeInc, int* sensorVal){

        switch(stat->cur_state){
            case Init:
                dbgOutputLoc(STATE_ONE); // debug message
                stat->curTime = 0;
                stat-> sensorTotal = -1;
                stat->sensorCount = 0;
                stat-> sensorAvg = -1;
                stat->cur_state = WaitingForTime1;
                break;
            case WaitingForTime1:
                dbgOutputLoc(STATE_ONE);
                if(*timeInc == 0){
                    stat->sensorTotal += *sensorVal;
                    stat->sensorCount++;
                }
                else{
                    stat->curTime += *timeInc;
                    stat->sensorAvg = stat->sensorTotal/ stat->sensorCount;

                    char output[20];
                    sprintf(output, "Sensor= %d - %d\n", stat->sensorAvg, stat->sensorCount);
                    int x;
                    for(x=0; x<20; x++){
                        dbgUARTVal(output[x]);
                    }
                    // TODO: UART print the string "Sensor= 'sensorAvg - 'sensorCount"
                    stat->sensorTotal = 0;
                    stat->sensorCount = 0;
                    stat->cur_state = WaitingForTime2;
                }
                break;
            
            case WaitingForTime2:
                dbgOutputLoc(STATE_TWO);

                if(*timeInc == 0){
                    stat->sensorTotal += *sensorVal;
                    stat->sensorCount++;
                }
                else{
                    stat->curTime += *timeInc;
                    stat->sensorAvg = stat->sensorTotal/ stat->sensorCount;
                    char output[20];
                    sprintf(output, "Sensor= %d - %d\n", stat->sensorAvg, stat->sensorCount);
                    int x;
                    for(x=0; x<20; x++){
                        dbgUARTVal(output[x]);
                    }
                    stat->sensorTotal = 0;
                    stat->sensorCount = 0;
                    stat->cur_state = WaitingForTime3;
                }
                break; 
            
            case WaitingForTime3:
                dbgOutputLoc(STATE_THREE);

                if(*timeInc == 0){
                    stat->sensorTotal += *sensorVal;
                    stat->sensorCount++;
                }
                else{
                    stat->curTime += *timeInc;
                    stat->sensorAvg = stat->sensorTotal/ stat->sensorCount;
                    // TODO: UART print the string "Sensor= 'sensorAvg - 'sensorCount"
                    char output[20];
                    sprintf(output, "Sensor= %d - %d\n", stat->sensorAvg, stat->sensorCount);
                    int x;
                    for(x=0; x<20; x++){
                        dbgUARTVal(output[x]);
                    }
                    stat->sensorTotal = 0;
                    stat->sensorCount = 0;
                    stat->cur_state = WaitingForTime4; 
                }
                break;
            
            case WaitingForTime4:
                dbgOutputLoc(STATE_FOUR);
                if(*timeInc == 0){
                    stat->sensorTotal += *sensorVal;
                    stat->sensorCount++;
                }
                else{
                    stat->curTime += *timeInc;
                    stat->sensorAvg = stat->sensorTotal/stat->sensorCount;
                    char output[20];
                    char output2[20];
                    sprintf(output, "Sensor= %d - %d\n", stat->sensorAvg, stat->sensorCount);
                    sprintf(output2, "CurTime= %d (s)\n", stat->curTime);
                    int x;
                    for(x=0; x < 20; x++){
                        dbgUARTVal(output[x]);
                    }
                    for(x=0; x < 20; x++){
                        dbgUARTVal(output2[x]);
                    }
                    stat->sensorTotal = 0;
                    stat->sensorCount = 0;
                    stat->cur_state = WaitingForTime1;
                }
                break;
            default:
                // TODO: Call error handling
                return 0;
    }
    return 1;
}

void *sesnorStateThread(void *arg0){
    int detachedState;
	GPIO_init();
    struct state_var current_status;
    current_status.cur_state = Init;
    pthread_attr_t      attrs;

    pthread_attr_init(&attrs); // create application threads

	// configure stuff here and the ints
    createQueue();

    dbgOutputLoc(BEFORE_WHILE);
	while(1){
        // TODO: Get the sensor data in here some how into two integers
	    int type;
	    int data;
	    int sensorVal;
	    int timeInc;

	    //dbgOutputLoc(BEFORE_RECIEVING_FROM_QUEUE);
	    int success = readMsgFromQ1(&type, &data);
	    //dbgOutputLoc(AFTER_RECIEVING_FROM_QUEUE);

	    if(type){
	        sensorVal = data;
	        timeInc = 0;
	    }
	    else{

	        timeInc = data;
	        sensorVal = -1; // This should be an invalid sensor data
	    }
	    if(success){
	        if(type && sensorVal != -1){
	            state_machine(&current_status, &timeInc, &sensorVal);
	        }
	    }
    }
}
