#ifndef _MY_UART_H_
#define _MY_UART_H_

#include <stdint.h>

#define FOSC F_CPU

enum receiver_config {
        RECEIVER_DISABLED,
        RECEIVER_ENABLED,
};

enum transmitter_config {
        TRANSMITTER_DISABLED,
        TRANSMITTER_ENABLED,
};

enum data_bit_nums {
        EIGHT_DATABITS,
};

enum stop_bit_nums {
        ONE_STOPBIT,
        TWO_STOPBITS,
};


void uart_init(uint16_t baud, 
                uint8_t receiver_config, uint8_t transmitter_config,
                uint8_t data_bit, uint8_t stop_bit);
void uart_transmit_char(uint8_t character);
void uart_transmit_number(uint16_t number);
void uart_transmit_string(char *string);
void uart_transmit_line();
void uart_set_baud_rate(uint16_t baud);
void uart_set_receiver(uint8_t enable);
void uart_set_transmitter(uint8_t enable);
void uart_set_data_bits(uint8_t data_bits);
void uart_set_stop_bits(uint8_t stop_bits);

#endif