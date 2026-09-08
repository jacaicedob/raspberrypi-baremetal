#!/usr/bin/env bash
set -euo pipefail


if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <path-to-mounted-sd-card-boot-partition>" >&2
    exit 1
fi

SD_CARD_ROOT="$1"

if [[ ! -d "${SD_CARD_ROOT}" ]]; then
    echo "Error: ${SD_CARD_ROOT} is not a directory" >&2
    exit 1
fi

# Resolve the repo root as the directory one level up from this script,
# so the build works no matter where it's invoked from.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

# User can overwrite FW_REPO_ROOT env variable before calling this script to point to a different clone location
FW_REPO_ROOT="${FW_REPO_ROOT:-${REPO_ROOT}/../firmware}"

cp "${FW_REPO_ROOT}"/boot/bootcode.bin \
   "${FW_REPO_ROOT}"/boot/start.elf \
   "${FW_REPO_ROOT}"/boot/fixup.dat \
   "${REPO_ROOT}"/boot/config.txt \
   "${SD_CARD_ROOT}"

cp "${REPO_ROOT}"/build-arm64/apps/blink.img \
   "${SD_CARD_ROOT}"/kernel7.img 

echo "Flashed to ${SD_CARD_ROOT}"
