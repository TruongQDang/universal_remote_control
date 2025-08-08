#include "uart.h"
#include <util/delay.h>


/* 
Send 0 and 569 through UART, each is separated by a new line
*/

void test_setup()
{
        uart_init(9600, 
                RECEIVER_DISABLED, TRANSMITTER_ENABLED, 
                EIGHT_DATABITS, ONE_STOPBIT);
}

void test_loop()
{
        while (1) {
                uart_transmit_number(529);
                uart_transmit_line();
                _delay_ms(1000);
                uart_transmit_number(0);
                uart_transmit_line();
                _delay_ms(1000);
        }
}