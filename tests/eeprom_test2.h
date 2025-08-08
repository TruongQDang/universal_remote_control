#include "eeprom.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <util/delay.h>

/*
Write an array of data to eeprom
Read back
Print to uart
*/



void test()
{
        uint8_t read_data[4]= {0,0,0,0};
        uint16_t eeprom_address = 0; 
        cli();
        for (uint8_t i = 0; i < 4; i++)
                read_data[i] = eeprom_read(eeprom_address++);

        sei();
        uart_init(9600, 
                RECEIVER_DISABLED, TRANSMITTER_ENABLED, 
                EIGHT_DATABITS, ONE_STOPBIT);

        while (1) {
                for (uint8_t i = 0; i < 4; i++) {
                        uart_transmit_number(read_data[i]);
                        uart_transmit_line();
                }
                _delay_ms(1000);
        }
        
}




