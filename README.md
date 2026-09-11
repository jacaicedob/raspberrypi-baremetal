# raspberrypi-baremetal

## Description
Bare-metal library for the Raspberry Pi 3 Model B+ (BCM2837B0). This project is an exercise for honing in my embedded programming skills using my old Pi and Sunfounder sensor kit.

## Current status

- [x] Cross-compilation container + CMake toolchain working
- [x] Linker script + startup assembly (core parking, stack setup, `.bss` zeroing)
- [x] GPIO register header (`bcm2837b0.h`) — GPFSEL/GPSET/GPCLR offsets
- [x] First working program: `blink-led.c` — toggles GPIO 17/18 via direct register writes
- [ ] Timer-driven (non-busy-wait) delays
- [ ] UART console output
- [ ] Bit-banged protocol exercise (I2C or DHT11) on bare metal


## Hardware

- Raspberry Pi 3 Model B+ (BCM2837B0)
- A dedicated microSD card — **do not reuse your Raspberry Pi OS card**; this project replaces the entire boot chain
- Sunfounder Dual-Color LED module
- USB to TTL UART Converter Cable

## Project structure

```
raspberrypi-baremetal/
├── Containerfile                          # Podman build environment (arm-none-eabi toolchain)
├── CMakeLists.txt                         # Root CMake config
├── container_config/
│   └── toolchain-arm-none-eabi.cmake      # CMake cross-compilation toolchain file
├── boot/
│   ├── CMakeLists.txt                     # Builds the startup object, exposes the linker script
│   ├── linker.ld                          # Memory layout / entry point (0x8000)
│   ├── startup.s                          # Core-0 selection, stack setup, .bss zeroing
│   └── config.txt                         # Pi firmware boot config
├── include/
│   └── rpi_bm/
│       ├── bcm2837b0.h                    # BCM2837B0 register map and offsets
│       └── gpio.h                         # GPIO library public interface
├── src/
│   ├── CMakeLists.txt                     # Builds the rpi_bm static library
│   └── gpio.c                             # GPIO library implementation
├── examples/
│   ├── CMakeLists.txt
│   └── blink-led/
│       ├── CMakeLists.txt                 # Links against rpi_bm + boot object, ELF -> raw .img
│       └── blink-led.c                    # GPIO blink example built on the rpi_bm library
├── docs/
│   ├── project/
│   │   └── peripheral-addressing.md       # Notes on peripheral base address handling
│   └── reference/
│       ├── RP-008249-DS-1-bcm2835-peripherals.pdf   # ARM Peripherals datasheet (GPIO/UART/Timer)
│       └── RP-008250-DS-1-bcm2836-peripherals.pdf   # ARM-local peripherals (interrupts, core timers)
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

This produces `build-arm64/examples/blin-led/blink-led.elf` and, via `objcopy`, `build-arm64/examples/blink-led/blink-led.img` — the raw binary the Pi's firmware actually boots.

## Flashing

1. Insert a spare/dedicated microSD card.
2. Use Raspberry Pi Imager's **"Erase"** option to write a clean FAT32 partition (Imager is not used to write the OS image itself — this project's boot files are copied on manually).
3. Mount the card, then run:
```bash
   ./scripts/flashSD.sh <example_name> /path/to/mounted/sdcard
```
   This copies `bootcode.bin`, `start.elf`, `fixup.dat`, `boot/config.txt`, and the compiled kernel (renamed to `kernel7.img`) onto the card.

4. Insert the card into the Pi and power on.

## Reference documentation

Local copies are kept in `docs/reference` so the exact revisions used are pinned alongside the code:

- [`docs/RP-008249-DS-1-bcm2835-peripherals.pdf`](docs/RP-008249-DS-1-bcm2835-peripherals.pdf) — GPIO, UART, System Timer register offsets (peripheral base differs per chip; see note below)
- [`docs/RP-008250-DS-1-bcm2836-peripherals.pdf`](docs/RP-008250-DS-1-bcm2836-peripherals.pdf) — per-core interrupt controller, core timers, spin-table addresses (needed if/when releasing cores 1–3)
- [Raspberry Pi: Processors documentation](https://www.raspberrypi.com/documentation/computers/processors.html) — chip lineage and which datasheet applies to which board
