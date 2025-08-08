#include "eeprom.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <util/delay.h>


void test()
{
        uint16_t data_write[] = {0,10,5,4,5,7,23,10,199};
        uint16_t data_read[8];
        uint16_t address = 0;
        cli();
        for (uint16_t i = 0; i < 8; i++) {
                data_write[i] = data_write[i+1];
                eeprom_write_word(address, data_write[i+1]);
                address += 2;
        }

        address = 0;
        for (uint16_t i = 0; i < 8; i++) {
                data_read[i] = eeprom_read_word(address);
                address += 2;
        }
        sei();

        uart_init(9600, 
                RECEIVER_DISABLED, TRANSMITTER_ENABLED, 
                EIGHT_DATABITS, ONE_STOPBIT);

        while (1) {
                for (uint16_t i = 0; i < 8; i++) {
                        uart_transmit_string("WRITE ");
                        uart_transmit_number(data_write[i]);
                        uart_transmit_line();
                        uart_transmit_string("READ ");
                        uart_transmit_number(data_read[i]);
                        uart_transmit_line();
                }
                _delay_ms(1000);
        }

}