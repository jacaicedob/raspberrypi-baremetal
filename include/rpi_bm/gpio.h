#ifndef RPIBM_GPIO_H
#define RPIBM_GPIO_H

#include "rpi_bm/bcm2837b0.h"

#include <stdint.h>
#include <stddef.h>

#define RPIBM_GPIO_MAX_PIN 53

typedef enum {
    RPIBM_GPIO_INPUT = 0x0,
    RPIBM_GPIO_OUTPUT = 0x1,
    RPIBM_GPIO_ALTFN0 = 0x4,
    RPIBM_GPIO_ALTFN1 = 0x5,
    RPIBM_GPIO_ALTFN2 = 0x6,
    RPIBM_GPIO_ALTFN3 = 0x7,
    RPIBM_GPIO_ALTFN4 = 0x3,
    RPIBM_GPIO_ALTFN5 = 0x2
} rpibm_gpio_function_t;

void rpibm_gpio_set_function(uint8_t pin, rpibm_gpio_function_t function);
void rpibm_gpio_set_high(uint8_t pin);
void rpibm_gpio_set_low(uint8_t pin);

#endif // RPIBM_GPIO_H