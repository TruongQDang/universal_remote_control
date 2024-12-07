#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>
#include "gpio_struct.h"
#include "helper.h"

enum GPIO_DIRECTION_CONFIG {
        GPIO_INPUT,
        GPIO_OUTPUT,
};

enum GPIO_OUTPUT_CONFIG {
        GPIO_LOW,
        GPIO_HIGH,
};

enum GPIO_INPUT_CONFIG {
        GPIO_PULLUP_DISABLED,
        GPIO_PULLUP_ENABLED,
};

void gpio_write(uint8_t port, uint8_t pin, uint8_t value);
void gpio_toggle(uint8_t port, uint8_t pin);
uint8_t gpio_read(uint8_t port, uint8_t pin);

static inline void gpio_set_direction(uint8_t port, uint8_t pin, uint8_t direction)
{
        switch (port) {
        case 'B':
                if (direction == GPIO_OUTPUT)
                        helper_set_bit(&gpio_portb->direction, pin);
                else 
                        helper_clear_bit(&gpio_portb->direction, pin); 
                break;
        case 'C':
                if (direction == GPIO_OUTPUT)
                        helper_set_bit(&gpio_portc->direction, pin);
                else 
                        helper_clear_bit(&gpio_portc->direction, pin); 
                break;
        case 'D':
                if (direction == GPIO_OUTPUT)
                        helper_set_bit(&gpio_portd->direction, pin);
                else 
                        helper_clear_bit(&gpio_portd->direction, pin); 
                break;        
        }
}



#endif