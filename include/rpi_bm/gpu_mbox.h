#ifndef RPIBM_MBOX_H
#define RPIBM_MBOX_H

#include "rpi_bm/bcm2837b0.h"
#include <stdint.h>
#include <stddef.h>

#define RPIBM_MBOX_STATUS_READ_EMPTY_MASK 0X40000000
#define RPIBM_MBOX_STATUS_WRITE_FULL_MASK 0X80000000
#define RPIBM_MBOX_CHANNEL_MASK           0XF

typedef enum {
    RPIBM_MBOX_FRAMEBUFFER_CH = 1,
    RPIBM_MBOX_PROPERTY_TAGS_CH = 8
} rpibm_gpu_mbox_channel_t;

struct rpibm_gpu_mbox_read_response_struct {
    uint8_t  valid;
    uint32_t response;
};

// Generic header for the mailbox
struct rpibm_gpu_mbox_header {
    uint32_t buf_size; // Overall buffer size in bytes (including this header)
    uint32_t code;     // Request/response code
};

// Generic header for tags
struct rpibm_gpu_mbox_tag_header {
    uint32_t tag; // Tag number from
                  // https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
    uint32_t val_buf_size; // Tag request + response buffer size (bytes)
    uint32_t val_len;
};

// Header for the FW revision tag
struct rpibm_gpu_mbox_tag_get_fw_revision {
    struct rpibm_gpu_mbox_tag_header tag_hdr;
    union {
        struct {
        } request; // 0-bytes request length
        struct {
            uint32_t fw_revision; // 4-byte response length
        } response;
    } body;
};

struct rpibm_gpu_mbox_fw_revision_message {
    struct rpibm_gpu_mbox_header              header;
    struct rpibm_gpu_mbox_tag_get_fw_revision fw_revision_tag;
    uint32_t                                  end_tag;
};

void rpibm_gpu_mbox_get_fw_version(int32_t *fw_revision);

// Struct for MAC Address tag
struct rpibm_gpu_mbox_tag_get_mac_address {
    struct rpibm_gpu_mbox_tag_header tag_hdr;
    union {
        struct {
        } request; // 0-bytes request length
        struct {
            uint8_t mac_address[6];
            uint8_t padding[2]; // Needed to align with 32-bit word
        } response;
    } body;
};

struct rpibm_gpu_mbox_mac_address_message {
    struct rpibm_gpu_mbox_header              header;
    struct rpibm_gpu_mbox_tag_get_mac_address mac_address_tag;
    uint32_t                                  end_tag;
};

void rpibm_gpu_mbox_get_mac_address(uint8_t *mac_address);

// Strcut for Set Physical Size
struct rpibm_gpu_mbox_tag_set_physical_size {
    struct rpibm_gpu_mbox_tag_header tag_hdr;
    union {
        struct {
            uint32_t width;
            uint32_t height;
        } request;
        struct {
            uint32_t width;
            uint32_t height;
        } response;
    } body;
};

// Strcut for Set Virtual Size
struct rpibm_gpu_mbox_tag_set_virtual_size {
    struct rpibm_gpu_mbox_tag_header tag_hdr;
    union {
        struct {
            uint32_t width;
            uint32_t height;
        } request;
        struct {
            uint32_t width;
            uint32_t height;
        } response;
    } body;
};

// Strcut for Set Depth
struct rpibm_gpu_mbox_tag_set_depth {
    struct rpibm_gpu_mbox_tag_header tag_hdr;
    union {
        struct {
            uint32_t depth;
        } request;
        struct {
            uint32_t depth;
        } response;
    } body;
};

// Strcut for Get Pitch
struct rpibm_gpu_mbox_tag_get_pitch {
    struct rpibm_gpu_mbox_tag_header tag_hdr;
    union {
        struct {
        } request;
        struct {
            uint32_t pitch;
        } response;
    } body;
};

// Struct for Frame Buffer allocation
struct rpibm_gpu_mbox_tag_allocate_frame_buffer {
    struct rpibm_gpu_mbox_tag_header tag_hdr;
    union {
        struct {
            uint32_t alignment;
        } request;
        struct {
            uint32_t base_address;
            uint32_t size;
        } response;
    } body;
};

struct rpibm_frame_buffer {
    uint32_t  phy_width;
    uint32_t  phy_height;
    uint32_t  virt_width;
    uint32_t  virt_height;
    uint32_t  depth;
    uint32_t  pitch;
    uint32_t  buffer_size;
    uint32_t *buffer;
};

struct rpibm_gpu_mbox_allocate_frame_buffer_message {
    struct rpibm_gpu_mbox_header                    header;
    struct rpibm_gpu_mbox_tag_set_physical_size     set_physical_size_tag;
    struct rpibm_gpu_mbox_tag_set_virtual_size      set_virtual_size_tag;
    struct rpibm_gpu_mbox_tag_set_depth             set_depth_tag;
    struct rpibm_gpu_mbox_tag_allocate_frame_buffer allocate_frame_buffer_tag;
    uint32_t                                        end_tag;
};

struct rpibm_gpu_mbox_get_pitch_message {
    struct rpibm_gpu_mbox_header        header;
    struct rpibm_gpu_mbox_tag_get_pitch get_pitch_tag;
    uint32_t                            end_tag;
};

void rpibm_gpu_mbox_allocate_frame_buffer(struct rpibm_frame_buffer *frame_buffer);
void rpibm_gpu_mbox_get_pitch(struct rpibm_frame_buffer *frame_buffer);

#endif // RPIBM_MBOX_H