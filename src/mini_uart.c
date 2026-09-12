#include "rpi_bm/mini_uart.h"
#include "rpi_bm/gpio.h"

#include <stdint.h>
#include <stddef.h>

static void rpibm_mini_uart_enable_aux();
static void rpibm_mini_uart_disable_aux();
static void rpibm_mini_uart_send_byte(char byte);
static void rpibm_mini_uart_enable_tx(void);
static void rpibm_mini_uart_disable_tx(void);
static void rpibm_mini_uart_set_baud(unsigned int baud);
static void rpibm_mini_uart_reset(void);
static void rpibm_mini_uart_set_data_size(rpibm_mini_uart_datasize_t data_size);

void rpibm_mini_uart_init(unsigned int baud)
{
    // Enable AUX
    rpibm_mini_uart_enable_aux();

    // Reset the UART (turn off TX & RX)
    rpibm_mini_uart_reset();

    // Set pin 14 (TXD1) to ALTFN5
    // Set pin 15 (RXD1) to ALTFN5
    rpibm_gpio_set_function(14, RPIBM_GPIO_ALTFN5);
    rpibm_gpio_set_function(15, RPIBM_GPIO_ALTFN5);

    // Set baud rate
    rpibm_mini_uart_set_baud(baud);

    // Set data size to 8 bits
    rpibm_mini_uart_set_data_size(RPIBM_MINI_UART_DATA_SIZE_8BIT);

    // Enable TX
    rpibm_mini_uart_enable_tx();
}

void rpibm_mini_uart_print(char *buffer)
{
    size_t i = 0;
    char   next_byte = buffer[i];

    while (next_byte != 0x0) {
        rpibm_mini_uart_send_byte(next_byte);
        next_byte = buffer[++i];
    }
}

static void rpibm_mini_uart_enable_aux()
{
    volatile uint32_t *addr = (volatile uint32_t *)RPIBM_AUX_ENABLES_ADDR;
    // Read register and add mini UART enable flag
    uint32_t aux_enables = *addr;
    *addr = (aux_enables | RPIBM_MINI_UART_AUX_EN);
}

static void rpibm_mini_uart_disable_aux()
{
    volatile uint32_t *addr = (volatile uint32_t *)RPIBM_AUX_ENABLES_ADDR;
    // Read register and remove mini UART enable flag
    uint32_t aux_enables = *addr;
    *addr = (aux_enables & ~RPIBM_MINI_UART_AUX_EN);
}

static void rpibm_mini_uart_reset(void)
{
    // Reset all entries in the CNTL register
    volatile uint32_t *addr;
    addr = (volatile uint32_t *)(RPIBM_MINI_UART_CNTL_ADDR);
    *addr = 0x0;

    // Disable all interrupts
    addr = (volatile uint32_t *)(RPIBM_MINI_UART_IER_ADDR);
    *addr = 0x0;
}

static void rpibm_mini_uart_enable_tx(void)
{
    volatile uint32_t *addr = (volatile uint32_t *)(RPIBM_MINI_UART_CNTL_ADDR);

    uint32_t cntl = *addr;
    uint32_t value = cntl | RPIBM_MINI_UART_TX_EN;
    *addr = value;
}

static void rpibm_mini_uart_disable_tx(void)
{
    volatile uint32_t *addr = (volatile uint32_t *)(RPIBM_MINI_UART_CNTL_ADDR);

    uint32_t cntl = *addr;
    uint32_t value = cntl & ~RPIBM_MINI_UART_TX_EN;
    *addr = value;
}

static void rpibm_mini_uart_set_baud(unsigned int baud_rate)
{
    /*
     * baud_rate = (system_clk_freq) / (8 * (baud_reg + 1))
     * baud_reg = (system_clk_freq/(8*baud_rate)) - 1
     * baud_reg = ((system_clk_freq/8)/baud_rate)) - 1
     */
    if (baud_rate == 0) {
        return;
    }
    volatile uint32_t *addr;

    // // Enable DLAB access
    // addr = (volatile uint32_t *)(RPIBM_MINI_UART_LCR_ADDR);
    // *addr = (uint32_t)RPIBM_MINI_UART_DLAB_EN;

    // Calculate value for BAUD register
    uint32_t baud_reg = ((SYSTEM_CLK_FREQ >> 3) / baud_rate) - 1;

    addr = (volatile uint32_t *)(RPIBM_MINI_UART_BAUD_ADDR);
    *addr = baud_reg & 0xFFFF; // Bits 31:16 must be 0

    // // Disable DLAB access
    // addr = (volatile uint32_t *)(RPIBM_MINI_UART_LCR_ADDR);
    // *addr = 0x0;
}

static void rpibm_mini_uart_send_byte(char byte)
{
    if (byte == '\n') {
        // Automatically send carriage return
        rpibm_mini_uart_send_byte('\r');
    }

    volatile uint32_t *addr;
    // Check TX FIFO has space
    addr = (volatile uint32_t *)(RPIBM_MINI_UART_STAT_ADDR);
    while (1) {
        uint32_t status = *addr;
        if (status & RPIBM_MINI_UART_SPACE_AVAILABLE) {
            // Write byte to FIFO
            addr = (volatile uint32_t *)(RPIBM_MINI_UART_IO_ADDR);
            *addr = (uint32_t)byte;
            break;
        }
    }
}

static void rpibm_mini_uart_set_data_size(rpibm_mini_uart_datasize_t data_size)
{
    volatile uint32_t *addr = (volatile uint32_t *)(RPIBM_MINI_UART_LCR_ADDR);
    *addr = (uint32_t)data_size;
}
