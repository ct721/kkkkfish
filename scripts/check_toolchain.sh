#!/usr/bin/env zsh
# Quick check for GNU Arm Embedded toolchain used by the Makefile.
set -e

echo "Checking for arm-none-eabi toolchain in PATH..."

if command -v arm-none-eabi-ld >/dev/null 2>&1; then
  echo "Found: $(command -v arm-none-eabi-ld)"
  echo "You should be able to link and build now. Try: make T=\"/path/to/vex/sdk\" -j4"
  exit 0
fi

echo "\nNo 'arm-none-eabi-ld' found in PATH. The linker (and other arm-none-eabi tools) must be installed and available on PATH for the Makefile to succeed."

if command -v brew >/dev/null 2>&1; then
  echo "Homebrew detected. Common fix (choose one):"
  echo "  1) Try installing the GNU Arm toolchain via Homebrew (may require tapping):"
  echo "     brew tap ArmMbed/homebrew-formulae"
  echo "     brew install gcc-arm-none-eabi"
  echo "  2) Or install the official GNU Arm Embedded Toolchain from ARM's site and add its 'bin' dir to PATH."
else
  echo "No Homebrew detected. Two options:"
  echo "  * Install Homebrew (https://brew.sh/) then run the Homebrew commands above, or"
  echo "  * Download the GNU Arm Embedded Toolchain from ARM's developer site and add its 'bin' dir to PATH."
fi

echo "\nHow to add the toolchain to PATH (example). After you install the arm toolchain, add a line like this to ~/.zshrc (adjust path to your install):"
echo '  export PATH="/opt/homebrew/bin:$PATH"    # typical Homebrew on Apple Silicon'
echo '  export PATH="/usr/local/bin:$PATH"      # typical Homebrew on Intel mac'
echo '  export PATH="/path/to/gcc-arm-none-eabi/bin:$PATH"  # if you extracted a downloaded archive'

echo "\nVerify after install:"
echo "  which arm-none-eabi-ld"
echo "  arm-none-eabi-ld --version"

echo "\nOnce the toolchain is available, rebuild with the SDK path if needed. Example (uses the SDK bundled by VEXcode extension):"
echo "  make T=\"/Users/aaron/Library/Application Support/Code/User/globalStorage/vexrobotics.vexcode/sdk/cpp/V5/V5_20240802_15_00_00\" -j4"

exit 2
