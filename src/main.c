#include <avr/io.h>
#include "gpio.h"
#include <util/delay.h>

int main()
{
        gpio_set_direction('B',5,GPIO_OUTPUT);
        while (1) {
                gpio_toggle('B',5);
                _delay_ms(300);
                gpio_toggle('B',5);
                _delay_ms(300);
        }
}