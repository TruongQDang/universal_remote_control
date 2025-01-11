#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#define RECORDING_MODE 0
#define PLAYBACK_MODE 1
#define MAX 500
uint16_t pulse_sequence[MAX];
volatile uint16_t index = 0;
volatile uint8_t mode;

volatile uint8_t transmit_done = 1;
uint8_t playback_button_released = 1;

uint8_t record_button_released = 1;
volatile uint8_t can_transmit = 0;
volatile uint8_t capture_done = 1;

void test()
{
        // switch state pin, D4
        gpio_set_direction('D',4,GPIO_INPUT);
        gpio_write('D',4,GPIO_PULLUP_ENABLED);
        gpio_enable_pin_change_interrupt(PCINT_16_23,PCINT20);
        sei();

        // ir led, D6 
        gpio_set_direction('D',6,GPIO_OUTPUT);
        
        // playback button, D3
        gpio_set_direction('D',3,GPIO_INPUT);
        gpio_write('D',3,GPIO_PULLUP_ENABLED); 

        // record button, D2
        gpio_set_direction('D',2,GPIO_INPUT);
        gpio_write('D',2,GPIO_PULLUP_ENABLED);         

        // ir receiver, PB0 = D8
        gpio_set_direction('B',0,GPIO_INPUT);
        gpio_write('B',0,GPIO_PULLUP_ENABLED);

        // led for playback state, PC3 = A3
        gpio_set_direction('C',3,GPIO_OUTPUT);

        // led for recording state, PC4 = A4
        gpio_set_direction('C',4,GPIO_OUTPUT);

        uart_init(9600, 
                RECEIVER_DISABLED, TRANSMITTER_ENABLED, 
                EIGHT_DATABITS, ONE_STOPBIT);

        if (gpio_read_inline('D',4) == PLAYBACK_MODE) {
                mode = PLAYBACK_MODE;

                timer0_set_operation_mode(FAST_PWM_OCR);
                timer0_set_compare_output_mode_A(TOGGLE_ON_MATCH);
                timer0_set_compare_value_A_inline(25);

                timer1_set_operation_mode(CTC_OCR);

                gpio_write('C',3,GPIO_HIGH);
        } else {
                mode = RECORDING_MODE;

                timer1_set_operation_mode(NORMAL_CLOCK);

                gpio_write('C',4,GPIO_HIGH);
        }

        
        while (1) {
                if (mode == PLAYBACK_MODE) {
                        if (gpio_read('D',3) == GPIO_LOW && transmit_done && playback_button_released) {
                                transmit_done = 0;
                                playback_button_released = 0;
                                gpio_write('C',3,GPIO_LOW);

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
                                playback_button_released = 1;
                } else if (mode == RECORDING_MODE) {
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

                        if (gpio_read('D',2) == GPIO_LOW && capture_done && record_button_released) {
                                capture_done = 0;
                                index = 0;     
                                record_button_released = 0;   
                                
                                timer1_set_counter_value_inline(0);
                                timer1_set_input_capture_edge_inline(FALLING_EDGE);
                                timer1_clear_interrupt_flag(OVERFLOW);
                                timer1_clear_interrupt_flag(INPUT_CAPTURE);
                                timer1_set_interrupt(INPUT_CAPTURE);
                                timer1_set_interrupt(OVERFLOW);
                        }
                        if (gpio_read('D',2) == GPIO_HIGH) 
                                record_button_released = 1;
                }
                
        }
        
}

ISR(PCINT2_vect)
{
        if (gpio_read_inline('D',4) == RECORDING_MODE) {
                mode = RECORDING_MODE;

                // disable previous timers
                timer0_set_clock_source(STOP_CLOCK);
                timer1_set_clock_source(STOP_CLOCK);
                timer1_clear_interrupt(COMPARE_MATCH_A);

                // enable new timers
                timer1_set_operation_mode(NORMAL_CLOCK);

                gpio_write('C',3,GPIO_LOW);
                gpio_write('C',4,GPIO_HIGH);                
        } else {
                mode = PLAYBACK_MODE;

                // disable previous timers
                timer1_set_clock_source(STOP_CLOCK);
                timer1_clear_interrupt(OVERFLOW);
                timer1_clear_interrupt(INPUT_CAPTURE);

                // enable new timers
                timer0_set_operation_mode(FAST_PWM_OCR);
                timer0_set_compare_output_mode_A(TOGGLE_ON_MATCH);
                timer0_set_compare_value_A_inline(25);

                timer1_set_operation_mode(CTC_OCR);

                gpio_write('C',4,GPIO_LOW);
                gpio_write('C',3,GPIO_HIGH);
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
                gpio_write('C',3,GPIO_HIGH);
        }
}

ISR(TIMER1_OVF_vect)
{
        timer1_clear_interrupt(INPUT_CAPTURE);
        timer1_set_clock_source(STOP_CLOCK);
        timer1_clear_interrupt(OVERFLOW); 
        capture_done = 1;
        can_transmit = 1;
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
