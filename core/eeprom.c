#include "eeprom.h"
#include "eeprom_struct.h"
#include "helper.h"
#include "avr/io.h"

static uint8_t write_finished();
static void set_eeprom_address(uint16_t address);
static void set_eeprom_data(uint8_t data);

void eeprom_write_byte(uint16_t address, uint8_t data)
{
        /* Wait for completion of previous write */
        while (!write_finished());
        set_eeprom_address(address);
        set_eeprom_data(data);
        helper_set_bit(&eeprom->control, EEMPE);
        /* Start eeprom write by setting EEPE */
        helper_set_bit(&eeprom->control, EEPE);        
}

uint8_t eeprom_read_byte(uint16_t address)
{
        /* Wait for completion of previous write */
        while (!write_finished());
        set_eeprom_address(address);
        /* Start eeprom read*/
        helper_set_bit(&eeprom->control, EERE);
        return eeprom->data;
}

void eeprom_write_word(uint16_t address, uint16_t data)
{
        uint8_t data_to_be_written = (uint8_t)data;
        eeprom_write_byte(address, data_to_be_written);
        data_to_be_written = (uint8_t)(data >> 8);
        eeprom_write_byte(++address, data_to_be_written);       
}

uint16_t eeprom_read_word(uint16_t address)
{
        uint16_t data_read = eeprom_read_byte(address);
        data_read |= (eeprom_read_byte(++address) << 8);
        return data_read;
}

uint8_t write_finished()
{
        if (helper_check_bit(&eeprom->control, EEPE))
                return 0;
        else
                return 1;
}

void set_eeprom_address(uint16_t address)
{
        eeprom->address = address;
}

void set_eeprom_data(uint8_t data)
{       
        eeprom->data = data;
}
