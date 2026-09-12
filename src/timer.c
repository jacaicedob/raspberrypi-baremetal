#include "rpi_bm/timer.h"

#include <stdint.h>
#include <stddef.h>

// Is it worht making these public at some point to allow low-level control?
static void rpibm_timer_set_timer(uint8_t timer, uint32_t ticks);
static void rpibm_timer_wait_for_match(uint8_t timer);
static void rpibm_timer_clear(uint8_t timer);

// Private functions
static uint32_t timer_match_mask(uint8_t timer);
static size_t   timer_compare_offset(uint8_t timer);

void rpibm_timer_delay_us(uint32_t microseconds)
{
    // Convenience function that is the main use of this libary
    // Uses default timer channel
    rpibm_timer_delay_us_ch(microseconds, (uint8_t)RPIBM_TIMER_CHANNEL_DEFAULT);
}

void rpibm_timer_delay_us_ch(uint32_t microseconds, uint8_t timer)
{
    // Read current CLO register
    volatile uint32_t *addr_clo = (volatile uint32_t *)(RPIBM_TIMER_CLO_ADDR);
    uint32_t           clo = *addr_clo;

    // Add microsecodns to CLO and poll the match registers
    rpibm_timer_set_timer(timer, clo + microseconds);
    rpibm_timer_wait_for_match(timer);
    rpibm_timer_clear(timer);
}

void rpibm_timer_set_timer(uint8_t timer, uint32_t ticks)
{
    volatile uint32_t *addr =
        (volatile uint32_t *)(RPIBM_TIMER_C0_ADDR + timer_compare_offset(timer));
    *addr = ticks;
}

void rpibm_timer_wait_for_match(uint8_t timer)
{
    uint32_t mask = timer_match_mask(timer);
    while (1) {
        volatile uint32_t *addr = (volatile uint32_t *)(RPIBM_TIMER_CS_ADDR);
        if (*addr & mask) {
            return;
        }
    }
}

void rpibm_timer_clear(uint8_t timer)
{
    uint32_t           mask = timer_match_mask(timer);
    volatile uint32_t *addr = (volatile uint32_t *)(RPIBM_TIMER_CS_ADDR);
    *addr = mask;
}

static uint32_t timer_match_mask(uint8_t timer) { return (uint32_t)(1 << (timer % 4)); }

static size_t timer_compare_offset(uint8_t timer)
{
    return (size_t)((timer % 4) * sizeof(uint32_t));
}