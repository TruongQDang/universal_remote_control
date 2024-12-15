#ifndef _GPIO_STRUCT_H_
#define _GPIO_STRUCT_H_

#include <stdint.h>

typedef struct {
        volatile uint8_t input; 
        volatile uint8_t direction;
        volatile uint8_t output;
} gpio_portb_hw;

typedef struct {
        volatile uint8_t input; 
        volatile uint8_t direction;
        volatile uint8_t output;
} gpio_portc_hw;

typedef struct {
        volatile uint8_t input; 
        volatile uint8_t direction;
        volatile uint8_t output;
} gpio_portd_hw;

#define GPIO_PORTB_BASE 0x23
#define GPIO_PORTC_BASE 0x26
#define GPIO_PORTD_BASE 0x29

#define gpio_portb ((gpio_portb_hw *)GPIO_PORTB_BASE)
#define gpio_portc ((gpio_portc_hw *)GPIO_PORTC_BASE)
#define gpio_portd ((gpio_portd_hw *)GPIO_PORTD_BASE)

_Static_assert(sizeof(gpio_portb_hw) == 3,"");
_Static_assert(sizeof(gpio_portc_hw) == 3,"");
_Static_assert(sizeof(gpio_portd_hw) == 3,"");


#endif