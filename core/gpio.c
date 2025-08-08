#include "gpio.h"

void gpio_write(uint8_t port, uint8_t pin, uint8_t value)
{
        switch (port) {
        case 'B':
                if (value == GPIO_HIGH)
                        helper_set_bit(&gpio_portb->output, pin);
                else 
                        helper_clear_bit(&gpio_portb->output, pin); 
                break;
        case 'C':
                if (value == GPIO_HIGH)
                        helper_set_bit(&gpio_portc->output, pin);
                else 
                        helper_clear_bit(&gpio_portc->output, pin); 
                break;
        case 'D':
                if (value == GPIO_HIGH)
                        helper_set_bit(&gpio_portd->output, pin);
                else 
                        helper_clear_bit(&gpio_portd->output, pin); 
                break;        
        }
}

void gpio_toggle(uint8_t port, uint8_t pin)
{
        switch (port) {
        case 'B':
                gpio_portb->output ^= (1 << pin);
                break;
        case 'C':
                gpio_portc->output ^= (1 << pin);
                break;
        case 'D':
                gpio_portd->output ^= (1 << pin);
                break;
        }

}

uint8_t gpio_read(uint8_t port, uint8_t pin)
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
