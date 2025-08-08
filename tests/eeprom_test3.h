#include "eeprom.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <util/delay.h>


void test()
{
        uint16_t data_write[] = {500,7000,1000,2500};
        uint16_t data_read[4] = {0,0,0,0};
        uint16_t address = 0;
        cli();
        for (uint16_t i = 0; i < 4; i++) {
                eeprom_write_word(address, data_write[i]);
                address += 2;
        }

        address = 0;
        for (uint16_t i = 0; i < 4; i++) {
                data_read[i] = eeprom_read_word(address);
                address += 2;
        }
        sei();

        uart_init(9600, 
                RECEIVER_DISABLED, TRANSMITTER_ENABLED, 
                EIGHT_DATABITS, ONE_STOPBIT);

        while (1) {
                for (uint16_t i = 0; i < 4; i++) {
                        uart_transmit_number(data_read[i]);
                        uart_transmit_line();
                }
                _delay_ms(1000);
        }

}