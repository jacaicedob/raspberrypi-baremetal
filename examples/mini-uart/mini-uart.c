#include "rpi_bm/gpio.h"
#include "rpi_bm/timer.h"
#include "rpi_bm/mini_uart.h"

#include <stdint.h>

void main(void)
{
    uint32_t sleep_us = 500000U;

    rpibm_mini_uart_init(115200U);
    rpibm_mini_uart_print("Hello, World!\n");

    while (1) {
        rpibm_mini_uart_print("Message 1\n");
        rpibm_timer_delay_us(sleep_us);

        rpibm_mini_uart_print("Message 2\n");
        rpibm_timer_delay_us(sleep_us);
    }
}