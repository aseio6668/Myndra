cmake_minimum_required(VERSION 3.16)
project(Myndra VERSION 1.0.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi /Wall")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

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

# Parser sources
set(PARSER_SOURCES
    src/parser/ast.cpp
    src/parser/parser.cpp
)

# Interpreter sources
set(INTERPRETER_SOURCES
    src/interpreter/interpreter.cpp
)

# All other components will be implemented as stubs for now
set(OTHER_SOURCES
    src/stubs.cpp
)

# Main executable
add_executable(myndra
    src/main.cpp
    src/compiler_impl.cpp
    ${LEXER_SOURCES}
    ${PARSER_SOURCES}
    ${INTERPRETER_SOURCES}
    ${OTHER_SOURCES}
)

# Link libraries (remove LLVM for now to simplify build)
target_link_libraries(myndra Threads::Threads)

# Compiler library
add_library(myndra_compiler STATIC
    src/compiler_impl.cpp
    ${LEXER_SOURCES}
    ${PARSER_SOURCES}
    ${INTERPRETER_SOURCES}
    ${OTHER_SOURCES}
)

# Runtime library
add_library(myndra_runtime STATIC
    ${OTHER_SOURCES}
)

target_link_libraries(myndra_runtime Threads::Threads)

# Package manager utility
add_executable(myn-pkg
    src/pom_pkg_main.cpp
)

target_link_libraries(myn-pkg myndra_runtime)

# Tests
enable_testing()
add_subdirectory(tests)

# Install targets
install(TARGETS myndra myn-pkg DESTINATION bin)
install(DIRECTORY src/stdlib/ DESTINATION lib/myndra/stdlib)
