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

#define SERIAL_OPERATOR_CMD '$'
#define SERIAL_OPERATOR_SPACE " "
#define SERIAL_OPERATOR_START 'S'
#define SERIAL_OPERATOR_END 'E'
#define SERIAL_OPERATOR_READ 'R'
#define SERIAL_OPERATOR_DELETE_STATE 'D'

#define PC_START_MSG "start ble scan\r\n"
#define PC_END_MSG "end ble scan\r\n"
#define PC_FORMAT_ERROR_MSG "format error\r\n"

#define PC_READ_STATUS_CMD_OFFSET 2
#define PC_ASSIGN_CALLBACK_CMD_OFFSET 1

#define CMD_BUFFER_SIZE 30

void pcManagerTask(void * a);
void initPcManagerTask(uint32_t priority);

size_t getCmdLength();

#endif //MONITOR_BARRERA_PCMANAGER_H
