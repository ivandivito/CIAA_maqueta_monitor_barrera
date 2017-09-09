//
// Created by ivan on 09/09/17.
//

#ifndef MONITOR_BARRERA_UART_H
#define MONITOR_BARRERA_UART_H


#include "board.h"

#include "sapi_uart.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"

xQueueHandle inputUartUsbQueue;
xQueueHandle outputUartUsbQueue;

xQueueHandle inputUart232Queue;
xQueueHandle outputUart232Queue;

uint32_t uartUsbSendByte(char byte);
uint32_t uart232SendByte(char byte);

void initUart();
void initUartTask(uint32_t priority);

void uart232OutputTask(void * a);
void uart232Callback(const uartMap_t uart, uint8_t data);

void uartUsbOutputTask(void * a);
void uartUsbCallback(const uartMap_t uart, uint8_t data);

#endif //MONITOR_BARRERA_UART_H
