#ifndef MYNDRA_TOKEN_H
#define MYNDRA_TOKEN_H

#include <string>
#include <variant>

namespace myndra {

enum class TokenType {
    // Literals
    INTEGER,
    FLOAT,
    STRING,
    BOOLEAN,
    NIL,
    
    // Identifiers and keywords
    IDENTIFIER,
    
    // Keywords
    LET,
    MUT,
    FN,
    IF,
    ELSE,
    WHILE,
    FOR,
    IN,
    RETURN,
    TRUE,
    FALSE,
    IMPORT,
    EXPORT,
    WITH,
    CAPABILITIES,
    CAPSULE,
    DSL,
    FALLBACK,
    RETRY,
    CONTEXT,
    OVER,
    TAG,
    DID,
    EVOLVING,
    
    // Execution model annotations
    AT_SYNC,      // @sync
    AT_ASYNC,     // @async
    AT_PARALLEL,  // @parallel
    AT_REACTIVE,  // @reactive
    AT_TEMPORAL,  // @temporal
    
    // Operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULO,
    ASSIGN,
    PLUS_ASSIGN,
    MINUS_ASSIGN,
    ARROW,        // ->
    FAT_ARROW,    // =>
    
    // Comparison
    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    
    // Logical
    AND,
    OR,
    NOT,
    
    // Punctuation
    SEMICOLON,
    COMMA,
    DOT,
    COLON,
    DOUBLE_COLON, // ::
    QUESTION,
    
    // Brackets
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_ANGLE,    // < for generics
    RIGHT_ANGLE,   // > for generics
    
    // Special
    HASH,          // # for semantic tags
    EOF_TOKEN,
    NEWLINE,
    COMMENT,
    
    // Context-aware
    IF_CONTEXT,    // if context == "dev"
    
    // Reactive
    OBSERVABLE,
    SUBSCRIBE,
    EMIT,
    
    // Temporal
    TRANSITION,
    TIMELINE,
    
    // DID/ZK
    VERIFY,
    PROOF,
    HAS_PROOF,
    
    // Error
    ERROR
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::variant<int64_t, double, std::string, bool> literal;
    size_t line;
    size_t column;
    
    Token(TokenType t, std::string lex, size_t ln, size_t col)
        : type(t), lexeme(std::move(lex)), line(ln), column(col) {}
    
    Token(TokenType t, std::string lex, int64_t value, size_t ln, size_t col)
        : type(t), lexeme(std::move(lex)), literal(value), line(ln), column(col) {}
    
    Token(TokenType t, std::string lex, double value, size_t ln, size_t col)
        : type(t), lexeme(std::move(lex)), literal(value), line(ln), column(col) {}
    
    Token(TokenType t, std::string lex, std::string value, size_t ln, size_t col)
        : type(t), lexeme(std::move(lex)), literal(std::move(value)), line(ln), column(col) {}
    
    Token(TokenType t, std::string lex, bool value, size_t ln, size_t col)
        : type(t), lexeme(std::move(lex)), literal(value), line(ln), column(col) {}
};

const char* token_type_to_string(TokenType type);

} // namespace myndra

#endif // MYNDRA_TOKEN_H
