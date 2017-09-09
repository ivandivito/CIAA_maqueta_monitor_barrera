//
// Created by ivan on 09/09/17.
//

#include "Uart.h"

#define QUEUE_IN_SIZE 64
#define QUEUE_OUT_SIZE 64

void initUart() {

    //Debug UART
    inputUartUsbQueue = xQueueCreate(QUEUE_IN_SIZE, sizeof(char));
    outputUartUsbQueue = xQueueCreate(QUEUE_OUT_SIZE, sizeof(char));

    uartConfigCb(UART_USB, uartUsbCallback);
    uartConfig(UART_USB, 115200);

    //BLE UART
    inputUart232Queue = xQueueCreate(QUEUE_IN_SIZE, sizeof(char));
    outputUart232Queue = xQueueCreate(QUEUE_OUT_SIZE, sizeof(char));

    uartConfigCb(UART_232, uart232Callback);
    uartConfig(UART_232, 115200);

}

void initUartTask(uint32_t priority){
    xTaskCreate(uartUsbOutputTask, (const char *)"uartUsbOutputTask", configMINIMAL_STACK_SIZE, 0, priority, 0);
    xTaskCreate(uart232OutputTask, (const char *)"uart232OutputTask", configMINIMAL_STACK_SIZE, 0, priority, 0);
}

uint32_t uartUsbSendByte(char byte) {
    return xQueueSendToBack(outputUartUsbQueue, ( void * ) &byte, 100 / portTICK_RATE_MS);
}

uint32_t uart232SendByte(char byte) {
    return xQueueSendToBack(outputUart232Queue, ( void * ) &byte, 100 / portTICK_RATE_MS);
}

void uart232OutputTask(void * a){

    char sendByte;

    while(1) {

        if(xQueueReceive(outputUart232Queue, &sendByte, portMAX_DELAY) != errQUEUE_EMPTY){
            uartWriteByte(UART_232, sendByte);
        }

    }

}

void uartUsbOutputTask(void * a){

    char sendByte;

    while(1) {

        if(xQueueReceive(outputUartUsbQueue, &sendByte, portMAX_DELAY) != errQUEUE_EMPTY){
            uartWriteByte(UART_USB, sendByte);
        }

    }

}

void uartUsbCallback(const uartMap_t uart, uint8_t data){

    portBASE_TYPE higherPriorityTaskWoken = pdFALSE;

    xQueueSendToBackFromISR(inputUartUsbQueue, &data, &higherPriorityTaskWoken);

    portEND_SWITCHING_ISR(higherPriorityTaskWoken);

}

void uart232Callback(const uartMap_t uart, uint8_t data){

    portBASE_TYPE higherPriorityTaskWoken = pdFALSE;

    xQueueSendToBackFromISR(inputUart232Queue, &data, &higherPriorityTaskWoken);

    portEND_SWITCHING_ISR(higherPriorityTaskWoken);

}
