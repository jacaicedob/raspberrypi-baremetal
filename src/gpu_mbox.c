#include "rpi_bm/gpu_mbox.h"

static void rpibm_gpu_mbox_clear_responses();
static void rpibm_gpu_mbox_wait_write_not_full();
static void rpibm_gpu_mbox_wait_read_empty();
static void rpibm_gpu_mbox_send_request(rpibm_gpu_mbox_channel_t channel, uint32_t data);
static void rpibm_gpu_mbox_read_raw_response(rpibm_gpu_mbox_channel_t channel, uint32_t *response);
static void rpibm_gpu_mbox_read_response(rpibm_gpu_mbox_channel_t                    channel,
                                         struct rpibm_gpu_mbox_read_response_struct *response);
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

static void rpibm_gpu_mbox_send_request(rpibm_gpu_mbox_channel_t channel, uint32_t buff_addr)
{
    volatile uint32_t *write_addr = (volatile uint32_t *)(RPIBM_MBOX_WRITE_ADDR);
    uint32_t           request = buff_addr | channel;
    *write_addr = request;
}

static void rpibm_gpu_mbox_read_response(rpibm_gpu_mbox_channel_t                    channel,
                                         struct rpibm_gpu_mbox_read_response_struct *response)
{
    uint32_t tmp_response;
    uint8_t  valid;
    rpibm_gpu_mbox_read_raw_response(channel, &tmp_response);
    rpibm_gpu_mbox_validate_response(channel, tmp_response, &valid);
    response->valid = valid;
    response->response = tmp_response;
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

void rpibm_gpu_mbox_get_fw_version(int32_t *fw_revision)
{
    struct rpibm_gpu_mbox_fw_revision_message msg __attribute__((aligned(16)));
    msg.header.buf_size = sizeof(msg);
    msg.header.code = 0x0;                 // Request
    msg.fw_revision_tag.tag_hdr.tag = 0x1; // FW revision tag
    msg.fw_revision_tag.tag_hdr.val_buf_size = sizeof(msg.fw_revision_tag.body.response);
    msg.fw_revision_tag.tag_hdr.val_len = 0x0; // This request has 0 bytes of payload
    msg.end_tag = 0x0;

    rpibm_gpu_mbox_clear_responses();
    rpibm_gpu_mbox_wait_write_not_full();
    rpibm_gpu_mbox_send_request(RPIBM_MBOX_PROPERTY_TAGS_CH, (uint32_t)&msg);
    rpibm_gpu_mbox_wait_read_empty();

    struct rpibm_gpu_mbox_read_response_struct response;
    rpibm_gpu_mbox_read_response(RPIBM_MBOX_PROPERTY_TAGS_CH, &response);

    if (response.valid) {
        *fw_revision = msg.fw_revision_tag.body.response.fw_revision;
    } else {
        *fw_revision = -1;
    }
}

void rpibm_gpu_mbox_get_mac_address(uint8_t *mac_address)
{

    struct rpibm_gpu_mbox_mac_address_message msg __attribute__((aligned(16)));
    msg.header.buf_size = sizeof(msg);
    msg.header.code = 0x0;                     // Request
    msg.mac_address_tag.tag_hdr.tag = 0x10003; // MAC Address tag
    msg.mac_address_tag.tag_hdr.val_buf_size = sizeof(msg.mac_address_tag.body.response);
    msg.mac_address_tag.tag_hdr.val_len = 0x0; // This request has 0 bytes of payload
    msg.end_tag = 0x0;

    rpibm_gpu_mbox_clear_responses();
    rpibm_gpu_mbox_wait_write_not_full();
    rpibm_gpu_mbox_send_request(RPIBM_MBOX_PROPERTY_TAGS_CH, (uint32_t)&msg);
    rpibm_gpu_mbox_wait_read_empty();

    struct rpibm_gpu_mbox_read_response_struct response;
    rpibm_gpu_mbox_read_response(RPIBM_MBOX_PROPERTY_TAGS_CH, &response);

    for (size_t i = 0; i < sizeof(msg.mac_address_tag.body.response.mac_address); ++i) {
        if (response.valid) {
            mac_address[i] = msg.mac_address_tag.body.response.mac_address[i];
        } else {
            mac_address[i] = 0;
        }
    }
}