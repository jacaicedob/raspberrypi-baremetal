#include "rpi_bm/fmt.h"
#include "rpi_bm/gpio.h"
#include "rpi_bm/gpu_mbox.h"
#include "rpi_bm/timer.h"
#include "rpi_bm/mini_uart.h"
#include "rpi_bm/display.h"

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

void main(void)
{
    char     buffer[256];
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

    // Get FW Revision from GPU via MBOX interface
    int32_t fw_revision;
    rpibm_gpu_mbox_get_fw_version(&fw_revision);
    rpibm_fmt_itoa(fw_revision, RPIBM_FMT_BASE_HEX, buffer);
    rpibm_mini_uart_print("FW Revision: 0x");
    rpibm_mini_uart_print(buffer);
    rpibm_mini_uart_print("\n");
    rpibm_fmt_itoa(fw_revision, RPIBM_FMT_BASE_DEC, buffer);
    rpibm_mini_uart_print("FW Revision: ");
    rpibm_mini_uart_print(buffer);
    rpibm_mini_uart_print("\n");

    // Get MAC address from GPU via MBOX interface
    uint8_t mac_address[6];
    rpibm_gpu_mbox_get_mac_address(mac_address);

    rpibm_mini_uart_print("MAC Addr: ");
    for (size_t i = 0; i < sizeof(mac_address); ++i) {
        rpibm_fmt_itoa_zeropad(mac_address[i], RPIBM_FMT_BASE_HEX, buffer, 2);
        rpibm_mini_uart_print(buffer);
        rpibm_mini_uart_print(" ");
    }
    rpibm_mini_uart_print("\n");

    // Allocate frame buffer from GPU
    struct rpibm_frame_buffer frame_buffer;
    frame_buffer.phy_width = 1920;
    frame_buffer.phy_height = 1080;
    frame_buffer.virt_width = frame_buffer.phy_width;
    frame_buffer.virt_height = 2 * frame_buffer.phy_height;
    frame_buffer.depth = 32;

    rpibm_mini_uart_print("Initializing frame buffer\n");
    rpibm_display_init(&frame_buffer);
    rpibm_mini_uart_print("Drawing rainbow gradient\n");
    rpibm_display_horizontal_gradient_rainbow_offset(&frame_buffer, 0);
    rpibm_mini_uart_print("Drawing grayscale gradient\n");
    rpibm_display_horizontal_gradient_grayscale_offset(&frame_buffer, frame_buffer.buffer_size / 2);

    size_t counter = 0;

    // Blink LEDs
    rpibm_mini_uart_print("Blinking LEDs\n");
    while (1) {
        // Set pins to high
        for (int i = 0; i < 2; ++i) {
            uint8_t pin = pins[i];
            rpibm_gpio_set_high(pin);
        }
        rpibm_timer_delay_us(sleep_us);

        // Set pins to low
        for (int i = 0; i < 2; ++i) {
            uint8_t pin = pins[i];
            rpibm_gpio_set_low(pin);
        }

        rpibm_timer_delay_us(sleep_us);

        if ((counter % 2) == 0) {
            rpibm_gpu_mbox_set_virtual_offset(0, frame_buffer.phy_height);
        } else {

            rpibm_gpu_mbox_set_virtual_offset(0, 0);
        }
        ++counter;
    }
}