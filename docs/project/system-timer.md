## Note on system timer:
On real Raspberry Pi hardware, channels 0 and 2 are typically reserved/used internally by the GPU firmware (for its own scheduling), so bare-metal projects conventionally use channel 1 or channel 3 to avoid conflicting with firmware that's still running on the VideoCore side.

The datasheet's interrupt controller chapter is incomplete/known-buggy on this exact topic. The BCM2835 ARM Peripherals PDF's IRQ chapter documents the interrupt controller's basic and normal pending registers, but it does not clearly enumerate which "normal" GPU IRQ number maps to each System Timer compare channel — this is one of the well-documented errata in that PDF (community-maintained errata lists exist specifically because Broadcom's original document has gaps and outright mistakes in the interrupts section).

The actual mapping, confirmed via community reverse-engineering and cross-referenced against the Linux kernel source (bcm2835-timer driver) and OSDev wiki:

The System Timer has 4 compare registers/match channels (0-3), and their corresponding IRQ bits in the interrupt controller's pending registers are:

* System Timer Match 0 → IRQ bit 0
* System Timer Match 1 → IRQ bit 1
* System Timer Match 2 → IRQ bit 2
* System Timer Match 3 → IRQ bit 3

These fall within the "GPU IRQs" (also called "normal" IRQs, bits 0-31 of the first pending register) — specifically, they're bits 0-3 of IRQ_pending_1 (the first 32-bit normal pending register). Important caveat you'll actually hit in practice: on real Raspberry Pi hardware, channels 0 and 2 are typically reserved/used internally by the GPU firmware (for its own scheduling), so bare-metal projects conventionally use channel 1 or channel 3 to avoid conflicting with firmware that's still running on the VideoCore side. This is exactly the kind of undocumented-but-widely-known gotcha that trips people up if they only read the official PDF.

Reference material better suited to this specific question than the official datasheet:

* The OSDev wiki's Raspberry Pi Bare Bones pages (search "osdev raspberry pi bare bones interrupts") — this community documentation exists specifically to patch the gaps Broadcom's PDF leaves, and explicitly covers System Timer IRQ numbering.
* Cambridge's "Baking Pi" tutorial series interrupts lesson — walks through this exact mapping in the context of a working example.
* Cross-referencing the Linux kernel source for the BCM2835 system timer driver (drivers/clocksource/bcm2835_timer.c in the mainline kernel) — seeing which IRQ number the real production Linux driver requests is a solid ground-truth check, since that code has to be correct or Linux wouldn't boot.