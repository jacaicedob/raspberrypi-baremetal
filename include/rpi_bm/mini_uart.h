#ifndef RPIBM_MINI_UART_H
#define RPIBM_MINI_UART_H

#include "rpi_bm/bcm2837b0.h"

#include <stdint.h>

#define RPIBM_MINI_UART_AUX_EN 1

#define RPIBM_MINI_UART_TX_EN   2
#define RPIBM_MINI_UART_DLAB_EN (1 << 7)

#define RPIBM_MINI_UART_TX_DONE         (1 << 9)
#define RPIBM_MINI_UART_TX_FIFO_EMPTY   (1 << 8)
#define RPIBM_MINI_UART_TX_FIFO_FULL    (1 << 5)
#define RPIBM_MINI_UART_TX_IDLE         (1 << 3)
#define RPIBM_MINI_UART_SPACE_AVAILABLE (1 << 1)

typedef enum {
    RPIBM_MINI_UART_DATA_SIZE_7BIT = 2, // Errata in BCM2835 documentation. These values taken from
                                        // https://elinux.org/BCM2835_datasheet_errata
    RPIBM_MINI_UART_DATA_SIZE_8BIT = 3
} rpibm_mini_uart_datasize_t;

void rpibm_mini_uart_init(unsigned int baud);
void rpibm_mini_uart_print(char *buffer);

#endif // RPIBM_MINI_UART_H
