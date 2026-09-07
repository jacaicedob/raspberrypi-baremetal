FROM debian:trixie

RUN apt-get update && apt-get install -y \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    libnewlib-arm-none-eabi \
    cmake \
    git \
    gdb-multiarch \
    qemu-system-arm \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
