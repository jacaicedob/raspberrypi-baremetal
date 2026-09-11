#include "bcm2837b0.h"
#include <stdint.h>

void main(void){
    volatile uint32_t *addr;
    uint32_t value;
    uint8_t pins[2];
    pins[0] = 17;
    pins[1] = 18;

    // Set pins as output
    // GPFSEL1 
    value = 0x0;
    addr = (volatile uint32_t *)(GPFSEL0) + 1;
    *(addr) = value;
    for (int i=0; i < 2; ++i)
    {
        uint8_t pin = pins[i];
        value = value | (GPFSEL_OUTPUT_VALUE << ((pin % 10)*3));
    }
    *(addr) = value;

    // Configure SET and CLR data for pins
    value = 0x0;
    for (int i=0; i < 2; ++i)
    {
        char pin = pins[i];
        value = value | (1 << pin);
    }

    while(1){
        // Set pins to high
        addr = (volatile uint32_t *)(GPSET0);
        *(addr) = value;

        for(volatile uint32_t i=0; i < (1 << 20); ++i){
        }

        // Set pins to low
        addr = (volatile uint32_t *)(GPCLR0);
        *(addr) = value;

        for(volatile uint32_t i=0; i < (1 << 20); ++i){
        }

    }
}