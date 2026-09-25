#ifndef RPIBM_FMT_H
#define RPIBM_FMT_H

#include <stdint.h>

typedef enum { RPIBM_FMT_BASE_DEC = 10, RPIBM_FMT_BASE_HEX = 16 } rpibm_fmt_base_t;

void rpibm_fmt_itoa(uint32_t value, rpibm_fmt_base_t base, char *output);
void rpibm_fmt_itoa_zeropad(uint32_t value, rpibm_fmt_base_t base, char *output,
                            uint8_t zero_pad_width);

#endif // RPIBM_FMT_H