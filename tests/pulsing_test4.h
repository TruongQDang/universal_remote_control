#include "timer.h"
#include "gpio.h"
#include <util/delay.h>
#include <avr/interrupt.h>

/*
ON signal are modulated at 38kHZ
timer0 for pwm, top = 25, prescaler = 8
timer1 for timing between pulse
Make sure that each button press corresponds to only one transaction
*/

#define MAX 67
uint16_t pulse_sequence[MAX] = {18116, 8939, 1185, 1061, 1198, 1049, 1191, 1057, 1186, 1059,
                                1191,1054,1193,1054,1191,1055,1197,1051,1187,3266,
                                1188,3267,1190,3261,1189,3263,1191,3262,1192,3261,
                                1191,3261,1193,3258,1204,3248,1196,1049,1194,3259,
                                1192,1054,1198,1050,1193,1050,1200,3254,1198,1047,
                                1260,986,1202,3250,1205,1042,1256,3194,1256,3198,
                                1254,3199,1254,991,1258,3197,1254};
volatile uint16_t index = 0;
uint8_t transmit_done = 1;
uint8_t button_released = 1;

void test()
{
        
        // PD6, D6, ir led pin
        gpio_set_direction('D',6,GPIO_OUTPUT);
        // button pin, D3
        gpio_set_direction('D',3,GPIO_INPUT);
        gpio_write('D',3,GPIO_PULLUP_ENABLED); 
        
        sei();

        timer0_set_operation_mode(FAST_PWM_OCR);
        timer0_set_compare_output_mode_A(TOGGLE_ON_MATCH);
        timer0_set_compare_value_A_inline(25);

        timer1_set_operation_mode(CTC_OCR);
        timer1_set_interrupt(COMPARE_MATCH_A);
        timer1_set_compare_value_A_inline(pulse_sequence[index++]);

        while (1) {
                if (gpio_read('D',3) == GPIO_LOW && transmit_done && button_released) {
                        transmit_done = 0;
                        button_released = 0;

                        timer0_set_compare_output_mode_A(TOGGLE_ON_MATCH);
                        timer0_set_counter_value_inline(0);

                        index = 0;
                        timer1_set_compare_value_A_inline(pulse_sequence[index++]);
                        timer1_set_counter_value_inline(0);
                        timer1_clear_interrupt_flag(COMPARE_MATCH_A);
                        timer1_set_interrupt(COMPARE_MATCH_A);
                        
                        timer0_set_clock_source(PRESCALER_8);
                        timer1_set_clock_source(PRESCALER_8);
                }
                if (gpio_read('D',3) == GPIO_HIGH)
                        button_released = 1;
        }
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
                timer1_set_clock_source(STOP_CLOCK);
                timer0_set_clock_source(STOP_CLOCK);
                timer1_clear_interrupt(COMPARE_MATCH_A);
                
                timer0_set_compare_output_mode_A(NORMAL_PORT);
                gpio_write('D',6,GPIO_LOW);
                
                transmit_done = 1;
        }
}