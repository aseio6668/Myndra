#include "lexer.h"
#include <iostream>
#include <cassert>

using namespace myndra;

void test_basic_tokens() {
    std::cout << "Testing basic tokens..." << std::endl;
    
    std::string source = "let x = 42 + 3.14";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    assert(tokens.size() == 7); // let, x, =, 42, +, 3.14, EOF
    assert(tokens[0].type == TokenType::LET);
    assert(tokens[1].type == TokenType::IDENTIFIER);
    assert(tokens[2].type == TokenType::ASSIGN);
    assert(tokens[3].type == TokenType::INTEGER);
    assert(tokens[4].type == TokenType::PLUS);
    assert(tokens[5].type == TokenType::FLOAT);
    
    std::cout << "✓ Basic tokens test passed" << std::endl;
}

void test_keywords() {
    std::cout << "Testing keywords..." << std::endl;
    
    std::string source = "fn capsule import with capabilities";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == TokenType::FN);
    assert(tokens[1].type == TokenType::CAPSULE);
    assert(tokens[2].type == TokenType::IMPORT);
    assert(tokens[3].type == TokenType::WITH);
    assert(tokens[4].type == TokenType::CAPABILITIES);
    
    std::cout << "✓ Keywords test passed" << std::endl;
}

void test_annotations() {
    std::cout << "Testing annotations..." << std::endl;
    
    std::string source = "@parallel @reactive @temporal";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == TokenType::AT_PARALLEL);
    assert(tokens[1].type == TokenType::AT_REACTIVE);
    assert(tokens[2].type == TokenType::AT_TEMPORAL);
    
    std::cout << "✓ Annotations test passed" << std::endl;
}

void test_operators() {
    std::cout << "Testing operators..." << std::endl;
    
    std::string source = "-> => :: += -= == != <= >=";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == TokenType::ARROW);
    assert(tokens[1].type == TokenType::FAT_ARROW);
    assert(tokens[2].type == TokenType::DOUBLE_COLON);
    assert(tokens[3].type == TokenType::PLUS_ASSIGN);
    assert(tokens[4].type == TokenType::MINUS_ASSIGN);
    assert(tokens[5].type == TokenType::EQUAL);
    assert(tokens[6].type == TokenType::NOT_EQUAL);
    assert(tokens[7].type == TokenType::LESS_EQUAL);
    assert(tokens[8].type == TokenType::GREATER_EQUAL);
    
    std::cout << "✓ Operators test passed" << std::endl;
}

void test_string_literals() {
    std::cout << "Testing string literals..." << std::endl;
    
    std::string source = "\"Hello, World!\" \"with\\nescapes\"";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == TokenType::STRING);
    assert(std::get<std::string>(tokens[0].literal) == "Hello, World!");
    
    assert(tokens[1].type == TokenType::STRING);
    assert(std::get<std::string>(tokens[1].literal) == "with\nescapes");
    
    std::cout << "✓ String literals test passed" << std::endl;
}

void test_semantic_tags() {
    std::cout << "Testing semantic tags..." << std::endl;
    
    std::string source = "#tag:auth #tag:ui";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == TokenType::TAG);
    assert(tokens[1].type == TokenType::TAG);
    
    std::cout << "✓ Semantic tags test passed" << std::endl;
}

void test_complex_example() {
    std::cout << "Testing complex example..." << std::endl;
    
    std::string source = R"(
        @reactive
        fn update_ui() {
            let score: evolving<int> = 0
            score -> 10 over 5s
        }
    )";
    
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    // Should not have any errors
    assert(!lexer.has_errors());
    
    // Check key tokens are present
    bool found_reactive = false, found_fn = false, found_evolving = false, found_arrow = false;
    
    for (const auto& token : tokens) {
        if (token.type == TokenType::AT_REACTIVE) found_reactive = true;
        if (token.type == TokenType::FN) found_fn = true;
        if (token.type == TokenType::EVOLVING) found_evolving = true;
        if (token.type == TokenType::ARROW) found_arrow = true;
    }
    
    assert(found_reactive);
    assert(found_fn);
    assert(found_evolving);
    assert(found_arrow);
    
    std::cout << "✓ Complex example test passed" << std::endl;
}

int main() {
    std::cout << "Running Myndra Lexer Tests..." << std::endl;
    std::cout << "=================================" << std::endl;
    
    try {
        test_basic_tokens();
        test_keywords();
        test_annotations();
        test_operators();
        test_string_literals();
        test_semantic_tags();
        test_complex_example();
        
        std::cout << std::endl;
        std::cout << "✓ All lexer tests passed!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Test failed: " << e.what() << std::endl;
        return 1;
    }
}
