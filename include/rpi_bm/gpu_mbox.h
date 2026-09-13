#ifndef RPIBM_MBOX_H
#define RPIBM_MBOX_H

#include "rpi_bm/bcm2837b0.h"
#include <stdint.h>

#define RPIBM_MBOX_STATUS_READ_EMPTY_MASK 0X40000000
#define RPIBM_MBOX_STATUS_WRITE_FULL_MASK 0X80000000
#define RPIBM_MBOX_CHANNEL_MASK           0XF

typedef enum {
    RPIBM_MBOX_FRAMEBUFFER_CH = 1,
    RPIBM_MBOX_PROPERTY_TAGS_CH = 8
} rpibm_gpu_mbox_channel_t;

#define RPIBM_MBOX_TAGS_FW_VERSION 0x1

void rpibm_gpu_mbox_get_fw_version(uint32_t *fw_version);

#endif // RPIBM_MBOX_H