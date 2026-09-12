#include "rpi_bm/gpio.h"

#include <stdint.h>
#include <stddef.h>

static size_t function_register_offset(uint8_t pin);
static size_t function_bit_offset(uint8_t pin);
static size_t output_register_offset(uint8_t pin);
static size_t output_bit_offset(uint8_t pin);

void rpibm_gpio_set_function(uint8_t pin, uint8_t function)
{
    volatile uint32_t *addr =
        (volatile uint32_t *)(RPIBM_GPIO_GPFSEL0_ADDR + function_register_offset(pin));
    *addr = (uint32_t)(function << function_bit_offset(pin));
}

void rpibm_gpio_set_high(uint8_t pin)
{
    volatile uint32_t *addr =
        (volatile uint32_t *)(RPIBM_GPIO_GPSET0_ADDR + output_register_offset(pin));
    *addr = (uint32_t)(1 << output_bit_offset(pin));
}
void rpibm_gpio_set_low(uint8_t pin)
{
    volatile uint32_t *addr =
        (volatile uint32_t *)(RPIBM_GPIO_GPCLR0_ADDR + output_register_offset(pin));
    *addr = (uint32_t)(1 << output_bit_offset(pin));
}

static size_t function_register_offset(uint8_t pin)
{
    // Calculates the offset for the GPFSEL register that corresponds to the pin
    // Each register configures 10 pins
    return (size_t)((pin / 10) * sizeof(uint32_t));
}

static size_t function_bit_offset(uint8_t pin)
{
    // Calculates the offset inside the GPFSEL register to write the pin function
    // The config values are 3 bits each and each GPFSEL register hold 10 pins
    // Example: Pin 15 maps to GPFSEL1 and bit 15
    return (size_t)((pin % 10) * 3);
}

static size_t output_register_offset(uint8_t pin)
{
    // GPSET and GPCLR registers have 1 bit per pin
    // GPSET0 and GPCLR0 hold pins 0-31
    // GPSET1 and GPCLR1 hold ping 32-53

    return (size_t)((pin / 32) * sizeof(uint32_t));
}

static size_t output_bit_offset(uint8_t pin)
{
    // GPSET and GPCLR registers have 1 bit per pin
    // GPSET0 and GPCLR0 hold pins 0-31
    // GPSET1 and GPCLR1 hold ping 32-53
    return (size_t)(pin % 32);
}