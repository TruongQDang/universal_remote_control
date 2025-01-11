#ifndef _MY_TIMER_H_
#define _MY_TIMER_H_

#include <stdint.h>
#include <timer_struct.h>
#include <avr/io.h>
#include "helper.h"

enum operation_mode {
        NORMAL_CLOCK,
        CTC_OCR,
        FAST_PWM_OCR,
};

enum compare_output_mode {
        NORMAL_PORT,
        TOGGLE_ON_MATCH,
        CLEAR_ON_MATCH,
        SET_ON_MATCH,
};

enum input_capture_edge {
        FALLING_EDGE,
        RISING_EDGE,
};

enum timer_interrupt {
        OVERFLOW,
        COMPARE_MATCH,
        COMPARE_MATCH_A,
        COMPARE_MATCH_B,
        INPUT_CAPTURE,
};

enum clock_source {
        STOP_CLOCK,
        PRESCALER_1,
        PRESCALER_8,
        PRESCALER_64,
        PRESCALER_256,
        PRESCALER_1024,
};

void timer0_set_clock_source(uint8_t clock_source);
void timer0_set_operation_mode(uint8_t operation_mode);
void timer0_set_compare_output_mode_A(uint8_t compare_output_mode);
void timer0_set_interrupt(uint8_t interrupt_type);
void timer0_clear_interrupt(uint8_t interrupt_type);
void timer0_clear_interrupt_flag(uint8_t interrupt_flag);

void timer1_set_operation_mode(uint8_t operation_mode);
void timer1_set_compare_output_mode_A(uint8_t compare_output_mode);
void timer1_set_clock_source(uint8_t clock_source);
void timer1_set_interrupt(uint8_t interrupt_type);
void timer1_clear_interrupt(uint8_t interrupt_type);
void timer1_clear_interrupt_flag(uint8_t interrupt_flag);
void timer1_enable_input_capture_noise_canceler();
void timer1_disable_input_capture_noise_canceler();
void timer1_set_counter_value(uint16_t counter_value);
uint16_t timer1_read_input_capture();

static inline void timer0_set_compare_value_A_inline(uint8_t compare_value)
{
        timer0->output_compare_A = compare_value;
}

static inline void timer0_set_counter_value_inline(uint8_t counter_value)
{
        timer0->counter = counter_value;
}

static inline uint16_t timer1_read_counter_value()
{
        return timer1->counter;  
}

static inline void timer1_set_clock_source_inline(uint8_t clock_source)
{
        switch (clock_source) {
        case STOP_CLOCK:
                helper_clear_bit(&timer1->control_B, CS10);
                helper_clear_bit(&timer1->control_B, CS11);
                helper_clear_bit(&timer1->control_B, CS12);
                break;
        case PRESCALER_1:
                helper_set_bit(&timer1->control_B, CS10);
                break;
        case PRESCALER_8:
                helper_set_bit(&timer1->control_B, CS11);
                break;
        case PRESCALER_64:
                helper_set_bit(&timer1->control_B, CS10);
                helper_set_bit(&timer1->control_B, CS11);
                break;
        case PRESCALER_256:
                helper_set_bit(&timer1->control_B, CS12);
                break;
        case PRESCALER_1024:
                helper_set_bit(&timer1->control_B, CS10);
                helper_set_bit(&timer1->control_B, CS12);
                break;
        }
}

static inline void timer1_set_counter_value_inline(uint16_t counter_value)
{
        timer1->counter = counter_value;
}

static inline uint16_t timer1_read_input_capture_inline()
{
        return timer1->input_capture;
}

static inline void timer1_set_compare_value_A_inline(uint16_t compare_value)
{
        timer1->output_compare_A = compare_value;
}

static inline void timer1_set_input_capture_edge_inline(uint8_t edge_type)
{
        if (edge_type == FALLING_EDGE)
                helper_clear_bit(&timer1->control_B, ICES1);
        else 
                helper_set_bit(&timer1->control_B, ICES1);
}


#endif