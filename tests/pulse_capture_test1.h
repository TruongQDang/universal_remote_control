#include "timer.h"
#include "gpio.h"
#include "uart.h"
#include <avr/interrupt.h>

/*
Input capture on button press and release.
Resolution of 4us per tick, time out duration of 262ms.
Calculate the duration of each on and off pulse.
Print to serial.

Need to do check if surpass the number of allocated space
Need to adjust solution for even faster pulse
*/

#define MAX_SEQUENCE_NUMBER 150
volatile uint16_t pulse_sequence[MAX_SEQUENCE_NUMBER];
volatile uint8_t index = 0;
volatile uint8_t capture_done = 0;
volatile uint8_t out_of_bound = 0;

void test()
{
        // capture pin, PB0 = D8
        gpio_set_direction('B',0,GPIO_INPUT);
        gpio_write('B',0,GPIO_PULLUP_ENABLED);

        timer1_set_operation_mode(NORMAL_CLOCK);
        timer1_disable_input_capture_noise_canceler();
        timer1_set_input_capture_edge(FALLING_EDGE);

        sei();
        timer1_set_interrupt(INPUT_CAPTURE);
        timer1_set_interrupt(OVERFLOW);
        
        uart_init(9600, 
                RECEIVER_DISABLED, TRANSMITTER_ENABLED, 
                EIGHT_DATABITS, ONE_STOPBIT);

        while (1) {
                if (out_of_bound) {
                        uart_transmit_string("OUT OF BOUND!");
                } else if (capture_done) {
                        for (uint8_t j = 0; j < index; j++) {
                                uart_transmit_number(j);
                                uart_transmit_line();
                                if (j % 2 == 0) 
                                        uart_transmit_string("ON ");
                                else
                                        uart_transmit_string("OFF ");
                                uart_transmit_number(pulse_sequence[j]);
                                uart_transmit_line();
                        }

                        index = 0;
                        capture_done = 0;
                        timer1_set_operation_mode(NORMAL_CLOCK);
                        timer1_set_input_capture_edge(FALLING_EDGE);
                        sei();
                        timer1_set_interrupt(INPUT_CAPTURE);
                        timer1_set_interrupt(OVERFLOW);
                }
        }
}

ISR(TIMER1_CAPT_vect)
{
        if (index == MAX_SEQUENCE_NUMBER)
                out_of_bound = 1;
        pulse_sequence[index++] = timer1_read_input_capture();
        timer1_set_clock_source(PRESCALER_64);
        if (gpio_read('B',0) == GPIO_LOW)
                timer1_set_input_capture_edge(RISING_EDGE);
        else 
                timer1_set_input_capture_edge(FALLING_EDGE);
        timer1_set_counter_value(0);  
}

ISR(TIMER1_OVF_vect)
{
        cli();  
        timer1_set_clock_source(STOP_CLOCK);
        timer1_clear_interrupt(OVERFLOW);
        timer1_clear_interrupt(INPUT_CAPTURE);
        timer1_clear_interrupt_flag(OVERFLOW);
        timer1_clear_interrupt_flag(INPUT_CAPTURE);
        capture_done = 1;
}
