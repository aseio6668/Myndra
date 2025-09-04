#include "parser.h"
#include <iostream>
#include <sstream>

namespace myndra {

Parser::Parser(const std::vector<Token>& tokens) : tokens_(tokens), current_(0) {
    // Constructor
}

// Utility methods
const Token& Parser::currentToken() const {
    if (isAtEnd()) {
        static Token eof_token{TokenType::EOF_TOKEN, "", 0, 0};
        return eof_token;
    }
    return tokens_[current_];
}

const Token& Parser::peekToken(size_t offset) const {
    size_t pos = current_ + offset;
    if (pos >= tokens_.size()) {
        static Token eof_token{TokenType::EOF_TOKEN, "", 0, 0};
        return eof_token;
    }
    return tokens_[pos];
}

bool Parser::isAtEnd() const {
    return current_ >= tokens_.size() || (current_ < tokens_.size() && tokens_[current_].type == TokenType::EOF_TOKEN);
}

Token Parser::advance() {
    if (!isAtEnd()) current_++;
    // DEBUG: Token advance removed for cleaner output
    return tokens_[current_ - 1];
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return currentToken().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        return advance();
    }
    
    error(message);
    return currentToken();
}

void Parser::error(const std::string& message) {
    const Token& token = currentToken();
    std::ostringstream oss;
    oss << "Parse error at line " << token.line << ", column " << token.column;
    oss << ": " << message;
    oss << " (got '" << token.lexeme << "')";
    errors_.push_back(oss.str());
}

void Parser::synchronize() {
    advance();
    
    while (!isAtEnd()) {
        if (tokens_[current_ - 1].type == TokenType::SEMICOLON) return;
        
        switch (currentToken().type) {
            case TokenType::FN:
            case TokenType::LET:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::FOR:
            case TokenType::RETURN:
                return;
            default:
                break;
        }
        
        advance();
    }
}

// Main parsing methods
std::unique_ptr<Program> Parser::parseProgram() {
    std::vector<std::unique_ptr<Statement>> statements;
    
    while (!isAtEnd()) {
        // Skip newlines between statements
        while (match(TokenType::NEWLINE)) {
            // Continue skipping newlines
        }
        
        if (isAtEnd()) break;
        
        // DEBUG: Program parsing debug removed for cleaner output
        auto stmt = parseDeclaration();
        if (stmt) {
            // DEBUG: Success message removed for cleaner output
            statements.push_back(std::move(stmt));
        } else {
            // DEBUG: Error message removed for cleaner output
            synchronize();
        }
    }
    
    return std::make_unique<Program>(std::move(statements));
}

std::unique_ptr<Expression> Parser::parseExpression() {
    return parseAssignment();
}

std::unique_ptr<Statement> Parser::parseStatement() {
    if (match(TokenType::IF)) return parseIfStatement();
    if (match(TokenType::WHILE)) return parseWhileStatement();
    if (match(TokenType::FOR)) return parseForStatement();
    if (match(TokenType::RETURN)) return parseReturnStatement();
    if (match(TokenType::LEFT_BRACE)) {
        current_--; // Back up to let parseBlockStatement consume the brace
        return parseBlockStatement();
    }
    
    return parseExpressionStatement();
}

// Expression parsing (precedence climbing)
std::unique_ptr<Expression> Parser::parseAssignment() {
    auto expr = parseLogicalOr();
    
    if (match(TokenType::ASSIGN)) {
        auto equals = tokens_[current_ - 1];
        auto value = parseAssignment();
        
        // Check if left side is assignable (identifier for now)
        if (auto identifier = dynamic_cast<Identifier*>(expr.get())) {
            return std::make_unique<BinaryExpression>(std::move(expr), BinaryOperator::Assign, std::move(value));
        }
        
        error("Invalid assignment target");
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parseLogicalOr() {
    auto expr = parseLogicalAnd();
    
    while (match(TokenType::OR)) {
        auto op = BinaryOperator::Or;
        auto right = parseLogicalAnd();
        expr = std::make_unique<BinaryExpression>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parseLogicalAnd() {
    auto expr = parseEquality();
    
    while (match(TokenType::AND)) {
        auto op = BinaryOperator::And;
        auto right = parseEquality();
        expr = std::make_unique<BinaryExpression>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parseEquality() {
    auto expr = parseComparison();
    
    while (match({TokenType::EQUAL, TokenType::NOT_EQUAL})) {
        auto op = tokenToBinaryOperator(tokens_[current_ - 1].type);
        auto right = parseComparison();
        expr = std::make_unique<BinaryExpression>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parseComparison() {
    auto expr = parseTerm();
    
    while (match({TokenType::LESS, TokenType::LESS_EQUAL, TokenType::GREATER, TokenType::GREATER_EQUAL})) {
        auto op = tokenToBinaryOperator(tokens_[current_ - 1].type);
        auto right = parseTerm();
        expr = std::make_unique<BinaryExpression>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parseTerm() {
    auto expr = parseFactor();
    
    while (match({TokenType::PLUS, TokenType::MINUS})) {
        auto op = tokenToBinaryOperator(tokens_[current_ - 1].type);
        auto right = parseFactor();
        expr = std::make_unique<BinaryExpression>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parseFactor() {
    auto expr = parseUnary();
    
    while (match({TokenType::MULTIPLY, TokenType::DIVIDE, TokenType::MODULO})) {
        auto op = tokenToBinaryOperator(tokens_[current_ - 1].type);
        auto right = parseUnary();
        expr = std::make_unique<BinaryExpression>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parseUnary() {
    if (match({TokenType::NOT, TokenType::MINUS, TokenType::PLUS})) {
        auto op = tokenToUnaryOperator(tokens_[current_ - 1].type);
        auto right = parseUnary();
        return std::make_unique<UnaryExpression>(op, std::move(right));
    }
    
    return parseCall();
}

std::unique_ptr<Expression> Parser::parseCall() {
    auto expr = parsePrimary();
    
    while (true) {
        if (match(TokenType::LEFT_PAREN)) {
            expr = finishCall(std::move(expr));
        } else if (match(TokenType::LEFT_BRACKET)) {
            expr = finishArrayAccess(std::move(expr));
        } else if (match(TokenType::DOT)) {
            expr = finishMemberAccess(std::move(expr));
        } else {
            break;
        }
    }
    
    // Check for context-aware conditional
    if (check(TokenType::IF) && peekToken().type == TokenType::IDENTIFIER && peekToken(2).type == TokenType::EQUAL) {
        return parseContextConditional(std::move(expr));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parsePrimary() {
    if (match(TokenType::TRUE)) {
        return std::make_unique<BooleanLiteral>(true);
    }
    
    if (match(TokenType::FALSE)) {
        return std::make_unique<BooleanLiteral>(false);
    }
    
    if (match(TokenType::INTEGER)) {
        int64_t value = std::stoll(tokens_[current_ - 1].lexeme);
        return std::make_unique<IntegerLiteral>(value);
    }
    
    if (match(TokenType::FLOAT)) {
        double value = std::stod(tokens_[current_ - 1].lexeme);
        return std::make_unique<FloatLiteral>(value);
    }
    
    if (match(TokenType::STRING)) {
        std::string value = tokens_[current_ - 1].lexeme;
        // Remove surrounding quotes
        if (value.length() >= 2 && value.front() == '"' && value.back() == '"') {
            value = value.substr(1, value.length() - 2);
        }
        return std::make_unique<StringLiteral>(value);
    }
    
    if (match(TokenType::IDENTIFIER)) {
        return std::make_unique<Identifier>(tokens_[current_ - 1].lexeme);
    }
    
    if (match(TokenType::LEFT_PAREN)) {
        auto expr = parseExpression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
        return expr;
    }
    
    error("Expect expression");
    if (!isAtEnd()) advance(); // Advance to avoid infinite loops
    return nullptr;
}

// Context-aware parsing
std::unique_ptr<Expression> Parser::parseContextConditional(std::unique_ptr<Expression> expr) {
    consume(TokenType::IF, "Expected 'if' for context conditional");
    consume(TokenType::IDENTIFIER, "Expected 'context' identifier");
    consume(TokenType::EQUAL, "Expected '==' in context conditional");
    consume(TokenType::EQUAL, "Expected '==' in context conditional");
    
    if (!match(TokenType::STRING)) {
        error("Expected context string (\"dev\", \"prod\", or \"test\")");
        return expr;
    }
    
    std::string context = tokens_[current_ - 1].lexeme;
    // Remove quotes
    if (context.length() >= 2 && context.front() == '"' && context.back() == '"') {
        context = context.substr(1, context.length() - 2);
    }
    
    return std::make_unique<ContextConditional>(std::move(expr), context);
}

// Helper methods for call, array access, member access
std::unique_ptr<Expression> Parser::finishCall(std::unique_ptr<Expression> callee) {
    std::vector<std::unique_ptr<Expression>> arguments;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            arguments.push_back(parseExpression());
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments");
    return std::make_unique<FunctionCall>(std::move(callee), std::move(arguments));
}

std::unique_ptr<Expression> Parser::finishArrayAccess(std::unique_ptr<Expression> array) {
    auto index = parseExpression();
    consume(TokenType::RIGHT_BRACKET, "Expect ']' after array index");
    return std::make_unique<ArrayAccess>(std::move(array), std::move(index));
}

std::unique_ptr<Expression> Parser::finishMemberAccess(std::unique_ptr<Expression> object) {
    Token name = consume(TokenType::IDENTIFIER, "Expect property name after '.'");
    return std::make_unique<MemberAccess>(std::move(object), name.lexeme);
}

// Statement parsing
std::unique_ptr<Statement> Parser::parseDeclaration() {
    try {
        if (match(TokenType::FN)) return parseFunctionDeclaration();
        if (match(TokenType::LET)) return parseVarDeclaration();
        
        return parseStatement();
    } catch (...) {
        synchronize();
        return nullptr;
    }
}

std::unique_ptr<Statement> Parser::parseVarDeclaration() {
    bool is_mutable = match(TokenType::MUT);
    
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name");
    
    std::string type;
    if (match(TokenType::COLON)) {
        type = parseType();
    }
    
    std::unique_ptr<Expression> initializer = nullptr;
    if (match(TokenType::ASSIGN)) {
        initializer = parseExpression();
    }
    
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration");
    
    return std::make_unique<VariableDeclaration>(name.lexeme, type, std::move(initializer), is_mutable);
}

std::unique_ptr<Statement> Parser::parseFunctionDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect function name");
    
    consume(TokenType::LEFT_PAREN, "Expect '(' after function name");
    auto parameters = parseParameterList();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters");
    
    std::string return_type;
    if (match(TokenType::ARROW)) {
        return_type = parseType();
    }
    
    auto body = parseBlockStatement();
    auto block_ptr = std::unique_ptr<Block>(dynamic_cast<Block*>(body.release()));
    
    return std::make_unique<FunctionDefinition>(name.lexeme, std::move(parameters), return_type, std::move(block_ptr));
}

std::unique_ptr<Statement> Parser::parseIfStatement() {
    auto condition = parseExpression();
    auto then_branch = parseStatement();
    
    std::unique_ptr<Statement> else_branch = nullptr;
    if (match(TokenType::ELSE)) {
        else_branch = parseStatement();
    }
    
    return std::make_unique<IfStatement>(std::move(condition), std::move(then_branch), std::move(else_branch));
}

std::unique_ptr<Statement> Parser::parseWhileStatement() {
    auto condition = parseExpression();
    auto body = parseStatement();
    
    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
}

std::unique_ptr<Statement> Parser::parseForStatement() {
    Token var_name = consume(TokenType::IDENTIFIER, "Expect loop variable name");
    consume(TokenType::IN, "Expect 'in' after loop variable");
    
    auto start = parseExpression();
    consume(TokenType::DOT, "Expect '..' in range");
    consume(TokenType::DOT, "Expect '..' in range");
    auto end = parseExpression();
    
    auto body = parseStatement();
    
    return std::make_unique<ForStatement>(var_name.lexeme, std::move(start), std::move(end), std::move(body));
}

std::unique_ptr<Statement> Parser::parseReturnStatement() {
    std::unique_ptr<Expression> value = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        value = parseExpression();
    }
    
    consume(TokenType::SEMICOLON, "Expect ';' after return value");
    return std::make_unique<ReturnStatement>(std::move(value));
}

std::unique_ptr<Statement> Parser::parseBlockStatement() {
    std::vector<std::unique_ptr<Statement>> statements;
    
    consume(TokenType::LEFT_BRACE, "Expect '{'");
    
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        // Skip newlines between statements
        while (match(TokenType::NEWLINE)) {
            // Continue skipping newlines
        }
        
        if (check(TokenType::RIGHT_BRACE) || isAtEnd()) break;
        
        statements.push_back(parseDeclaration());
    }
    
    consume(TokenType::RIGHT_BRACE, "Expect '}'");
    return std::make_unique<Block>(std::move(statements));
}

std::unique_ptr<Statement> Parser::parseExpressionStatement() {
    auto expr = parseExpression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression");
    return std::make_unique<ExpressionStatement>(std::move(expr));
}

// Helper methods
std::vector<Parameter> Parser::parseParameterList() {
    std::vector<Parameter> parameters;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            Token name = consume(TokenType::IDENTIFIER, "Expect parameter name");
            consume(TokenType::COLON, "Expect ':' after parameter name");
            std::string type = parseType();
            parameters.emplace_back(name.lexeme, type);
        } while (match(TokenType::COMMA));
    }
    
    return parameters;
}

std::string Parser::parseType() {
    if (match(TokenType::IDENTIFIER)) {
        return tokens_[current_ - 1].lexeme;
    }
    
    error("Expected type name");
    return "";
}

// Operator conversion helpers
BinaryOperator Parser::tokenToBinaryOperator(TokenType type) const {
    switch (type) {
        case TokenType::PLUS: return BinaryOperator::Add;
        case TokenType::MINUS: return BinaryOperator::Sub;
        case TokenType::MULTIPLY: return BinaryOperator::Mul;
        case TokenType::DIVIDE: return BinaryOperator::Div;
        case TokenType::MODULO: return BinaryOperator::Mod;
        case TokenType::EQUAL: return BinaryOperator::Eq;
        case TokenType::NOT_EQUAL: return BinaryOperator::Ne;
        case TokenType::LESS: return BinaryOperator::Lt;
        case TokenType::LESS_EQUAL: return BinaryOperator::Le;
        case TokenType::GREATER: return BinaryOperator::Gt;
        case TokenType::GREATER_EQUAL: return BinaryOperator::Ge;
        case TokenType::AND: return BinaryOperator::And;
        case TokenType::OR: return BinaryOperator::Or;
        case TokenType::ASSIGN: return BinaryOperator::Assign;
        default:
            // Should not reach here
            return BinaryOperator::Add;
    }
}

UnaryOperator Parser::tokenToUnaryOperator(TokenType type) const {
    switch (type) {
        case TokenType::NOT: return UnaryOperator::Not;
        case TokenType::MINUS: return UnaryOperator::Neg;
        case TokenType::PLUS: return UnaryOperator::Plus;
        default:
            // Should not reach here
            return UnaryOperator::Neg;
    }
}

// Missing method implementations
int Parser::getBinaryPrecedence(TokenType type) const {
    switch (type) {
        case TokenType::OR: return 1;
        case TokenType::AND: return 2;
        case TokenType::EQUAL:
        case TokenType::NOT_EQUAL: return 3;
        case TokenType::LESS:
        case TokenType::LESS_EQUAL:
        case TokenType::GREATER:
        case TokenType::GREATER_EQUAL: return 4;
        case TokenType::PLUS:
        case TokenType::MINUS: return 5;
        case TokenType::MULTIPLY:
        case TokenType::DIVIDE:
        case TokenType::MODULO: return 6;
        default: return 0;
    }
}

std::vector<std::unique_ptr<Expression>> Parser::parseArgumentList() {
    std::vector<std::unique_ptr<Expression>> arguments;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            arguments.push_back(parseExpression());
        } while (match(TokenType::COMMA));
    }
    
    return arguments;
}

} // namespace myndra