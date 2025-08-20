#ifndef POMEGRANATE_LEXER_H
#define POMEGRANATE_LEXER_H

#include "token.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace pomegranate {

class Lexer {
public:
    explicit Lexer(const std::string& source);
    
    std::vector<Token> tokenize();
    Token next_token();
    
    bool has_errors() const { return !errors_.empty(); }
    const std::vector<std::string>& get_errors() const { return errors_; }
    
private:
    std::string source_;
    size_t current_;
    size_t line_;
    size_t column_;
    std::vector<std::string> errors_;
    
    static std::unordered_map<std::string, TokenType> keywords_;
    static std::unordered_map<std::string, TokenType> annotations_;
    
    // Helper methods
    bool is_at_end() const;
    char advance();
    char peek() const;
    char peek_next() const;
    bool match(char expected);
    void skip_whitespace();
    void skip_line_comment();
    void skip_block_comment();
    
    // Token creation
    Token make_token(TokenType type);
    Token make_token(TokenType type, int64_t value);
    Token make_token(TokenType type, double value);
    Token make_token(TokenType type, const std::string& value);
    Token make_token(TokenType type, bool value);
    Token error_token(const std::string& message);
    
    // Literal parsing
    Token string_literal();
    Token number_literal();
    Token identifier_or_keyword();
    Token annotation();
    Token semantic_tag();
    
    // Utility
    bool is_alpha(char c) const;
    bool is_digit(char c) const;
    bool is_alnum(char c) const;
    
    void add_error(const std::string& message);
    
    static void init_keywords();
    static void init_annotations();
};

} // namespace pomegranate

#endif // POMEGRANATE_LEXER_H
