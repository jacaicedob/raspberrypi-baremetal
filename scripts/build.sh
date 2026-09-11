#!/usr/bin/env bash
set -euo pipefail

podman run --rm -v "$(pwd):/workspace:Z" raspberrypi-baremetal bash -c "
  mkdir -p build-arm64 && cd build-arm64 &&
  cmake -DCMAKE_TOOLCHAIN_FILE=/workspace/container_config/toolchain-arm-none-eabi.cmake .. &&
  make
"
