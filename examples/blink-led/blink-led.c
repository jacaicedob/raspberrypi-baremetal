#include "rpi_bm/gpio.h"
#include "rpi_bm/timer.h"
#include "rpi_bm/mini_uart.h"

#include <stdint.h>

void main(void)
{
    uint32_t sleep_us = 500000U;
    uint8_t  pins[2];
    pins[0] = 19;
    pins[1] = 26;

    // Set pins as output
    for (int i = 0; i < 2; ++i) {
        uint8_t pin = pins[i];
        rpibm_gpio_set_function(pin, RPIBM_GPIO_OUTPUT);
    }

    rpibm_mini_uart_init(115200U);
    rpibm_mini_uart_print("Hello, World!\n");

    while (1) {
        // Set pins to high
        for (int i = 0; i < 2; ++i) {
            uint8_t pin = pins[i];
            rpibm_gpio_set_high(pin);
        }
        rpibm_mini_uart_print("LED High\n");
        rpibm_timer_delay_us(sleep_us);

        // Set pins to low
        for (int i = 0; i < 2; ++i) {
            uint8_t pin = pins[i];
            rpibm_gpio_set_low(pin);
        }

        rpibm_mini_uart_print("LED Low\n");
        rpibm_timer_delay_us(sleep_us);
    }
}