#!/bin/bash
# Build script for Fuse Windows distribution
# Run this from MSYS2 MinGW 64-bit terminal
#
# This script:
# 1. Generates configure script (if needed)
# 2. Configures Fuse for Windows
# 3. Builds Fuse and creates Windows distribution packages

set -e

echo "=== Building Fuse Windows Distribution ==="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if we're in MSYS2/MinGW environment
if [[ -z "$MSYSTEM" ]]; then
    echo -e "${RED}Error: This script should be run from MSYS2 MinGW 64-bit terminal${NC}"
    echo "Please open 'MSYS2 MinGW 64-bit' from Start Menu"
    exit 1
fi

if [[ "$MSYSTEM" != "MINGW64" ]]; then
    echo -e "${YELLOW}Warning: You're not in MINGW64 environment. Using MINGW64 is recommended.${NC}"
fi

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

echo -e "${GREEN}Current directory: $SCRIPT_DIR${NC}"

# Check for required tools
echo -e "\n${GREEN}Checking for required tools...${NC}"

for tool in gcc make pkg-config perl; do
    if ! command -v $tool &> /dev/null; then
        echo -e "${RED}Error: $tool is not installed${NC}"
        echo "Please install it with: pacman -S mingw-w64-x86_64-$tool"
        exit 1
    fi
    echo "  ✓ $tool found"
done

# Build 3rdparty dependencies if needed
if [[ ! -d "$SCRIPT_DIR/3rdparty/dist/bin" ]]; then
    echo -e "\n${GREEN}Building 3rdparty dependencies...${NC}"
    cd "$SCRIPT_DIR/3rdparty"
    make -j$(nproc)
    cd "$SCRIPT_DIR"
else
    echo -e "\n${GREEN}3rdparty dependencies already built (found 3rdparty/dist/bin)${NC}"
fi

# Set up local dist prefix
DIST_PREFIX="$SCRIPT_DIR/3rdparty/dist"
export PKG_CONFIG_PATH="$DIST_PREFIX/lib/pkgconfig:${PKG_CONFIG_PATH}"

# Generate configure script if needed
echo -e "\n${GREEN}Generating configure script...${NC}"
if [[ ! -f configure ]]; then
    if [[ ! -f autogen.sh ]]; then
        echo -e "${RED}Error: autogen.sh not found${NC}"
        exit 1
    fi
    ./autogen.sh
else
    echo -e "  ✓ Configure script exists"
fi

# Configure Fuse
echo -e "\n${GREEN}Configuring Fuse...${NC}"

# Build configure options based on available libraries
CONFIGURE_OPTS="--with-win32 --without-zlib --without-png --prefix=/usr/local"

if pkg-config --exists libxml-2.0; then
    echo -e "  Configuring with libxml2 support"
else
    echo -e "  Configuring without libxml2"
    CONFIGURE_OPTS="$CONFIGURE_OPTS --without-libxml2"
fi

# PKG_CONFIG_PATH is already exported above
./configure $CONFIGURE_OPTS

# Build and create distribution
echo -e "\n${GREEN}Building Fuse and creating Windows distribution...${NC}"
make dist-win32 -j$(nproc)

echo -e "\n${GREEN}=== Build complete! ===${NC}"
echo ""
echo "Distribution packages created:"
if [[ -n "$(find . -maxdepth 1 -name "fuse-*-win32.zip" 2>/dev/null | head -1)" ]]; then
    echo "  - $(find . -maxdepth 1 -name "fuse-*-win32.zip" | head -1)"
fi
if [[ -n "$(find . -maxdepth 1 -name "fuse-*-win32.7z" 2>/dev/null | head -1)" ]]; then
    echo "  - $(find . -maxdepth 1 -name "fuse-*-win32.7z" | head -1)"
fi
if [[ -n "$(find . -maxdepth 1 -name "fuse-*-win32-setup.exe" 2>/dev/null | head -1)" ]]; then
    echo "  - $(find . -maxdepth 1 -name "fuse-*-win32-setup.exe" | head -1)"
fi
echo ""
