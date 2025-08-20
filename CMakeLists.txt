cmake_minimum_required(VERSION 3.16)
project(Pomegranate VERSION 1.0.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find required packages
find_package(Threads REQUIRED)

# LLVM will be added later when we implement the backend
# find_package(LLVM REQUIRED CONFIG)

# Source files
set(LEXER_SOURCES
    src/lexer/token.cpp
    src/lexer/lexer.cpp
)

# All other components will be implemented as stubs for now
set(OTHER_SOURCES
    src/stubs.cpp
)

# Main executable
add_executable(pomegranate
    src/main.cpp
    src/compiler_impl.cpp
    ${LEXER_SOURCES}
    ${OTHER_SOURCES}
)

# Link libraries (remove LLVM for now to simplify build)
target_link_libraries(pomegranate Threads::Threads)

# Compiler library
add_library(pomegranate_compiler STATIC
    src/compiler_impl.cpp
    ${LEXER_SOURCES}
    ${OTHER_SOURCES}
)

# Runtime library
add_library(pomegranate_runtime STATIC
    ${OTHER_SOURCES}
)

target_link_libraries(pomegranate_runtime Threads::Threads)

# Package manager utility
add_executable(pom-pkg
    src/pom_pkg_main.cpp
)

target_link_libraries(pom-pkg pomegranate_runtime)

# Tests
enable_testing()
add_subdirectory(tests)

# Install targets
install(TARGETS pomegranate pom-pkg DESTINATION bin)
install(DIRECTORY src/stdlib/ DESTINATION lib/pomegranate/stdlib)
