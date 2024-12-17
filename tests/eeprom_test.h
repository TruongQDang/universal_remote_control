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
        uint8_t write_data[4] = {9,10,11,12};
        uint16_t eeprom_address = 0; 
        cli();

        for (uint8_t i = 0; i < 4; i++) 
                eeprom_write(eeprom_address++, write_data[i]);
        
}




