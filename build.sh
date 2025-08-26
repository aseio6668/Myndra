#!/bin/bash

# Myndra Build Script
# Builds the Myndra programming language compiler and runtime

set -e

echo "Building Myndra Programming Language..."

# Create build directory
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Configure with CMake
echo "Configuring build system..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
echo "Compiling..."
make -j$(nproc)

echo "Build completed successfully!"
echo ""
echo "Executables created:"
echo "  - myndra (main compiler/interpreter)"
echo "  - myn-pkg (package manager utility)"
echo ""
echo "To run examples:"
echo "  ./myndra ../examples/hello_world.myn --run"
echo "  ./myndra --interactive"
