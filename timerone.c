/*
 * timerone.c
   Author: Tori Guy
 * Created on: 9/10/19
 */


#include "timerone.h"
#include "sensor_queue.h"
#include "Board.h"
#include "debug.h"


void *timerOneThread()
{
    Timer_Handle timer1;
    Timer_Params params;

    /* Call driver init functions */

    /* Setting up the timer in continuous callback mode that calls the callback
     * function every 250000 microseconds, or .25 second.
     */
    Timer_Params_init(&params);
    params.period = 250000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerQuarterCallback;

    timer1 = Timer_open(Board_TIMER0, &params);
    if (timer1 == NULL) {
        /* Failed to initialized timer */
        while (1) {}
    }

    if (Timer_start(timer1) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) {}
    }

    return (NULL);
}
void timerQuarterCallback(Timer_Handle myHandle)
{
    dbgOutputLoc(ENTERING_ISR);
    int curTime = Timer_getCount(myHandle);

    dbgOutputLoc(BEFORE_SENDING_TO_QUEUE_ISR);
    sendTimeMsgToQ1FromISR(curTime, 69);
    dbgOutputLoc(AFTER_SENDING_TO_QUEUE_ISR);

    /*
     static uint32_t baseTicks = 0;
    if (timerStart == 0)
    {
        uint32_t baseTicks = Timer_getCount(myHandle);
        timerStart = baseTicks;
    }
    uint32_t timerElapsed = Timer_getCount(myHandle);
    unsigned int timeChange = (timerElapsed-timerStart)/baseTicks;
    unsigned int finalTime = 250000*timeChange;
    timerStart=timerElapsed;
     *
     */
    dbgOutputLoc(LEAVING_ISR);
}
