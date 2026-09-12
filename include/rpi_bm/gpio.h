#ifndef RPIBM_GPIO_H
#define RPIBM_GPIO_H

#include "rpi_bm/bcm2837b0.h"

#include <stdint.h>
#include <stddef.h>

#define RPIBM_GPIO_MAX_PIN 53

#define RPIBM_GPIO_INPUT  0x0
#define RPIBM_GPIO_OUTPUT 0x1
#define RPIBM_GPIO_ALTFN0 0x4
#define RPIBM_GPIO_ALTFN1 0x5
#define RPIBM_GPIO_ALTFN2 0x6
#define RPIBM_GPIO_ALTFN3 0x7
#define RPIBM_GPIO_ALTFN4 0x3
#define RPIBM_GPIO_ALTFN5 0x2

void rpibm_gpio_set_function(uint8_t pin, uint8_t function);
void rpibm_gpio_set_high(uint8_t pin);
void rpibm_gpio_set_low(uint8_t pin);

#endif // RPIBM_GPIO_H