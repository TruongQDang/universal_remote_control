#ifndef _MY_EEPROM_H_
#define _MY_EEPROM_H_

#include <stdint.h>

/**
No interrupt should be allowed during read/write operation!
*/

void eeprom_write(uint16_t address, uint8_t data);
uint8_t eeprom_read(uint16_t address);

#endif