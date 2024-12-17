#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include "eeprom.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#define RECORDING_MODE 0
#define PLAYBACK_MODE 1
#define MAX 500
uint16_t pulse_sequence[MAX];
volatile uint16_t index = 0;
uint16_t index_cap = 0;
volatile uint8_t mode;

volatile uint8_t playback_done = 1;
uint8_t playback_button_released = 1;

uint8_t record_button_released = 1;
volatile uint8_t record_done = 1;
volatile uint8_t need_post_processing = 0;

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

        // retrieve data from eeprom
        cli();
        uint16_t address = 0;
        index_cap = eeprom_read_word(address);
        for (uint16_t i = 0; i < index_cap; i++) {
                address += 2;
                pulse_sequence[i] = eeprom_read_word(address);
        }
        sei();
        

        if (gpio_read_inline('D',4) == PLAYBACK_MODE) {
                mode = PLAYBACK_MODE;

                // setup
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
                        if (gpio_read('D',3) == GPIO_LOW && playback_done && playback_button_released) {
                                playback_done = 0;
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
                        if (need_post_processing) {
                                // process the signal data and store into eeprom
                                index_cap = index - 1;
                                
                                cli();
                                uint16_t address = 0;
                                eeprom_write_word(address, index_cap);
                                for (uint16_t j = 0; j < index_cap; j++) {
                                        pulse_sequence[j] = pulse_sequence[j+1];
                                        address += 2;
                                        eeprom_write_word(address ,pulse_sequence[j+1]); 
                                }
                                sei();  

                                need_post_processing = 0;
                        }

                        if (gpio_read('D',2) == GPIO_LOW && record_done && record_button_released) {
                                record_done = 0;
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
        if (index < index_cap) {
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
                
                playback_done = 1;
                gpio_write('C',3,GPIO_HIGH);
        }
}

ISR(TIMER1_OVF_vect)
{
        timer1_clear_interrupt(INPUT_CAPTURE);
        timer1_set_clock_source(STOP_CLOCK);
        timer1_clear_interrupt(OVERFLOW); 
        record_done = 1;
        need_post_processing = 1;
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
