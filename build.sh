#!/bin/bash

# Pomegranate Build Script
# Builds the Pomegranate programming language compiler and runtime

set -e

echo "Building Pomegranate Programming Language..."

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
echo "  - pomegranate (main compiler/interpreter)"
echo "  - pom-pkg (package manager utility)"
echo ""
echo "To run examples:"
echo "  ./pomegranate ../examples/hello_world.pom --run"
echo "  ./pomegranate --interactive"
