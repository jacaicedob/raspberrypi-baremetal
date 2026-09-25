## Overview

Mailboxes facilitate communication between the ARM and the VideoCore. This page lists the available mailboxes/channels. Each mailbox is an 8-deep FIFO of 32-bit words, which can be read (popped)/written (pushed) by the ARM and VC. Only mailbox 0's status can trigger interrupts on the ARM, so MB 0 is always for communication from VC to ARM and MB 1 is for ARM to VC. **The ARM should never write MB 0 or read MB 1.**

## Channels

The following lists the currently defined mailbox channels, with links to pages describing the format of the messages.

Mailbox 0 defines the following channels:

0. Power management
1. Framebuffer
2. Virtual UART
3. VCHIQ
4. LEDs
5. Buttons
6. Touch screen
7. 
8. Property tags (ARM -> VC)
9. Property tags (VC -> ARM)
Mailbox registers

The following table shows the register offsets for the different mailboxes. 
```
Mailbox Read/Write Peek  Sender  Status    Config
   0    0x00       0x10  0x14    0x18      0x1c
   1    0x20       0x30  0x34    0x38      0x3c
```
## ARM Peripheral Register Map
https://git.u-boot-project.org/u-boot/u-boot/-/blob/master/arch/arm/mach-bcm283x/include/mach/acpi/bcm2836.h


```
#define BCM2836_MBOX_OFFSET                   0x0000b880
#define BCM2836_MBOX_BASE_ADDRESS             (BCM2836_SOC_REGISTERS + BCM2836_MBOX_OFFSET)
#define BCM2836_MBOX_LENGTH                   0x00000024
#define BCM2836_MBOX_READ_OFFSET              0x00000000
#define BCM2836_MBOX_STATUS_OFFSET            0x00000018
#define BCM2836_MBOX_CONFIG_OFFSET            0x0000001c
#define BCM2836_MBOX_WRITE_OFFSET             0x00000020
```
And they define `BCM2836_SOC_REGISTERS  0xfe000000`, which in our case would translate to `0x3F000000`

## General procedure

To read from a mailbox:

    1. Read the status register until the empty flag is not set
    2. Read data from the read register
    3. If the lower four bits do not match the channel number desired then repeat from 1
    4. The upper 28 bits are the returned data

To write to a mailbox

    1. Read the status register until the full flag is not set
    2. Write the data (shifted into the upper 28 bits) combined with the channel (in the lower four bits) to the write register

For each tag type:
    1. Create a struct for the tag and fill out the request values. Make sure its memory aligned to 16 bits
    2. Get the address of this struct, pack the channel number in the lower bits, and write it to the MBOX_WRITE_ADDR.
    3. Wait for the read flag in MBOX_STATUS_ADDR
    4. Read from MBOX_READ_ADDR and validate the response is for the channel
    5. Read the data from the struct created in 1.


##Addresses as data

**With the exception of the property tags mailbox channel**, *when passing memory addresses as the data part of a mailbox message, the addresses should be bus addresses as seen from the VC*. These vary depending on whether the L2 cache is enabled. If it is, <u>physical memory is mapped to start at 0x40000000 by the VC MMU; if L2 caching is disabled, physical memory is mapped to start at 0xC0000000 by the VC MMU</u>. Returned addresses (both those returned in the data part of the mailbox response and any written into the buffer you passed) will also be as mapped by the VC MMU. In the exceptional case when you are using the property tags mailbox channel you should send and receive physical addresses (the same as you'd see from the ARM before enabling the MMU).

For example, if you have created a framebuffer description structure in memory (without having enabled the ARM MMU) at 0x00010000 and you have not changed config.txt to disable the L2 cache, to send it to channel 1 you would send 0x40010001 (0x40000000 | 0x00010000 | 0x1) to the mailbox. Your structure would be updated to include a framebuffer address starting from 0x40000000 (e.g. 0x4D385000) and you would write to it using the corresponding ARM physical address (e.g. 0x0D385000).