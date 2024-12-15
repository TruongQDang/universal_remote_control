#include "timer.h"
#include "gpio.h"
#include <util/delay.h>
#include <avr/interrupt.h>

/*
ON signal are modulated at 38kHZ
timer0 for pwm, top = 25, prescaler = 8
timer1 for timing between pulse
*/

#define MAX 4
uint16_t pulse_sequence[MAX] = {500, 600, 400, 6000};
uint16_t index = 0;

void test()
{
        
        // PD6, D6
        gpio_set_direction('D',6,GPIO_OUTPUT);
        
        sei();

        timer0_set_operation_mode(FAST_PWM_OCR);
        timer0_set_compare_output_mode_A(TOGGLE_ON_MATCH);
        timer0_set_compare_value_A_inline(25);

        timer1_set_operation_mode(CTC_OCR);
        timer1_set_interrupt(COMPARE_MATCH_A);
        timer1_set_compare_value_A_inline(pulse_sequence[index++]);
        
        timer0_set_clock_source(PRESCALER_8);
        timer1_set_clock_source(PRESCALER_8);

        while (1);
}

ISR(TIMER1_COMPA_vect)
{
        if (index < MAX) {
                if (index % 2 == 0) {
                        timer0_set_compare_output_mode_A(TOGGLE_ON_MATCH);
                        timer1_set_compare_value_A_inline(pulse_sequence[index++]);
                } else {
                        timer0_set_compare_output_mode_A(NORMAL_PORT);
                        gpio_write('D',6,GPIO_LOW);
                        timer1_set_compare_value_A_inline(pulse_sequence[index++]);
                }
        } else { 
                index = 0;
                timer0_set_compare_output_mode_A(TOGGLE_ON_MATCH);
                timer1_set_compare_value_A_inline(pulse_sequence[index++]);
        }
}