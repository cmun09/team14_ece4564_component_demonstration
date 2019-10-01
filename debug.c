/*
 * debug.c
 *
 *  Created on: Sep 9, 2019
 *      Author: Phillip Sullivan
 */

//for sleep
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

#include <ti/drivers/UART.h>
#include <ti/drivers/GPIO.h>
#include "Board.h"

void configDebugPins() {
    GPIO_setConfig (Debug_GPIO0, GPIO_CFG_OUTPUT);
    GPIO_setConfig (Debug_GPIO1, GPIO_CFG_OUTPUT);
    GPIO_setConfig (Debug_GPIO2, GPIO_CFG_OUTPUT);
    GPIO_setConfig (Debug_GPIO3, GPIO_CFG_OUTPUT);
    GPIO_setConfig (Debug_GPIO4, GPIO_CFG_OUTPUT);
    GPIO_setConfig (Debug_GPIO5, GPIO_CFG_OUTPUT);
    GPIO_setConfig (Debug_GPIO6, GPIO_CFG_OUTPUT);
    GPIO_setConfig (Debug_GPIO7, GPIO_CFG_OUTPUT);

    GPIO_write(Debug_GPIO0, GPIO_CFG_OUT_LOW);
    GPIO_write(Debug_GPIO1, GPIO_CFG_OUT_LOW);
    GPIO_write(Debug_GPIO2, GPIO_CFG_OUT_LOW);
    GPIO_write(Debug_GPIO3, GPIO_CFG_OUT_LOW);
    GPIO_write(Debug_GPIO4, GPIO_CFG_OUT_LOW);
    GPIO_write(Debug_GPIO5, GPIO_CFG_OUT_LOW);
    GPIO_write(Debug_GPIO6, GPIO_CFG_OUT_LOW);
    GPIO_write(Debug_GPIO7, GPIO_CFG_OUT_LOW);
    UART_init();

}

void* dbgUARTVal(unsigned char outVal){
    static UART_Handle uart = NULL;
    if(uart == NULL)
    {
        UART_Params uartParams;


        /* Create a UART with data processing off. */
        UART_Params_init(&uartParams);
        uartParams.writeDataMode = UART_DATA_BINARY;
        uartParams.readDataMode = UART_DATA_BINARY;
        uartParams.readReturnMode = UART_RETURN_FULL;
        uartParams.readEcho = UART_ECHO_OFF;
        uartParams.baudRate = 115200;

        uart = UART_open(Board_UART0, &uartParams);
        if (uart == NULL) {
            while (1);
        }
    }

    UART_write(uart,&outVal, 1);

    return (void*)1;
}

void* dbgOutputLoc(unsigned int outLoc){
    static int lastVal=0;
    if(outLoc > 127)
    {
        return (void*) 0;
    }

    GPIO_toggle(Debug_GPIO7);
    GPIO_write(Debug_GPIO0, (outLoc>>0)&1);
    GPIO_write(Debug_GPIO1, (outLoc>>1)&1);
    GPIO_write(Debug_GPIO2, (outLoc>>2)&1);
    GPIO_write(Debug_GPIO3, (outLoc>>3)&1);
    GPIO_write(Debug_GPIO4, (outLoc>>4)&1);
    GPIO_write(Debug_GPIO5, (outLoc>>5)&1);
    GPIO_write(Debug_GPIO6, (outLoc>>6)&1);

    return (void*) 1;
}

void failure(unsigned int num){

    dbgOutputLoc(num);
    taskDISABLE_INTERRUPTS();
    while(1) {
        int i;
        for(i=0 ; i<10000 ; i++){

        }
        GPIO_toggle(Board_GPIO_LED0);
    }
}
