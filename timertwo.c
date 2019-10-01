/*
 * timertwo.c
 * Author: Tori Guy
 * Created on: 9/10/19
 */


#include "timertwo.h"
#include "sensor_queue.h"
#include "Board.h"
#include "math.h"
#include "debug.h"


void *timerTwoThread()
{
    Timer_Handle timer2;
    Timer_Params params;


    /* Setting up the timer in continuous callback mode that calls the callback
     * function every 1,000,000 microseconds, or 1 second.
     */
    Timer_Params_init(&params);
    params.period = 55000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timer55Callback;
    timer2 = Timer_open(Board_TIMER1, &params);

    if (timer2 == NULL) {
        /* Failed to initialized timer */
        while (1) {}
    }

    if (Timer_start(timer2) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) {}
    }

    return (NULL);
}
void timer55Callback(Timer_Handle myHandle)
{

    dbgOutputLoc(ENTERING_ISR);
    ADC_init();
       // initialize optional ADC parameters
    ADC_Params params;
    ADC_Params_init(&params);
    params.isProtected = true;

    uint16_t result;

    // Open ADC channels for usage
    ADC_Handle adcHandle = ADC_open(Board_ADC0, &params);

    // Sample the analog output from the adc0
    ADC_convert(adcHandle, &result);

    if (adcHandle == NULL) {
            /* Failed to initialized timer */
            while (1) {}
        }
    if (adcHandle == ADC_STATUS_ERROR ) {
            /* The conversion failed*/
            while (1) {}
        }

    // Convert the sample to microvolts
    ADC_close(adcHandle);
    int finaldist = sensorReadingmm(result);

    if(finaldist ==-1)
    {
        while(1){}//failed reading
    }
    if(finaldist>=0)//reading is fairly accurate
    {
        dbgOutputLoc(BEFORE_SENDING_TO_QUEUE_ISR);
        sendSensorMsgToQ1FromISR(finaldist, 420);
        dbgOutputLoc(AFTER_SENDING_TO_QUEUE_ISR);
    }
    dbgOutputLoc(LEAVING_ISR);
}

int sensorReadingmm(int sensorRead){
    if (sensorRead > 1530){
        //I plugged data points into a curve simulator and this was the resulting formula
        return -46.51322 + 0.02052515 * sensorRead + 0.00002713143*pow(sensorRead, 2) - 9.692949 * pow(10,-9) * pow(sensorRead,3) * 100;
    }
    else{
        return -1;
    }
}
