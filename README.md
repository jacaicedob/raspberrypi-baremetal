# raspberrypi-baremetal

Bare-metal firmware for the Raspberry Pi 3 Model B+ (BCM2837B0) — no Linux, no RTOS. Built with a cross-compiling `arm-none-eabi` toolchain, direct memory-mapped register access (GPIO, UART, timers), and a custom linker script/boot sequence written from the BCM2835/BCM2836 datasheets.

This project is a companion to [`linux-low-level-protocols`](../linux-low-level-protocols), which covers the same peripherals (I2C, GPIO) from userspace on top of Linux. This project strips that away entirely — the code here runs directly on the ARM core from power-on, with no OS, no drivers, and no C runtime underneath it.

## Hardware

- Raspberry Pi 3 Model B+ (BCM2837B0, confirmed via `cat /sys/firmware/devicetree/base/model`)
- A dedicated microSD card — **do not reuse your Raspberry Pi OS card**; this project replaces the entire boot chain
- LEDs + resistors wired to GPIO pins (currently GPIO 17/18, BCM numbering, per the SunFounder GPIO Extension Board's silkscreen labels)

## Project structure

```
raspberrypi-baremetal/
├── Containerfile                          # Podman build environment (arm-none-eabi toolchain)
├── CMakeLists.txt                         # Root CMake config
├── container_config/
│   └── toolchain-arm-none-eabi.cmake      # CMake cross-compilation toolchain file
├── boot/
│   ├── linker.ld                          # Memory layout / entry point (0x8000)
│   ├── startup.s                          # Core-0 selection, stack setup, .bss zeroing
│   └── config.txt                         # Pi firmware boot config
├── apps/
│   ├── CMakeLists.txt
│   ├── bcm2837b0.h                        # BCM2837B0 peripheral register offsets (GPIO)
│   └── blink.c                            # First bare-metal program: GPIO blink
├── docs/
│   ├── RP-008249-DS-1-bcm2835-peripherals.pdf   # ARM Peripherals datasheet (GPIO/UART/Timer)
│   └── RP-008250-DS-1-bcm2836-peripherals.pdf   # ARM-local peripherals (interrupts, core timers)
├── scripts/
│   ├── build.sh                           # Cross-compile inside the running container
│   ├── containerBuild.sh                  # Build the Podman image
│   └── flashSD.sh                         # Copies firmware + kernel image to SD card
└── README.md
```

## Prerequisites

- [Podman](https://podman.io/)
- A clone of [`raspberrypi/firmware`](https://github.com/raspberrypi/firmware) (for `bootcode.bin`, `start.elf`, `fixup.dat`) — set `FW_REPO_ROOT` if it's not cloned as a sibling directory to this repo
- (Optional) `qemu-system-arm`, already included in the container image, for test-booting without real hardware

## Building

Build the cross-compilation container image:
```bash
./scripts/containerBuild.sh
```

Cross-compile:
```bash
./scripts/build.sh
```

This produces `build-arm64/apps/blink.elf` and, via `objcopy`, `build-arm64/apps/blink.img` — the raw binary the Pi's firmware actually boots.

## Flashing

1. Insert a spare/dedicated microSD card.
2. Use Raspberry Pi Imager's **"Erase"** option to write a clean FAT32 partition (Imager is not used to write the OS image itself — this project's boot files are copied on manually).
3. Mount the card, then run:
```bash
   ./scripts/flashSD.sh /path/to/mounted/sdcard
```
   This copies `bootcode.bin`, `start.elf`, `fixup.dat`, `boot/config.txt`, and the compiled kernel (renamed to `kernel7.img`) onto the card.
4. Insert the card into the Pi and power on.

## Current status

- [x] Cross-compilation container + CMake toolchain working
- [x] Linker script + startup assembly (core parking, stack setup, `.bss` zeroing)
- [x] GPIO register header (`bcm2837b0.h`) — GPFSEL/GPSET/GPCLR offsets
- [x] First working program: `blink.c` — toggles GPIO 17/18 via direct register writes
- [ ] UART console output
- [ ] Timer-driven (non-busy-wait) delays
- [ ] Bit-banged protocol exercise (I2C or DHT11) on bare metal

## Reference documentation

Local copies are kept in `docs/` so the exact revisions used are pinned alongside the code:

- [`docs/RP-008249-DS-1-bcm2835-peripherals.pdf`](docs/RP-008249-DS-1-bcm2835-peripherals.pdf) — GPIO, UART, System Timer register offsets (peripheral base differs per chip; see note below)
- [`docs/RP-008250-DS-1-bcm2836-peripherals.pdf`](docs/RP-008250-DS-1-bcm2836-peripherals.pdf) — per-core interrupt controller, core timers, spin-table addresses (needed if/when releasing cores 1–3)
- [Raspberry Pi: Processors documentation](https://www.raspberrypi.com/documentation/computers/processors.html) — chip lineage and which datasheet applies to which board

**Note on peripheral base address:** the BCM2835 datasheet documents offsets relative to `0x7E000000`, which is the GPU-side bus address. On this board (BCM2837B0), the ARM-side physical base is `0x3F000000` — every offset in the datasheet is used as-is, only the base prefix changes.

## Known constraints / gotchas

- `arm-none-eabi-gcc` targets 32-bit ARM (`kernel7.img`), not AArch64 — this is a deliberate choice for simplicity; see the "Path B" discussion in `linux-low-level-protocols` for reasoning.
- `CMAKE_ASM_FLAGS` must be set explicitly (separately from `CMAKE_C_FLAGS`) or the assembler defaults to an architecture too old to support instructions like `wfe`.
- Busy-wait delay loops using `(1 << N)` iteration counts can take far longer than expected on unmapped/uncached memory (no MMU is configured) — a `volatile` loop counter forces real memory traffic on every iteration, not a fast register-only loop.