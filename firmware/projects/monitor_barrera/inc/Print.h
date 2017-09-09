#ifndef PRINT_H
#define PRINT_H

#include "Uart.h"
#include "sapi_uart.h"

int sprintf(char *out, const char *format, ...);
int debug_uart(const char *format, ...);
int ble_uart(const char *format, ...);
int pc_uart(const char *format, ...);

#endif
