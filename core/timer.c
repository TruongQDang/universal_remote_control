#include "timer.h"
#include "helper.h"
#include "timer_struct.h"
#include <avr/io.h>

void timer0_set_clock_source(uint8_t clock_source)
{
        switch (clock_source) {
        case STOP_CLOCK:
                helper_clear_bit(&timer0->control_B, CS00);
                helper_clear_bit(&timer0->control_B, CS01);
                helper_clear_bit(&timer0->control_B, CS02);
                break;
        case PRESCALER_1:
                helper_set_bit(&timer0->control_B, CS00);
                break;
        case PRESCALER_8:
                helper_set_bit(&timer0->control_B, CS01);
                break;
        case PRESCALER_64:
                helper_set_bit(&timer0->control_B, CS00);
                helper_set_bit(&timer0->control_B, CS01);
                break;
        case PRESCALER_256:
                helper_set_bit(&timer0->control_B, CS02);
                break;
        case PRESCALER_1024:
                helper_set_bit(&timer0->control_B, CS00);
                helper_set_bit(&timer0->control_B, CS02);
                break;
        }
}

void timer0_set_operation_mode(uint8_t operation_mode)
{
        switch (operation_mode) {
        case NORMAL_CLOCK:
                helper_clear_bit(&timer0->control_A, WGM00);
                helper_clear_bit(&timer0->control_A, WGM01);
                helper_clear_bit(&timer0->control_B, WGM02);
                break;
        case CTC_OCR:
                helper_set_bit(&timer0->control_A, WGM01);
                break;
        case FAST_PWM_OCR:
                helper_set_bit(&timer0->control_A, WGM00);
                helper_set_bit(&timer0->control_A, WGM01);
                helper_set_bit(&timer0->control_B, WGM02);
                break;
        }
}

void timer0_set_compare_output_mode_A(uint8_t compare_output_mode)
{
        switch (compare_output_mode) {
        case NORMAL_PORT:
                helper_clear_bit(&timer0->control_A, COM0A1);
                helper_clear_bit(&timer0->control_A, COM0A0);
                break;
        case TOGGLE_ON_MATCH:
                helper_set_bit(&timer0->control_A, COM0A0);
                break;
        case CLEAR_ON_MATCH:
                helper_set_bit(&timer0->control_A, COM0A1);
                break;
        case SET_ON_MATCH:
                helper_set_bit(&timer0->control_A, COM0A1);
                helper_set_bit(&timer0->control_A, COM0A0);
                break;
        }
}

void timer0_set_interrupt(uint8_t interrupt_type)
{
        switch (interrupt_type) {
        case OVERFLOW:
                helper_set_bit(&timer_interrupt_mask->timer0_interrupt_mask, TOIE0);
                break;
        case COMPARE_MATCH_A:
                helper_set_bit(&timer_interrupt_mask->timer0_interrupt_mask, OCIE0A);
                break;
        }
}

void timer0_clear_interrupt(uint8_t interrupt_type)
{
        switch (interrupt_type) {
        case OVERFLOW:
                helper_clear_bit(&timer_interrupt_mask->timer0_interrupt_mask, TOIE0);
                break;
        case COMPARE_MATCH_A:
                helper_clear_bit(&timer_interrupt_mask->timer0_interrupt_mask, OCIE0A);
                break;
        }
}

void timer0_clear_interrupt_flag(uint8_t interrupt_flag)
{
        switch (interrupt_flag) {
        case OVERFLOW:
                helper_clear_flag(&timer_interrupt_flag->timer0_interrupt_flag, TOV0);
                break;
        case COMPARE_MATCH_A:
                helper_clear_flag(&timer_interrupt_flag->timer0_interrupt_flag, OCF0A);
                break;
        }
}

void timer1_set_operation_mode(uint8_t operation_mode)
{
        switch (operation_mode) {
        case NORMAL_CLOCK:
                helper_clear_bit(&timer1->control_A, WGM10);
                helper_clear_bit(&timer1->control_A, WGM11);
                helper_clear_bit(&timer1->control_B, WGM12);
                helper_clear_bit(&timer1->control_B, WGM13);
                break;
        case CTC_OCR:
                helper_set_bit(&timer1->control_B, WGM12);
                break;
        case FAST_PWM_OCR:
                helper_set_bit(&timer1->control_A, WGM10);
                helper_set_bit(&timer1->control_A, WGM11);
                helper_set_bit(&timer1->control_B, WGM12);
                helper_set_bit(&timer1->control_B, WGM13);
                break;
        }
}

void timer1_set_compare_output_mode_A(uint8_t compare_output_mode)
{
        switch (compare_output_mode) {
        case NORMAL_PORT:
                helper_clear_bit(&timer1->control_A, COM1A1);
                helper_clear_bit(&timer1->control_A, COM1A0);
                break;
        case TOGGLE_ON_MATCH:
                helper_set_bit(&timer1->control_A, COM1A0);
                break;
        case CLEAR_ON_MATCH:
                helper_set_bit(&timer1->control_A, COM1A1);
                break;
        case SET_ON_MATCH:
                helper_set_bit(&timer1->control_A, COM1A1);
                helper_set_bit(&timer1->control_A, COM1A0);
                break;
        }
}

void timer1_set_clock_source(uint8_t clock_source)
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

void timer1_set_interrupt(uint8_t interrupt_type)
{
        switch (interrupt_type) {
        case OVERFLOW:
                helper_set_bit(&timer_interrupt_mask->timer1_interrupt_mask, TOIE1);
                break;
        case COMPARE_MATCH_A:
                helper_set_bit(&timer_interrupt_mask->timer1_interrupt_mask, OCIE1A);
                break;
        case INPUT_CAPTURE:
                helper_set_bit(&timer_interrupt_mask->timer1_interrupt_mask, ICIE1);
                break;
        }
}

void timer1_clear_interrupt(uint8_t interrupt_type)
{
        switch (interrupt_type) {
        case OVERFLOW:
                helper_clear_bit(&timer_interrupt_mask->timer1_interrupt_mask, TOIE1);
                break;
        case COMPARE_MATCH_A:
                helper_clear_bit(&timer_interrupt_mask->timer1_interrupt_mask, OCIE1A);
                break;
        case INPUT_CAPTURE:
                helper_clear_bit(&timer_interrupt_mask->timer1_interrupt_mask, ICIE1);
                break;
        } 
}

void timer1_clear_interrupt_flag(uint8_t interrupt_flag)
{
        switch (interrupt_flag) {
        case OVERFLOW:
                helper_clear_flag(&timer_interrupt_flag->timer1_interrupt_flag, TOV1);
                break;
        case COMPARE_MATCH_A:
                helper_clear_flag(&timer_interrupt_flag->timer1_interrupt_flag, OCF1A);
                break;
        case INPUT_CAPTURE:
                helper_clear_flag(&timer_interrupt_flag->timer1_interrupt_flag, ICF1);
                break;
        }
}

void timer1_enable_input_capture_noise_canceler()
{
        helper_set_bit(&timer1->control_B, ICNC1);
}

void timer1_disable_input_capture_noise_canceler()
{
        helper_clear_bit(&timer1->control_B, ICNC1);
}

void timer1_set_counter_value(uint16_t counter_value)
{
        timer1->counter = counter_value;
}


uint16_t timer1_read_input_capture()
{
        return timer1->input_capture;
}
