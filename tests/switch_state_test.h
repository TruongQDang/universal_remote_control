#include "gpio.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#define RECORDING_STATE 0
#define PLAYBACK_STATE 1

void test()
{
        // switch state pin, D4
        gpio_set_direction('D',4,GPIO_INPUT);
        gpio_write('D',4,GPIO_PULLUP_ENABLED);
        gpio_enable_pin_change_interrupt(PCINT_16_23,PCINT20);
        sei();

        // led for playback state, PC3 = A3
        gpio_set_direction('C',3,GPIO_OUTPUT);

        // led for recording state, PC4 = A4
        gpio_set_direction('C',4,GPIO_OUTPUT);

        uart_init(9600, 
                RECEIVER_DISABLED, TRANSMITTER_ENABLED, 
                EIGHT_DATABITS, ONE_STOPBIT);
        
        while (1) {
                uart_transmit_number(gpio_read_inline('D',4));
                uart_transmit_line();
        }
        
}

ISR(PCINT2_vect)
{
        if (gpio_read_inline('D',4) == RECORDING_STATE) {
                gpio_write('C',3,GPIO_LOW);
                gpio_write('C',4,GPIO_HIGH);
        } else {
                gpio_write('C',4,GPIO_LOW);
                gpio_write('C',3,GPIO_HIGH);
        }
}
