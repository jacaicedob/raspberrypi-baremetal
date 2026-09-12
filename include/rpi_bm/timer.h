#ifndef RPIBM_TIMER_H
#define RPIBM_TIMER_H

#include "rpi_bm/bcm2837b0.h"

#include <stdint.h>
#include <stddef.h>

typedef enum {
    RPIBM_TIMER_CHANNEL1 = 1,
    RPIBM_TIMER_CHANNEL3 = 3,
    RPIBM_TIMER_CHANNEL_DEFAULT = 1
} rpibm_timer_channel_t;

void rpibm_timer_delay_us(uint32_t microseconds); // Uses default channel
void rpibm_timer_delay_us_ch(uint32_t microseconds, rpibm_timer_channel_t timer);

#endif // RPIBM_TIMER_H