#include "timer.h"
#include "gpio.h"
#include "uart.h"
#include <avr/interrupt.h>

/*
Input capture on button press and release.
Resolution of 0.5us per tick, time out duration of 32.7ms.
Calculate the duration of each on and off pulse.
Print to serial.

Need to do check if surpass the number of allocated space
Need to adjust solution for even faster pulse
*/

#define MAX_SEQUENCE_NUMBER 500
volatile uint16_t pulse_sequence[MAX_SEQUENCE_NUMBER];
volatile uint16_t index = 0;
volatile uint8_t capture_done = 0;
volatile uint8_t can_transmit = 0;
uint8_t button_released = 1;

void test()
{
        // capture pin, PB0 = D8
        gpio_set_direction('B',0,GPIO_INPUT);
        gpio_write('B',0,GPIO_PULLUP_ENABLED);

        // button pin, D2
        gpio_set_direction('D',2,GPIO_INPUT);
        gpio_write('D',2,GPIO_PULLUP_ENABLED); 

        // capture timer
        timer1_set_operation_mode(NORMAL_CLOCK);
        timer1_set_input_capture_edge_inline(FALLING_EDGE);
        timer1_set_counter_value_inline(0);

        timer1_set_interrupt(INPUT_CAPTURE);
        timer1_set_interrupt(OVERFLOW);
        sei();
        
        uart_init(9600, 
                RECEIVER_DISABLED, TRANSMITTER_ENABLED, 
                EIGHT_DATABITS, ONE_STOPBIT);

        while (1) {
                if (can_transmit) {
                        uart_transmit_string("BEGIN");
                        uart_transmit_line();
                        for (uint16_t j = 0; j < index; j++) {
                                uart_transmit_number(j);
                                uart_transmit_line();
                                if (j % 2 == 0) 
                                        uart_transmit_string("ON ");
                                else
                                        uart_transmit_string("OFF ");
                                uart_transmit_number(pulse_sequence[j]);
                                uart_transmit_line();
                        }
                        uart_transmit_string("END");
                        uart_transmit_line();
                        can_transmit = 0;
                }

                if (gpio_read('D',2) == GPIO_LOW && capture_done && button_released) {
                        capture_done = 0;
                        index = 0;     
                        button_released = 0;   
                        
                        timer1_set_counter_value_inline(0);
                        timer1_set_input_capture_edge_inline(FALLING_EDGE);
                        timer1_clear_interrupt_flag(OVERFLOW);
                        timer1_clear_interrupt_flag(INPUT_CAPTURE);
                        timer1_set_interrupt(INPUT_CAPTURE);
                        timer1_set_interrupt(OVERFLOW);
                }
                if (gpio_read('D',2) == GPIO_HIGH) 
                        button_released = 1;

                
        }
}

ISR(TIMER1_CAPT_vect)
{
        pulse_sequence[index++] = timer1_read_input_capture_inline();
        if (gpio_read_inline('B',0) == GPIO_LOW)
                timer1_set_input_capture_edge_inline(RISING_EDGE);
        else 
                timer1_set_input_capture_edge_inline(FALLING_EDGE);
        timer1_set_clock_source_inline(PRESCALER_8);
        timer1_set_counter_value_inline(0);
}

ISR(TIMER1_OVF_vect)
{
        timer1_clear_interrupt(INPUT_CAPTURE);
        timer1_set_clock_source(STOP_CLOCK);
        timer1_clear_interrupt(OVERFLOW); 
        capture_done = 1;
        can_transmit = 1;
}
