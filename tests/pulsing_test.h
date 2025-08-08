#include "timer.h"
#include "gpio.h"
#include <util/delay.h>
#include <avr/interrupt.h>

/*
ON signal are modulated at 38kHZ
timer1, top = 52, prescaler = 8
*/

#define MAX 10
uint16_t pulse_sequence[MAX] = {2300, 300, 250, 150, 100, 140, 80, 200, 280,350};
uint16_t index = 0;

void test()
{
        
        // PB1, D9
        gpio_set_direction('B',1,GPIO_OUTPUT);
        
        sei();
        timer1_set_operation_mode(CTC_OCR);
        timer1_set_interrupt(COMPARE_MATCH_A);
        timer1_set_compare_value_A_inline(pulse_sequence[index++]);
        timer1_set_clock_source(PRESCALER_8);

        gpio_write('B',1,GPIO_HIGH);

        while (1);
}

ISR(TIMER1_COMPA_vect)
{
        if (index < MAX) {
                if (index % 2 == 0) {
                        gpio_write('B',1,GPIO_HIGH); 
                        timer1_set_compare_value_A_inline(pulse_sequence[index++]);
                } else {
                        gpio_write('B',1,GPIO_LOW); 
                        timer1_set_compare_value_A_inline(pulse_sequence[index++]);
                }
        } else { 
                index = 0;
                gpio_write('B',1,GPIO_HIGH);
                timer1_set_compare_value_A_inline(pulse_sequence[index++]);
        }
}