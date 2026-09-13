#include <stdint.h>

typedef enum { RPIBM_FMT_BASE_DEC = 10, RPIBM_FMT_BASE_HEX = 8 } rpibm_fmt_base_t;

void rpibm_fmt_itoa(uint32_t value, rpibm_fmt_base_t base, char *output);