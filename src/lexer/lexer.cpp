#include "lexer.h"
#include <cctype>
#include <algorithm>

namespace pomegranate {

std::unordered_map<std::string, TokenType> Lexer::keywords_;
std::unordered_map<std::string, TokenType> Lexer::annotations_;

Lexer::Lexer(const std::string& source) 
    : source_(source), current_(0), line_(1), column_(1) {
    if (keywords_.empty()) {
        init_keywords();
    }
    if (annotations_.empty()) {
        init_annotations();
    }
}

void Lexer::init_keywords() {
    keywords_ = {
        {"let", TokenType::LET},
        {"fn", TokenType::FN},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"while", TokenType::WHILE},
        {"for", TokenType::FOR},
        {"return", TokenType::RETURN},
        {"import", TokenType::IMPORT},
        {"export", TokenType::EXPORT},
        {"with", TokenType::WITH},
        {"capabilities", TokenType::CAPABILITIES},
        {"capsule", TokenType::CAPSULE},
        {"dsl", TokenType::DSL},
        {"fallback", TokenType::FALLBACK},
        {"retry", TokenType::RETRY},
        {"context", TokenType::CONTEXT},
        {"over", TokenType::OVER},
        {"tag", TokenType::TAG},
        {"did", TokenType::DID},
        {"evolving", TokenType::EVOLVING},
        {"true", TokenType::BOOLEAN},
        {"false", TokenType::BOOLEAN},
        {"nil", TokenType::NIL},
        {"and", TokenType::AND},
        {"or", TokenType::OR},
        {"not", TokenType::NOT},
        {"observable", TokenType::OBSERVABLE},
        {"subscribe", TokenType::SUBSCRIBE},
        {"emit", TokenType::EMIT},
        {"transition", TokenType::TRANSITION},
        {"timeline", TokenType::TIMELINE},
        {"verify", TokenType::VERIFY},
        {"proof", TokenType::PROOF},
        {"has_proof", TokenType::HAS_PROOF}
    };
}

void Lexer::init_annotations() {
    annotations_ = {
        {"@sync", TokenType::AT_SYNC},
        {"@async", TokenType::AT_ASYNC},
        {"@parallel", TokenType::AT_PARALLEL},
        {"@reactive", TokenType::AT_REACTIVE},
        {"@temporal", TokenType::AT_TEMPORAL}
    };
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (!is_at_end()) {
        Token token = next_token();
        if (token.type != TokenType::COMMENT) { // Skip comments
            tokens.push_back(token);
        }
        if (token.type == TokenType::EOF_TOKEN || token.type == TokenType::ERROR) {
            break;
        }
    }
    
    if (tokens.empty() || tokens.back().type != TokenType::EOF_TOKEN) {
        tokens.emplace_back(TokenType::EOF_TOKEN, "", line_, column_);
    }
    
    return tokens;
}

Token Lexer::next_token() {
    skip_whitespace();
    
    if (is_at_end()) {
        return make_token(TokenType::EOF_TOKEN);
    }
    
    char c = advance();
    
    // Single-character tokens
    switch (c) {
        case '(': return make_token(TokenType::LEFT_PAREN);
        case ')': return make_token(TokenType::RIGHT_PAREN);
        case '{': return make_token(TokenType::LEFT_BRACE);
        case '}': return make_token(TokenType::RIGHT_BRACE);
        case '[': return make_token(TokenType::LEFT_BRACKET);
        case ']': return make_token(TokenType::RIGHT_BRACKET);
        case ',': return make_token(TokenType::COMMA);
        case '.': return make_token(TokenType::DOT);
        case ';': return make_token(TokenType::SEMICOLON);
        case '?': return make_token(TokenType::QUESTION);
        case '+': 
            if (match('=')) return make_token(TokenType::PLUS_ASSIGN);
            return make_token(TokenType::PLUS);
        case '-': 
            if (match('=')) return make_token(TokenType::MINUS_ASSIGN);
            if (match('>')) return make_token(TokenType::ARROW);
            return make_token(TokenType::MINUS);
        case '*': return make_token(TokenType::MULTIPLY);
        case '%': return make_token(TokenType::MODULO);
        case '!':
            if (match('=')) return make_token(TokenType::NOT_EQUAL);
            return make_token(TokenType::NOT);
        case '=':
            if (match('=')) return make_token(TokenType::EQUAL);
            if (match('>')) return make_token(TokenType::FAT_ARROW);
            return make_token(TokenType::ASSIGN);
        case '<':
            if (match('=')) return make_token(TokenType::LESS_EQUAL);
            return make_token(TokenType::LESS);
        case '>':
            if (match('=')) return make_token(TokenType::GREATER_EQUAL);
            return make_token(TokenType::GREATER);
        case ':':
            if (match(':')) return make_token(TokenType::DOUBLE_COLON);
            return make_token(TokenType::COLON);
        case '/':
            if (match('/')) {
                skip_line_comment();
                return make_token(TokenType::COMMENT);
            }
            if (match('*')) {
                skip_block_comment();
                return make_token(TokenType::COMMENT);
            }
            return make_token(TokenType::DIVIDE);
        case '#':
            if (is_alpha(peek())) {
                return semantic_tag();
            }
            return make_token(TokenType::HASH);
        case '@':
            return annotation();
        case '\n':
            line_++;
            column_ = 1;
            return make_token(TokenType::NEWLINE);
        case '"':
            return string_literal();
        default:
            if (is_digit(c)) {
                current_--; // Back up to re-read the digit
                column_--;
                return number_literal();
            }
            if (is_alpha(c)) {
                current_--; // Back up to re-read the character
                column_--;
                return identifier_or_keyword();
            }
            add_error("Unexpected character: " + std::string(1, c));
            return error_token("Unexpected character");
    }
}

bool Lexer::is_at_end() const {
    return current_ >= source_.length();
}

char Lexer::advance() {
    if (is_at_end()) return '\0';
    column_++;
    return source_[current_++];
}

char Lexer::peek() const {
    if (is_at_end()) return '\0';
    return source_[current_];
}

char Lexer::peek_next() const {
    if (current_ + 1 >= source_.length()) return '\0';
    return source_[current_ + 1];
}

bool Lexer::match(char expected) {
    if (is_at_end()) return false;
    if (source_[current_] != expected) return false;
    current_++;
    column_++;
    return true;
}

void Lexer::skip_whitespace() {
    while (!is_at_end()) {
        char c = peek();
        if (c == ' ' || c == '\r' || c == '\t') {
            advance();
        } else {
            break;
        }
    }
}

void Lexer::skip_line_comment() {
    while (peek() != '\n' && !is_at_end()) {
        advance();
    }
}

void Lexer::skip_block_comment() {
    while (!is_at_end()) {
        if (peek() == '*' && peek_next() == '/') {
            advance(); // consume '*'
            advance(); // consume '/'
            break;
        }
        if (peek() == '\n') {
            line_++;
            column_ = 0; // Will be incremented by advance()
        }
        advance();
    }
}

Token Lexer::string_literal() {
    std::string value;
    
    while (peek() != '"' && !is_at_end()) {
        if (peek() == '\n') {
            line_++;
            column_ = 0; // Will be incremented by advance()
        }
        if (peek() == '\\') {
            advance(); // consume backslash
            char escaped = peek();
            switch (escaped) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case 'r': value += '\r'; break;
                case '\\': value += '\\'; break;
                case '"': value += '"'; break;
                default:
                    add_error("Unknown escape sequence: \\" + std::string(1, escaped));
                    value += escaped;
                    break;
            }
            advance();
        } else {
            value += advance();
        }
    }
    
    if (is_at_end()) {
        add_error("Unterminated string");
        return error_token("Unterminated string");
    }
    
    advance(); // consume closing quote
    return make_token(TokenType::STRING, value);
}

Token Lexer::number_literal() {
    size_t start = current_;
    
    while (is_digit(peek())) {
        advance();
    }
    
    bool is_float = false;
    if (peek() == '.' && is_digit(peek_next())) {
        is_float = true;
        advance(); // consume '.'
        while (is_digit(peek())) {
            advance();
        }
    }
    
    std::string number_str = source_.substr(start, current_ - start);
    
    if (is_float) {
        double value = std::stod(number_str);
        return make_token(TokenType::FLOAT, value);
    } else {
        int64_t value = std::stoll(number_str);
        return make_token(TokenType::INTEGER, value);
    }
}

Token Lexer::identifier_or_keyword() {
    size_t start = current_;
    
    while (is_alnum(peek()) || peek() == '_') {
        advance();
    }
    
    std::string text = source_.substr(start, current_ - start);
    
    // Check for keywords
    auto keyword_it = keywords_.find(text);
    if (keyword_it != keywords_.end()) {
        TokenType type = keyword_it->second;
        if (type == TokenType::BOOLEAN) {
            bool value = (text == "true");
            return make_token(type, value);
        }
        return make_token(type);
    }
    
    return make_token(TokenType::IDENTIFIER);
}

Token Lexer::annotation() {
    size_t start = current_ - 1; // Include the '@'
    
    while (is_alnum(peek()) || peek() == '_') {
        advance();
    }
    
    std::string text = source_.substr(start, current_ - start);
    
    auto annotation_it = annotations_.find(text);
    if (annotation_it != annotations_.end()) {
        return make_token(annotation_it->second);
    }
    
    add_error("Unknown annotation: " + text);
    return error_token("Unknown annotation");
}

Token Lexer::semantic_tag() {
    advance(); // Skip '#'
    
    while (is_alnum(peek()) || peek() == '_' || peek() == ':') {
        advance();
    }
    
    return make_token(TokenType::TAG);
}

bool Lexer::is_alpha(char c) const {
    return std::isalpha(c) || c == '_';
}

bool Lexer::is_digit(char c) const {
    return std::isdigit(c);
}

bool Lexer::is_alnum(char c) const {
    return std::isalnum(c) || c == '_';
}

Token Lexer::make_token(TokenType type) {
    return Token(type, "", line_, column_);
}

Token Lexer::make_token(TokenType type, int64_t value) {
    return Token(type, std::to_string(value), value, line_, column_);
}

Token Lexer::make_token(TokenType type, double value) {
    return Token(type, std::to_string(value), value, line_, column_);
}

Token Lexer::make_token(TokenType type, const std::string& value) {
    return Token(type, value, value, line_, column_);
}

Token Lexer::make_token(TokenType type, bool value) {
    return Token(type, value ? "true" : "false", value, line_, column_);
}

Token Lexer::error_token(const std::string& message) {
    return Token(TokenType::ERROR, message, line_, column_);
}

void Lexer::add_error(const std::string& message) {
    errors_.push_back("Line " + std::to_string(line_) + 
                      ", Column " + std::to_string(column_) + 
                      ": " + message);
}

} // namespace pomegranate
