#include "rpi_bm/gpio.h"
#include <stdint.h>

void main(void)
{
    volatile uint32_t *addr;
    uint32_t           value;
    uint8_t            pins[2];
    pins[0] = 17;
    pins[1] = 18;

    // Set pins as output
    // GPFSEL1
    for (int i = 0; i < 2; ++i) {
        uint8_t pin = pins[i];
        rpibm_gpio_set_function(pin, RPIBM_GPIO_OUTPUT);
    }

    while (1) {
        // Set pins to high
        for (int i = 0; i < 2; ++i) {
            uint8_t pin = pins[i];
            rpibm_gpio_set_high(pin);
        }

        for (volatile uint32_t i = 0; i < (1 << 20); ++i) {
        }

        // Set pins to low
        for (int i = 0; i < 2; ++i) {
            uint8_t pin = pins[i];
            rpibm_gpio_set_low(pin);
        }

        for (volatile uint32_t i = 0; i < (1 << 20); ++i) {
        }
    }
}