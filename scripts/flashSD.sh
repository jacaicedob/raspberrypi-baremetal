#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 2 ]]; then
    echo "Usage: $0 <example-name> <path-to-mounted-sd-card-boot-partition>" >&2
    echo "Example: $0 blink /media/sdcard" >&2
    exit 1
fi

EXAMPLE_NAME="$1"
SD_CARD_ROOT="$2"

if [[ ! -d "${SD_CARD_ROOT}" ]]; then
    echo "Error: ${SD_CARD_ROOT} is not a directory" >&2
    exit 1
fi

# Resolve the repo root as the directory one level up from this script,
# so the build works no matter where it's invoked from.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

KERNEL_IMG="${REPO_ROOT}/build-arm64/examples/${EXAMPLE_NAME}/${EXAMPLE_NAME}.img"

if [[ ! -f "${KERNEL_IMG}" ]]; then
    echo "Error: ${KERNEL_IMG} not found — did you build '${EXAMPLE_NAME}'?" >&2
    exit 1
fi

# User can overwrite FW_REPO_ROOT env variable before calling this script to point to a different clone location
FW_REPO_ROOT="${FW_REPO_ROOT:-${REPO_ROOT}/../firmware}"

cp "${FW_REPO_ROOT}"/boot/bootcode.bin \
   "${FW_REPO_ROOT}"/boot/start.elf \
   "${FW_REPO_ROOT}"/boot/fixup.dat \
   "${REPO_ROOT}"/boot/config.txt \
   "${SD_CARD_ROOT}"

cp "${KERNEL_IMG}" "${SD_CARD_ROOT}"/kernel7.img

echo "Flashed ${EXAMPLE_NAME} to ${SD_CARD_ROOT}"