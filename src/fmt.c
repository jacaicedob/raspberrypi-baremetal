#include "rpi_bm/fmt.h"
#include <stddef.h>

void rpibm_fmt_itoa(uint32_t value, rpibm_fmt_base_t base, char *output)
{
    rpibm_fmt_itoa_zeropad(value, base, output, 0);
}

void rpibm_fmt_itoa_zeropad(uint32_t value, rpibm_fmt_base_t base, char *output,
                            uint8_t zero_pad_width)
{
    char intermediate[100];

    uint8_t count = 0;

    for (;;) {
        uint32_t lsb = value % base;
        if (lsb < 10) {
            // 0x30 is '0'
            intermediate[count] = (char)(0x30 + lsb);
        } else {
            // 0x41 is 'A'
            lsb = lsb % 10;
            intermediate[count] = (char)(0x41 + lsb);
        }
        ++count;

        value = value / base;
        if (value == 0) {
            break;
        }
    }

    // Zero pad if count < zero_pad_width
    for (size_t i = count; i < zero_pad_width; ++i) {
        intermediate[count] = (char)0x30;
        ++count;
    }

    // Reverse string
    for (uint8_t i = 0; i < count; ++i) {
        output[i] = intermediate[count - i - 1];
    }
    output[count] = 0x0; // Null termination for string
}