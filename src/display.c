#include "rpi_bm/display.h"

void rpibm_display_init(struct rpibm_frame_buffer *frame_buffer)
{
    char buffer[256];
    rpibm_gpu_mbox_allocate_frame_buffer(frame_buffer);
    rpibm_gpu_mbox_get_pitch(frame_buffer);

    // Print buffer details
    rpibm_mini_uart_print("Frame buffer details:\n");
    rpibm_mini_uart_print("  Physical dimensions (WxH): ");
    rpibm_fmt_itoa(frame_buffer->phy_width, RPIBM_FMT_BASE_DEC, buffer);
    rpibm_mini_uart_print(buffer);
    rpibm_mini_uart_print("x");
    rpibm_fmt_itoa(frame_buffer->phy_height, RPIBM_FMT_BASE_DEC, buffer);
    rpibm_mini_uart_print(buffer);
    rpibm_mini_uart_print("\n");
    rpibm_mini_uart_print("  Virtual dimensions (WxH): ");
    rpibm_fmt_itoa(frame_buffer->virt_width, RPIBM_FMT_BASE_DEC, buffer);
    rpibm_mini_uart_print(buffer);
    rpibm_mini_uart_print("x");
    rpibm_fmt_itoa(frame_buffer->virt_height, RPIBM_FMT_BASE_DEC, buffer);
    rpibm_mini_uart_print(buffer);
    rpibm_mini_uart_print("\n");
    rpibm_mini_uart_print("  Pixel Depth (bits per pixel): ");
    rpibm_fmt_itoa(frame_buffer->depth, RPIBM_FMT_BASE_DEC, buffer);
    rpibm_mini_uart_print(buffer);
    rpibm_mini_uart_print("\n");
    rpibm_mini_uart_print("  Pitch: ");
    rpibm_fmt_itoa(frame_buffer->pitch, RPIBM_FMT_BASE_DEC, buffer);
    rpibm_mini_uart_print(buffer);
    rpibm_mini_uart_print("\n");
    rpibm_mini_uart_print("  Buffer size (bytes): ");
    rpibm_fmt_itoa(frame_buffer->buffer_size, RPIBM_FMT_BASE_DEC, buffer);
    rpibm_mini_uart_print(buffer);
    rpibm_mini_uart_print("\n");
    rpibm_mini_uart_print("  Buffer base addr: ");
    rpibm_fmt_itoa((uint32_t)frame_buffer->buffer, RPIBM_FMT_BASE_HEX, buffer);
    rpibm_mini_uart_print(buffer);
    rpibm_mini_uart_print("\n");
}

void rpibm_display_horizontal_gradient_grayscale(struct rpibm_frame_buffer *frame_buffer)
{
    rpibm_display_horizontal_gradient_grayscale_offset(frame_buffer, 0);
}

void rpibm_display_horizontal_gradient_grayscale_offset(struct rpibm_frame_buffer *frame_buffer,
                                                        uint32_t                   offset)
{
    uint8_t *base_offset = (uint8_t *)(frame_buffer->buffer) + offset;
    for (uint32_t row = 0; row < frame_buffer->phy_height; ++row) {
        uint32_t row_offset = (uint32_t)(base_offset + (row * frame_buffer->pitch));
        for (uint32_t col = 0; col < frame_buffer->phy_width; ++col) {
            uint32_t pixel_offset = row_offset + col * (frame_buffer->depth / 8);
            uint8_t  r = (col % 256);
            uint8_t  g = (col % 256);
            uint8_t  b = (col % 256);
            uint8_t  alpha = 0xFF;
            *(uint32_t *)pixel_offset = alpha << 24 | r << 16 | g << 8 | b;
        }
    }
}

void rpibm_display_horizontal_gradient_rainbow(struct rpibm_frame_buffer *frame_buffer)
{
    rpibm_display_horizontal_gradient_rainbow_offset(frame_buffer, 0);
}

void rpibm_display_horizontal_gradient_rainbow_offset(struct rpibm_frame_buffer *frame_buffer,
                                                      uint32_t                   offset)
{
    uint32_t max_hue = 256 * 6;

    uint8_t *base_offset = (uint8_t *)(frame_buffer->buffer) + offset;
    for (uint32_t row = 0; row < frame_buffer->phy_height; ++row) {
        uint32_t row_offset = (uint32_t)(base_offset + (row * frame_buffer->pitch));
        for (uint32_t col = 0; col < frame_buffer->phy_width; ++col) {
            uint32_t pixel_offset = row_offset + col * (frame_buffer->depth / 8);
            uint8_t  alpha = 0xFF;
            uint32_t hue = col % max_hue;

            uint8_t r, g, b;
            if (hue < 256) {
                r = 255;
                b = 0;
                g = hue;
            } else if (hue < 512) {
                r = (255 - (hue - 256));
                b = 0;
                g = 255;
            } else if (hue < 768) {
                r = 0;
                b = hue - 512;
                g = 255;
            } else if (hue < 1024) {
                r = 0;
                b = 255;
                g = (255 - (hue - 768));
            } else if (hue < 1280) {
                r = hue - 768;
                b = 255;
                g = 0;
            } else if (hue < max_hue) {
                r = 255;
                b = (255 - (hue - 1280));
                g = 0;
            }

            *(uint32_t *)pixel_offset = alpha << 24 | r << 16 | g << 8 | b;
        }
    }
}
