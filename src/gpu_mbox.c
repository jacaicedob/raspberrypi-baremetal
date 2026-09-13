#include "rpi_bm/gpu_mbox.h"

static void rpibm_gpu_mbox_clear_responses();
static void rpibm_gpu_mbox_wait_write_not_full();
static void rpibm_gpu_mbox_wait_read_empty();
static void rpibm_gpu_mbox_send_request(rpibm_gpu_mbox_channel_t channel, uint32_t data);
static void rpibm_gpu_mbox_read_raw_response(rpibm_gpu_mbox_channel_t channel, uint32_t *response);
static void rpibm_gpu_mbox_read_response(rpibm_gpu_mbox_channel_t channel, uint32_t *response);
static void rpibm_gpu_mbox_validate_response(rpibm_gpu_mbox_channel_t channel, uint32_t response,
                                             uint8_t *valid);

static void rpibm_gpu_mbox_clear_responses()
{
    // TODO: Add timeout using timers
    volatile uint32_t *status_addr = (volatile uint32_t *)(RPIBM_MBOX_STATUS_ADDR);
    volatile uint32_t *read_addr = (volatile uint32_t *)(RPIBM_MBOX_READ_ADDR);
    while (1) {
        uint32_t status = *status_addr;
        if ((status & RPIBM_MBOX_STATUS_READ_EMPTY_MASK)) {
            break;
        }
        uint32_t dummy = *read_addr;
    }
}

static void rpibm_gpu_mbox_wait_write_not_full()
{
    // TODO: Add timeout using timers
    volatile uint32_t *status_addr = (volatile uint32_t *)(RPIBM_MBOX_STATUS_ADDR);
    while (1) {
        uint32_t status = *status_addr;
        if (!(status & RPIBM_MBOX_STATUS_WRITE_FULL_MASK)) {
            return;
        }
    }
}

static void rpibm_gpu_mbox_wait_read_empty()
{
    // TODO: Add timeout using timers
    volatile uint32_t *status_addr = (volatile uint32_t *)(RPIBM_MBOX_STATUS_ADDR);
    while (1) {
        uint32_t status = *status_addr;
        if (!(status & RPIBM_MBOX_STATUS_READ_EMPTY_MASK)) {
            return;
        }
    }
}

static void rpibm_gpu_mbox_send_request(rpibm_gpu_mbox_channel_t channel, uint32_t data)
{
    volatile uint32_t *write_addr = (volatile uint32_t *)(RPIBM_MBOX_WRITE_ADDR);
    uint32_t           request = (data << 4) | (channel);
    *write_addr = request;
}

static void rpibm_gpu_mbox_read_response(rpibm_gpu_mbox_channel_t channel, uint32_t *response)
{
    uint32_t tmp_response;
    rpibm_gpu_mbox_read_raw_response(channel, &tmp_response);
    uint8_t valid;
    rpibm_gpu_mbox_validate_response(channel, tmp_response, &valid);
    if (valid) {
        // Remove channel bits from response
        *response = tmp_response >> 4;
    }
}

static void rpibm_gpu_mbox_read_raw_response(rpibm_gpu_mbox_channel_t channel, uint32_t *response)
{
    volatile uint32_t *read_addr = (volatile uint32_t *)(RPIBM_MBOX_READ_ADDR);
    *response = *read_addr;
}

static void rpibm_gpu_mbox_validate_response(rpibm_gpu_mbox_channel_t channel, uint32_t response,
                                             uint8_t *valid)
{
    if ((response & RPIBM_MBOX_CHANNEL_MASK) != (uint32_t)channel) {
        *valid = 0;
    } else {
        *valid = 1;
    }
}

void rpibm_gpu_mbox_get_fw_version(uint32_t *fw_version)
{
    rpibm_gpu_mbox_clear_responses();
    rpibm_gpu_mbox_wait_write_not_full();
    rpibm_gpu_mbox_send_request(RPIBM_MBOX_PROPERTY_TAGS_CH, RPIBM_MBOX_TAGS_FW_VERSION);
    rpibm_gpu_mbox_wait_read_empty();
    rpibm_gpu_mbox_read_raw_response(RPIBM_MBOX_PROPERTY_TAGS_CH, fw_version);
}