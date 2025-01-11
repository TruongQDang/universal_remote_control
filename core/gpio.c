#include "gpio.h"
#include <avr/io.h>

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

void gpio_enable_pin_change_interrupt(uint8_t pcint_group, uint8_t pcint_x)
{
        switch (pcint_group) {
        case PCINT_0_7:
                helper_set_bit(&pin_change_interrupt_control->control, PCIE0);
                helper_set_bit(&pin_change_interrupt_mask->mask0, pcint_x);
                break;
        case PCINT_8_14:
                helper_set_bit(&pin_change_interrupt_control->control, PCIE1);
                helper_set_bit(&pin_change_interrupt_mask->mask1, pcint_x);
                break;
        case PCINT_16_23:
                helper_set_bit(&pin_change_interrupt_control->control, PCIE2);
                helper_set_bit(&pin_change_interrupt_mask->mask2, pcint_x);
        }
}

void gpio_clear_pin_change_interrupt_flag(uint8_t pcint_x)
{
        if (pcint_x >= PCINT0 && pcint_x <= PCINT7) {
                helper_clear_flag(&pin_change_interrupt_flag->interrupt_flag, PCIF0);
        } else if (pcint_x >= PCINT8 && pcint_x <= PCINT14) {
                helper_clear_flag(&pin_change_interrupt_flag->interrupt_flag, PCIF1);
        } else if (pcint_x >= PCINT16 && pcint_x <= PCINT23) {
                helper_clear_flag(&pin_change_interrupt_flag->interrupt_flag, PCIF2);
        }
}
