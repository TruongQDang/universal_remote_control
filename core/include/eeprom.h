#ifndef _MY_EEPROM_H_
#define _MY_EEPROM_H_

#include <stdint.h>

/**
No interrupt should be allowed during read/write operation!
*/

void eeprom_write_byte(uint16_t address, uint8_t data);
uint8_t eeprom_read_byte(uint16_t address);
void eeprom_write_word(uint16_t address, uint16_t data);
uint16_t eeprom_read_word(uint16_t address);

#endif