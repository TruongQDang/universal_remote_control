#ifndef _EEPROM_STRUCT_H_
#define _EEPROM_STRUCT_H_

#include <stdint.h>

typedef struct {
        volatile uint8_t control;
        volatile uint8_t data;
        volatile uint16_t address;
} eeprom_hw;

#define EEPROM_BASE 0x3F

#define eeprom ((eeprom_hw *)EEPROM_BASE)

_Static_assert(sizeof(eeprom_hw) == 4,"");


#endif