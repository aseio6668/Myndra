cmake_minimum_required(VERSION 3.16)

# Find test framework (we'll use simple assertions for now)
enable_testing()

# Test executable for lexer
add_executable(test_lexer
    test_lexer.cpp
    ../src/lexer/lexer.cpp
    ../src/lexer/token.cpp
)

target_include_directories(test_lexer PRIVATE ../src/lexer)

add_test(NAME LexerTests COMMAND test_lexer)

# Test executable for basic compilation
add_executable(test_basic_compilation
    test_basic_compilation.cpp
)

target_link_libraries(test_basic_compilation pomegranate_compiler)

add_test(NAME BasicCompilationTests COMMAND test_basic_compilation)
