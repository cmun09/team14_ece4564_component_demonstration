#include <ti/drivers/Capture.h>
#include <ti/display/Display.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <stddef.h>

/* Example/Board Header files */
#include "Board.h"

/* Callback used for blinking LED on timer completion */
void encoder1CaptureCallback(Capture_Handle handle, uint32_t interval);
void encoder2CaptureCallback(Capture_Handle handle, uint32_t interval);

/* Local Variables */
static Display_Handle display;
volatile uint32_t encoder1Int;
volatile uint32_t encoder2Int;

volatile uint32_t encoder1IntPrev = 0;
volatile uint32_t encoder2IntPrev = 0;




/*
 *  ======== mainThread ========
 *  Task that will capture two rising edges and output the time between the
 *  two edges
 */
void *encoderReadThread(void *arg0){

    Capture_Params encoder1Params;
    Capture_Params encoder2Params;

    Capture_Handle encoder1;
    Capture_Handle encoder2;

    /* Driver Init Functions */
    Capture_init();
    Display_init();

    /* Open Display for Output */
    display = Display_open(Display_Type_UART, NULL);

    if (display == NULL)
    {
        /* Failed to open display driver */
        while (1);
    }

    /* Setting up the Capture driver to detect two rising edges and report
     * the result in microseconds
     */
    Capture_Params_init(&encoder1Params);
    encoder1Params.mode = Capture_RISING_EDGE;
    encoder1Params.periodUnit = Capture_PERIOD_HZ;
    encoder1Params.callbackFxn = encoder1CaptureCallback;

    Capture_Params_init(&encoder2Params);
    encoder2Params.mode = Capture_RISING_EDGE;
    encoder2Params.periodUnit = Capture_PERIOD_HZ;
    encoder2Params.callbackFxn = encoder2CaptureCallback;



    encoder1 = Capture_open(Board_CAPTURE0, &encoder1Params); // check sysconfig for the pin number p5
    encoder2 = Capture_open(Board_CAPTURE1, &encoder2Params); // check sysconfig for the pin number p6

    if (encoder1 == NULL || encoder2 == NULL)
    {
        Display_printf(display, 0, 0, "Failed to initialized Capture!\n");
        while(1);
    }

    Display_printf(display, 0, 0, "About to Capture!\n");
    Capture_start(encoder1);
    Capture_start(encoder2);

    volatile uint8_t count1 = 0;
    volatile uint8_t count2 = 0;
    while(1){
        encoder1IntPrev = encoder1Int;
        encoder2IntPrev = encoder2Int;

        /* The value printed should be close to the period of the pwm */
        Display_printf(display, 0, 0, "E1_P %d  E2_P %d\n", encoder1Int, encoder2Int);\
        uint32_t asdf = encoder1Int;
        uint32_t asdf2 = encoder1IntPrev;
        uint32_t asdf1 = encoder2Int;
        uint32_t asdf12 = encoder2IntPrev;
        if(encoder1IntPrev < 80){
            if(count1 > 1){
                count1 = 0;
                encoder1Int = 0;
            }
            if(encoder1IntPrev == encoder1Int){
                count1++;
            }
        }
        if(encoder2IntPrev < 80){
            if(count2 > 1){
                count2 = 0;
                encoder2Int = 0;
            }
            if(encoder2IntPrev == encoder2Int){
                count2++;
            }
        }

    }
}

    /* Callback function that displays the interval */
void encoder1CaptureCallback(Capture_Handle handle, uint32_t interval){
    encoder1Int = interval;
}

void encoder2CaptureCallback(Capture_Handle handle, uint32_t interval){
    encoder2Int = interval;

}
