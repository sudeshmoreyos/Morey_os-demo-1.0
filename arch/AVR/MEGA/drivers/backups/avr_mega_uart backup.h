#ifndef AVR_MEGA_UART_H
#define AVR_MEGA_UART_H

#include "../../../../os/drivers/Serial.h"
#include "../../../../platform.h"

#ifdef PLATFORM_SUPPORT_UART

#define EVEN 2
#define ODD 1
#define NONE 0

#define Serial avr_mega_uart_driver
extern const struct serial_driver avr_mega_uart_driver;

#endif
#endif