#ifndef RPIBM_TIMER_H
#define RPIBM_TIMER_H

#include "rpi_bm/bcm2837b0.h"

#include <stdint.h>
#include <stddef.h>

#define RPIBM_TIMER_CHANNEL1        1
#define RPIBM_TIMER_CHANNEL3        3
#define RPIBM_TIMER_CHANNEL_DEFAULT (RPIBM_TIMER_CHANNEL1)

void rpibm_timer_delay_us(uint32_t microseconds); // Uses default channel
void rpibm_timer_delay_us_ch(uint32_t microseconds, uint8_t timer);

#endif // RPIBM_TIMER_H