#ifndef _UART_STRUCT_H_
#define _UART_STRUCT_H_

#include <stdint.h>

typedef struct {
        volatile uint8_t control_and_status_A;
        volatile uint8_t control_and_status_B;
        volatile uint8_t control_and_status_C;
        uint8_t reserved;
        volatile uint16_t baud_rate;
        volatile uint8_t data;       
} uart_hw;

#define UART_BASE 0xC0

#define uart ((uart_hw *)UART_BASE)

_Static_assert(sizeof(uart_hw) == 7, "");

#endif 