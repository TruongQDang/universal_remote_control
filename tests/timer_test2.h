#include "timer.h"
#include "gpio.h"
#include <util/delay.h>
#include <avr/interrupt.h>

/*
timer0 for pwm 38kHZ, top = 25, prescaler = 8
*/

void test()
{
        
        // PD6, D6
        gpio_set_direction('D',6,GPIO_OUTPUT);

        timer0_set_operation_mode(FAST_PWM_OCR);
        timer0_set_compare_output_mode_A(TOGGLE_ON_MATCH);
        timer0_set_compare_value_A_inline(25);
        timer0_set_clock_source(PRESCALER_8);

        while (1);
}