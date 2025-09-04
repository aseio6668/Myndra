#ifndef MYNDRA_PARSER_H
#define MYNDRA_PARSER_H

#include "ast.h"
#include "../lexer/lexer.h"
#include <memory>
#include <vector>
#include <string>

namespace myndra {

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    
    // Main parsing entry points
    std::unique_ptr<Program> parseProgram();
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Statement> parseStatement();
    
    // Error handling
    bool hasErrors() const { return !errors_.empty(); }
    const std::vector<std::string>& getErrors() const { return errors_; }
    
private:
    std::vector<Token> tokens_;
    size_t current_;
    std::vector<std::string> errors_;
    
    // Utility methods
    const Token& currentToken() const;
    const Token& peekToken(size_t offset = 1) const;
    bool isAtEnd() const;
    Token advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool match(const std::vector<TokenType>& types);
    Token consume(TokenType type, const std::string& message);
    
    // Error handling
    void error(const std::string& message);
    void synchronize();
    
    // Expression parsing (precedence climbing)
    std::unique_ptr<Expression> parseAssignment();
    std::unique_ptr<Expression> parseLogicalOr();
    std::unique_ptr<Expression> parseLogicalAnd();
    std::unique_ptr<Expression> parseEquality();
    std::unique_ptr<Expression> parseComparison();
    std::unique_ptr<Expression> parseTerm();
    std::unique_ptr<Expression> parseFactor();
    std::unique_ptr<Expression> parseUnary();
    std::unique_ptr<Expression> parseCall();
    std::unique_ptr<Expression> parsePrimary();
    
    // Context-aware parsing
    std::unique_ptr<Expression> parseContextConditional(std::unique_ptr<Expression> expr);
    
    // Helper for function calls and array access
    std::unique_ptr<Expression> finishCall(std::unique_ptr<Expression> expr);
    std::unique_ptr<Expression> finishArrayAccess(std::unique_ptr<Expression> expr);
    std::unique_ptr<Expression> finishMemberAccess(std::unique_ptr<Expression> expr);
    
    // Statement parsing
    std::unique_ptr<Statement> parseDeclaration();
    std::unique_ptr<Statement> parseVarDeclaration();
    std::unique_ptr<Statement> parseFunctionDeclaration();
    std::unique_ptr<Statement> parseIfStatement();
    std::unique_ptr<Statement> parseWhileStatement();
    std::unique_ptr<Statement> parseForStatement();
    std::unique_ptr<Statement> parseReturnStatement();
    std::unique_ptr<Statement> parseBlockStatement();
    std::unique_ptr<Statement> parseExpressionStatement();
    
    // Helper methods
    std::vector<Parameter> parseParameterList();
    std::vector<std::unique_ptr<Expression>> parseArgumentList();
    
    // Type parsing (for future type system)
    std::string parseType();
    
    // Operator precedence helpers
    int getBinaryPrecedence(TokenType type) const;
    BinaryOperator tokenToBinaryOperator(TokenType type) const;
    UnaryOperator tokenToUnaryOperator(TokenType type) const;
};

} // namespace myndra

#endif // MYNDRA_PARSER_H