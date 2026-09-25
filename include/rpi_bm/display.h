#ifndef RPIBM_DISPLAY_H
#define RPIBM_DISPLAY_H

#include "rpi_bm/fmt.h"
#include "rpi_bm/gpu_mbox.h"
#include "rpi_bm/mini_uart.h"

void rpibm_display_init(struct rpibm_frame_buffer *frame_buffer);
void rpibm_display_horizontal_gradient_grayscale(struct rpibm_frame_buffer *frame_buffer);
void rpibm_display_horizontal_gradient_rainbow(struct rpibm_frame_buffer *frame_buffer);

#endif // RPIBM_DISPLAY_H