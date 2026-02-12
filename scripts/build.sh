#!/usr/bin/env zsh
# Build helper for this repo. It will:
#  - choose a sensible SDK if VEX_SDK_PATH is not set
#  - check for arm-none-eabi-* toolchain on PATH
#  - run make with the selected SDK

set -euo pipefail

# Prefer env override
if [[ -n "${VEX_SDK_PATH-}" ]]; then
  SDK="${VEX_SDK_PATH}"
else
  # Fallback to the VEXcode extension SDK path discovered earlier (common location).
  SDK="$HOME/Library/Application Support/Code/User/globalStorage/vexrobotics.vexcode/sdk/cpp/V5/V5_20240802_15_00_00/vexv5"
fi

echo "Using SDK: ${SDK}"

if [[ ! -d "${SDK}" ]]; then
  echo "ERROR: SDK directory not found at ${SDK}"
  echo "Set VEX_SDK_PATH or pass T= to make. Example:" 
  echo "  make T=\"/path/to/vex/sdk\" -j4"
  exit 2
fi

if ! command -v arm-none-eabi-ld >/dev/null 2>&1; then
  echo "ERROR: arm-none-eabi toolchain not found in PATH."
  echo "Install it (Homebrew):"
  echo "  brew tap ArmMbed/homebrew-formulae && brew install gcc-arm-none-eabi"
  echo "Then add its bin dir to PATH (e.g., export PATH=\"/opt/homebrew/bin:$PATH\")."
  exit 3
fi

echo "Toolchain found: $(command -v arm-none-eabi-ld)"

echo "Running make with SDK..."
make T="${SDK}" -j2

echo "Build finished. If you want to flash, use the VEX Utility or VEXcode GUI to upload the generated .bin located in build/"
