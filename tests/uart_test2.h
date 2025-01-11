#include "uart.h"
#include <util/delay.h>

void test()
{
        uart_init(9600, 
                RECEIVER_DISABLED, TRANSMITTER_ENABLED, 
                EIGHT_DATABITS, ONE_STOPBIT);

        while (1) {
                uart_transmit_string("OFF ");
        }
}