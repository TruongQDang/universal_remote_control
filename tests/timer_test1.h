#include "timer.h"
#include "gpio.h"
#include "uart.h"
#include <avr/interrupt.h>

/*
Input capture on button press and release
Calculate the duration of each off pulse.
Resolution of 4us per tick, time out duration of 262ms 
*/
volatile uint16_t duration;
volatile uint8_t obtained = 0;
void test()
{
        gpio_set_direction('B',0,GPIO_INPUT);
        gpio_write('B',0,GPIO_PULLUP_ENABLED);
        timer1_set_operation_mode(NORMAL_CLOCK);
        timer1_disable_input_capture_noise_canceler();
        timer1_set_input_capture_edge(FALLING_EDGE);
        sei();
        timer1_set_interrupt(INPUT_CAPTURE);
        
        uart_init(9600, 
                RECEIVER_DISABLED, TRANSMITTER_ENABLED, 
                EIGHT_DATABITS, ONE_STOPBIT);

        while (1) {
                if (obtained) {
                        obtained = 0;
                        uart_transmit_number(duration);
                        uart_transmit_line();
                }
        }
}

ISR(TIMER1_CAPT_vect)
{
        if (gpio_read('B',0) == GPIO_LOW) {
                timer1_set_clock_source(PRESCALER_64);
                timer1_set_input_capture_edge(RISING_EDGE);
        } else {
                duration = timer1_read_input_capture();
                timer1_set_clock_source(STOP_CLOCK);
                timer1_set_counter_value(0); 
                timer1_set_input_capture_edge(FALLING_EDGE);
                obtained = 1;             
        }
        
}