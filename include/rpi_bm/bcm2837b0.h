#ifndef BCM2837B0_H
#define BCM2837B0_H

#define PERIPH_BASE             0x3F000000

#define GPIO_OFFSET             0x00200000
#define GPIO_GPFSEL0_OFFSET     0x00
#define GPIO_GPFSEL1_OFFSET     0x04
#define GPIO_GPFSEL2_OFFSET     0x08
#define GPIO_GPFSEL3_OFFSET     0x0C
#define GPIO_GPFSEL4_OFFSET     0x10
#define GPIO_GPFSEL5_OFFSET     0x14
#define GPIO_GPSET0_OFFSET      0x1C
#define GPIO_GPSET1_OFFSET      0x20
#define GPIO_GPCLR0_OFFSET      0x28
#define GPIO_GPCLR1_OFFSET      0x2C

#define GPIO_BASE               (PERIPH_BASE + GPIO_OFFSET)
#define GPFSEL0                 (GPIO_BASE + GPIO_GPFSEL0_OFFSET)
#define GPFSEL1                 (GPIO_BASE + GPIO_GPFSEL1_OFFSET)
#define GPFSEL2                 (GPIO_BASE + GPIO_GPFSEL2_OFFSET)
#define GPFSEL3                 (GPIO_BASE + GPIO_GPFSEL3_OFFSET)
#define GPFSEL4                 (GPIO_BASE + GPIO_GPFSEL4_OFFSET)
#define GPFSEL5                 (GPIO_BASE + GPIO_GPFSEL5_OFFSET)
#define GPSET0                  (GPIO_BASE + GPIO_GPSET0_OFFSET)
#define GPSET1                  (GPIO_BASE + GPIO_GPSET1_OFFSET)
#define GPCLR0                  (GPIO_BASE + GPIO_GPCLR0_OFFSET)
#define GPCLR1                  (GPIO_BASE + GPIO_GPCLR1_OFFSET)

#define GPFSEL_INPUT_VALUE      0x0
#define GPFSEL_OUTPUT_VALUE     0x1

#endif // BCM2837B0_H