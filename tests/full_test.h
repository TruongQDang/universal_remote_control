#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#define RECORDING_MODE 0
#define PLAYBACK_MODE 1
#define MAX 68
uint16_t pulse_sequence[MAX] = {18116, 8939, 1185, 1061, 1198, 1049, 1191, 1057, 1186, 1059,
                                1191,1054,1193,1054,1191,1055,1197,1051,1187,3266,
                                1188,3267,1190,3261,1189,3263,1191,3262,1192,3261,
                                1191,3261,1193,3258,1204,3248,1196,1049,1194,3259,
                                1192,1054,1198,1050,1193,1050,1200,3254,1198,1047,
                                1260,986,1202,3250,1205,1042,1256,3194,1256,3198,
                                1254,3199,1254,991,1258,3197,1254,65000};
uint16_t pulse_sequence[MAX];
volatile uint16_t index = 0;
volatile uint8_t mode;

volatile uint8_t transmit_done = 1;
uint8_t playback_button_released = 1;

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


        if (gpio_read_inline('D',4) == PLAYBACK_MODE) {
                mode = PLAYBACK_MODE;
                timer0_set_operation_mode(FAST_PWM_OCR);
                timer0_set_compare_output_mode_A(TOGGLE_ON_MATCH);
                timer0_set_compare_value_A_inline(25);

                timer1_set_operation_mode(CTC_OCR);

                gpio_write('C',3,GPIO_HIGH);
        } else {
                mode = RECORDING_MODE;
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

                gpio_write('C',3,GPIO_LOW);
                gpio_write('C',4,GPIO_HIGH);                
        } else {
                mode = PLAYBACK_MODE;

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