//
// Created by ivan on 09/09/17.
//

#ifndef MONITOR_BARRERA_PCMANAGER_H
#define MONITOR_BARRERA_PCMANAGER_H

#include "ctype.h"
#include "stddef.h"
#include "sapi.h"
#include "Uart.h"
#include "Print.h"
#include "queue.h"

#define SERIAL_OPERATOR_CMD '$'
#define SERIAL_OPERATOR_ASSIGN '='
#define SERIAL_OPERATOR_MODE 'M'

#define SERIAL_OPERATOR_MODE_NORMAL '0'
#define SERIAL_OPERATOR_MODE_SAFE_FAIL '1'
#define SERIAL_OPERATOR_MODE_UNSAFE_FAIL '2'

#define CMD_BUFFER_SIZE 30

#define QUEUE_SIZE 64

typedef enum{normal, safeFail, unsafeFail } modelMode_t;

xQueueHandle modelModeQueue;

void pcManagerTask(void * a);
void initPcManagerTask(uint32_t priority);

size_t getCmdLength();

#endif //MONITOR_BARRERA_PCMANAGER_H
