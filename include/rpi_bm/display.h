#ifndef RPIBM_DISPLAY_H
#define RPIBM_DISPLAY_H

#include "rpi_bm/fmt.h"
#include "rpi_bm/gpu_mbox.h"
#include "rpi_bm/mini_uart.h"

#define RPIBM_DISPLAY_MAX_HUE   1536
#define RPIBM_DISPLAY_MAX_WIDTH 1920

void rpibm_display_init(struct rpibm_frame_buffer *frame_buffer);
void rpibm_display_horizontal_gradient_grayscale(struct rpibm_frame_buffer *frame_buffer);
void rpibm_display_horizontal_gradient_grayscale_offset(struct rpibm_frame_buffer *frame_buffer,
                                                        uint32_t frame_buffer_index);
void rpibm_display_horizontal_gradient_rainbow(struct rpibm_frame_buffer *frame_buffer);
void rpibm_display_horizontal_gradient_rainbow_offset(struct rpibm_frame_buffer *frame_buffer,
                                                      uint32_t                   frame_buffer_index,
                                                      uint32_t                   phase_offset);

#endif // RPIBM_DISPLAY_H