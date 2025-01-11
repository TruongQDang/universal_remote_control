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

typedef struct {
        volatile uint8_t mask0;
        volatile uint8_t mask1;
        volatile uint8_t mask2;
} pin_change_interrupt_mask_hw;

typedef struct  {
        volatile uint8_t interrupt_flag;
} pin_change_interrupt_flag_hw;

typedef struct  {
        volatile uint8_t control;
} pin_change_interrupt_control_hw;




#define GPIO_PORTB_BASE 0x23
#define GPIO_PORTC_BASE 0x26
#define GPIO_PORTD_BASE 0x29
#define PIN_CHANGE_INTERRUPT_CONTROL_BASE 0x68
#define PIN_CHANGE_INTERRUPT_FLAG_BASE 0x3B
#define PIN_CHANGE_INTERRUPT_MASK_BASE 0x6B

#define gpio_portb ((gpio_portb_hw *)GPIO_PORTB_BASE)
#define gpio_portc ((gpio_portc_hw *)GPIO_PORTC_BASE)
#define gpio_portd ((gpio_portd_hw *)GPIO_PORTD_BASE)
#define pin_change_interrupt_control ((pin_change_interrupt_control_hw *)PIN_CHANGE_INTERRUPT_CONTROL_BASE)
#define pin_change_interrupt_mask ((pin_change_interrupt_mask_hw *)PIN_CHANGE_INTERRUPT_MASK_BASE)
#define pin_change_interrupt_flag ((pin_change_interrupt_flag_hw *)PIN_CHANGE_INTERRUPT_FLAG_BASE)

_Static_assert(sizeof(gpio_portb_hw) == 3,"");
_Static_assert(sizeof(gpio_portc_hw) == 3,"");
_Static_assert(sizeof(gpio_portd_hw) == 3,"");
_Static_assert(sizeof(pin_change_interrupt_control_hw) == 1,"");
_Static_assert(sizeof(pin_change_interrupt_mask_hw) == 3,"");
_Static_assert(sizeof(pin_change_interrupt_flag_hw) == 1,"");


#endif