#include "uart.h"
#include "helper.h" 
#include "uart_struct.h"
#include <avr/io.h>


static uint8_t is_transmit_buffer_empty();


void uart_init(uint16_t baud, 
                uint8_t receiver_config, uint8_t transmitter_config,
                uint8_t data_bit, uint8_t stop_bit)
{
        uart_set_baud_rate(baud);
        uart_set_receiver(receiver_config);
        uart_set_transmitter(transmitter_config);
        uart_set_data_bits(data_bit);
        uart_set_stop_bits(stop_bit);
}

void uart_transmit_char(uint8_t character)
{
        while (!is_transmit_buffer_empty());
        uart->data = character;
}

void uart_transmit_number(uint16_t number)
{
        if (number == 0) {
                uart_transmit_char('0');
        } else {
                uint8_t digit[5];
                uint8_t i = 0;
                while (number > 0) {
                        digit[i++] = number % 10 + '0';
                        number /= 10;
                }

                for (int8_t j = i - 1; j >= 0; j--)
                        uart_transmit_char(digit[j]);
        }
}

void uart_transmit_string(char *string)
{
        while (*string) {
                uart_transmit_char(*string);
                string++;
        }

}

void uart_transmit_line()
{
        uart_transmit_char('\n');
}

void uart_set_baud_rate(uint16_t baud)
{
        uart->baud_rate = FOSC/16/baud - 1;
}

void uart_set_receiver(uint8_t enable)
{
        if (enable) 
                helper_set_bit(&uart->control_and_status_B, RXEN0);
        else 
                helper_clear_bit(&uart->control_and_status_B, RXEN0);
}

void uart_set_transmitter(uint8_t enable)
{
        if (enable) 
                helper_set_bit(&uart->control_and_status_B, TXEN0);
        else 
                helper_clear_bit(&uart->control_and_status_B, TXEN0);
}

void uart_set_data_bits(uint8_t data_bits)
{
        switch (data_bits) {
        case EIGHT_DATABITS:
                helper_set_bit(&uart->control_and_status_C, UCSZ00);
                helper_set_bit(&uart->control_and_status_C, UCSZ01);                
                break;
        }
}

void uart_set_stop_bits(uint8_t stop_bits)
{
        switch (stop_bits) {
        case ONE_STOPBIT:
                helper_clear_bit(&uart->control_and_status_C,USBS0);
                break;
        case TWO_STOPBITS:
                helper_set_bit(&uart->control_and_status_C,USBS0);
                break;
        }
}

uint8_t is_transmit_buffer_empty()
{
        return helper_check_bit(&uart->control_and_status_A, UDRE0);
}

void transmit_number(uint16_t number)
{
        

}
