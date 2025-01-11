#ifndef _MY_GPIO_H_
#define _MY_GPIO_H_

#include <stdint.h>
#include "gpio_struct.h"
#include "helper.h"

enum gpio_direction_config {
        GPIO_INPUT,
        GPIO_OUTPUT,
};

enum gpio_ouput_config {
        GPIO_LOW,
        GPIO_HIGH,
};

enum gpio_input_config {
        GPIO_PULLUP_DISABLED,
        GPIO_PULLUP_ENABLED,
};

enum pin_change_group {
        PCINT_0_7,
        PCINT_8_14,
        PCINT_16_23,
};

void gpio_write(uint8_t port, uint8_t pin, uint8_t value);
void gpio_toggle(uint8_t port, uint8_t pin);
uint8_t gpio_read(uint8_t port, uint8_t pin);

void gpio_enable_pin_change_interrupt(uint8_t pcint_group, uint8_t pcint_x);
void gpio_clear_pin_change_interrupt_flag(uint8_t pcint_x);



static inline uint8_t gpio_read_inline(uint8_t port, uint8_t pin)
{
        switch (port) {
        case 'B':
                if (helper_check_bit(&gpio_portb->input, pin)) 
                        return 1;
                else 
                        return 0;
        case 'C':
                if (helper_check_bit(&gpio_portc->input, pin)) 
                        return 1;
                else 
                        return 0;
        case 'D':
                if (helper_check_bit(&gpio_portd->input, pin))
                        return 1;
                else 
                        return 0;
        }
        return -1;
}

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