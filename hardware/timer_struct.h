#ifndef _TIMER_STRUCT_H_
#define _TIMER_STRUCT_H_

#include <stdint.h>

typedef struct  {
        volatile uint8_t control_A;
        volatile uint8_t control_B;
        volatile uint8_t counter;
        volatile uint8_t output_compare_A;
        volatile uint8_t output_compare_B;
} timer0_hw;

typedef struct  {
        volatile uint8_t control_A;
        volatile uint8_t control_B;
        volatile uint8_t control_C;
        uint8_t reserved;
        volatile uint16_t counter;
        volatile uint16_t input_capture;
        volatile uint16_t output_compare_A;
        volatile uint16_t output_compare_B;
} timer1_hw;

typedef struct {
        volatile uint8_t timer0_interrupt_mask;
        volatile uint8_t timer1_interrupt_mask;
        volatile uint8_t timer2_interrupt_mask;
} timer_interrupt_mask_hw;

typedef struct {
        volatile uint8_t timer0_interrupt_flag;
        volatile uint8_t timer1_interrupt_flag;
        volatile uint8_t timer2_interrupt_flag;
} timer_interrupt_flag_hw;

#define TIMER0_BASE 0x44
#define TIMER1_BASE 0x80
#define TIMER_INTERRUPT_MASK_BASE 0x6E
#define TIMER_INTERRUPT_FLAG_BASE 0x35

#define timer0 ((timer0_hw *)TIMER0_BASE)
#define timer1 ((timer1_hw *)TIMER1_BASE)
#define timer_interrupt_mask ((timer_interrupt_mask_hw *)TIMER_INTERRUPT_MASK_BASE)
#define timer_interrupt_flag ((timer_interrupt_flag_hw *)TIMER_INTERRUPT_FLAG_BASE)

_Static_assert(sizeof(timer0_hw) == 5,"");
_Static_assert(sizeof(timer1_hw) == 12,"");
_Static_assert(sizeof(timer_interrupt_mask_hw) == 3,"");
_Static_assert(sizeof(timer_interrupt_flag_hw) == 3,"");

#endif