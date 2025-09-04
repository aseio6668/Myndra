#ifndef MYNDRA_AST_H
#define MYNDRA_AST_H

#include <memory>
#include <vector>
#include <string>
#include <variant>
#include "../lexer/token.h"

namespace myndra {

// Forward declarations
class ASTVisitor;

// Base AST node class
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
    virtual std::string to_string() const = 0;
    
    // Source location information
    size_t line = 0;
    size_t column = 0;
};

// Expression nodes
class Expression : public ASTNode {
public:
    virtual ~Expression() = default;
};

class Statement : public ASTNode {
public:
    virtual ~Statement() = default;
};

// Literal expressions
class IntegerLiteral : public Expression {
public:
    int64_t value;
    
    explicit IntegerLiteral(int64_t val) : value(val) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override {
        return std::to_string(value);
    }
};

class FloatLiteral : public Expression {
public:
    double value;
    
    explicit FloatLiteral(double val) : value(val) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override {
        return std::to_string(value);
    }
};

class StringLiteral : public Expression {
public:
    std::string value;
    
    explicit StringLiteral(std::string val) : value(std::move(val)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override {
        return "\"" + value + "\"";
    }
};

class BooleanLiteral : public Expression {
public:
    bool value;
    
    explicit BooleanLiteral(bool val) : value(val) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override {
        return value ? "true" : "false";
    }
};

class Identifier : public Expression {
public:
    std::string name;
    
    explicit Identifier(std::string n) : name(std::move(n)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override {
        return name;
    }
};

// Binary operations
enum class BinaryOperator {
    Add, Sub, Mul, Div, Mod,           // Arithmetic
    Eq, Ne, Lt, Le, Gt, Ge,            // Comparison
    And, Or,                           // Logical
    Assign                             // Assignment
};

class BinaryExpression : public Expression {
public:
    std::unique_ptr<Expression> left;
    BinaryOperator op;
    std::unique_ptr<Expression> right;
    
    BinaryExpression(std::unique_ptr<Expression> l, BinaryOperator o, std::unique_ptr<Expression> r)
        : left(std::move(l)), op(o), right(std::move(r)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
    
private:
    std::string operator_to_string(BinaryOperator op) const;
};

// Unary operations
enum class UnaryOperator {
    Not, Neg, Plus
};

class UnaryExpression : public Expression {
public:
    UnaryOperator op;
    std::unique_ptr<Expression> operand;
    
    UnaryExpression(UnaryOperator o, std::unique_ptr<Expression> expr)
        : op(o), operand(std::move(expr)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
    
private:
    std::string operator_to_string(UnaryOperator op) const;
};

// Function call
class FunctionCall : public Expression {
public:
    std::unique_ptr<Expression> function;  // Usually an Identifier
    std::vector<std::unique_ptr<Expression>> arguments;
    
    FunctionCall(std::unique_ptr<Expression> func, std::vector<std::unique_ptr<Expression>> args)
        : function(std::move(func)), arguments(std::move(args)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
};

// Array access
class ArrayAccess : public Expression {
public:
    std::unique_ptr<Expression> array;
    std::unique_ptr<Expression> index;
    
    ArrayAccess(std::unique_ptr<Expression> arr, std::unique_ptr<Expression> idx)
        : array(std::move(arr)), index(std::move(idx)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
};

// Member access (for future struct support)
class MemberAccess : public Expression {
public:
    std::unique_ptr<Expression> object;
    std::string member;
    
    MemberAccess(std::unique_ptr<Expression> obj, std::string mem)
        : object(std::move(obj)), member(std::move(mem)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
};

// Context-aware conditional expression
class ContextConditional : public Expression {
public:
    std::unique_ptr<Expression> expression;
    std::string context;  // "dev", "prod", "test"
    
    ContextConditional(std::unique_ptr<Expression> expr, std::string ctx)
        : expression(std::move(expr)), context(std::move(ctx)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
};

// Statement nodes
class ExpressionStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;
    
    explicit ExpressionStatement(std::unique_ptr<Expression> expr)
        : expression(std::move(expr)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
};

// Variable declaration
class VariableDeclaration : public Statement {
public:
    std::string name;
    std::string type;  // Optional type annotation (empty if inferred)
    std::unique_ptr<Expression> initializer;
    bool is_mutable;
    
    VariableDeclaration(std::string n, std::string t, std::unique_ptr<Expression> init, bool mut = false)
        : name(std::move(n)), type(std::move(t)), initializer(std::move(init)), is_mutable(mut) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
};

// Function parameter
struct Parameter {
    std::string name;
    std::string type;
    
    Parameter(std::string n, std::string t) : name(std::move(n)), type(std::move(t)) {}
};

// Block statement
class Block : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    
    explicit Block(std::vector<std::unique_ptr<Statement>> stmts)
        : statements(std::move(stmts)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
};

// Function definition
class FunctionDefinition : public Statement {
public:
    std::string name;
    std::vector<Parameter> parameters;
    std::string return_type;  // Optional return type (empty if void/inferred)
    std::unique_ptr<Block> body;
    
    FunctionDefinition(std::string n, std::vector<Parameter> params, std::string ret_type, std::unique_ptr<Block> b)
        : name(std::move(n)), parameters(std::move(params)), return_type(std::move(ret_type)), body(std::move(b)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
};

// Return statement
class ReturnStatement : public Statement {
public:
    std::unique_ptr<Expression> value;  // nullptr for bare "return"
    
    explicit ReturnStatement(std::unique_ptr<Expression> val = nullptr)
        : value(std::move(val)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
};

// If statement
class IfStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> then_branch;
    std::unique_ptr<Statement> else_branch;  // nullptr if no else
    
    IfStatement(std::unique_ptr<Expression> cond, std::unique_ptr<Statement> then_stmt, std::unique_ptr<Statement> else_stmt = nullptr)
        : condition(std::move(cond)), then_branch(std::move(then_stmt)), else_branch(std::move(else_stmt)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
};

// While loop
class WhileStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> body;
    
    WhileStatement(std::unique_ptr<Expression> cond, std::unique_ptr<Statement> b)
        : condition(std::move(cond)), body(std::move(b)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
};

// For loop (simplified version)
class ForStatement : public Statement {
public:
    std::string variable;     // Loop variable name
    std::unique_ptr<Expression> start;      // Start value
    std::unique_ptr<Expression> end;        // End value  
    std::unique_ptr<Statement> body;
    
    ForStatement(std::string var, std::unique_ptr<Expression> s, std::unique_ptr<Expression> e, std::unique_ptr<Statement> b)
        : variable(std::move(var)), start(std::move(s)), end(std::move(e)), body(std::move(b)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
};

// Program (top-level)
class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    
    explicit Program(std::vector<std::unique_ptr<Statement>> stmts)
        : statements(std::move(stmts)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string to_string() const override;
};

// Visitor pattern for AST traversal
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    // Expression visitors
    virtual void visit(IntegerLiteral& node) = 0;
    virtual void visit(FloatLiteral& node) = 0;
    virtual void visit(StringLiteral& node) = 0;
    virtual void visit(BooleanLiteral& node) = 0;
    virtual void visit(Identifier& node) = 0;
    virtual void visit(BinaryExpression& node) = 0;
    virtual void visit(UnaryExpression& node) = 0;
    virtual void visit(FunctionCall& node) = 0;
    virtual void visit(ArrayAccess& node) = 0;
    virtual void visit(MemberAccess& node) = 0;
    virtual void visit(ContextConditional& node) = 0;
    
    // Statement visitors
    virtual void visit(ExpressionStatement& node) = 0;
    virtual void visit(VariableDeclaration& node) = 0;
    virtual void visit(Block& node) = 0;
    virtual void visit(FunctionDefinition& node) = 0;
    virtual void visit(ReturnStatement& node) = 0;
    virtual void visit(IfStatement& node) = 0;
    virtual void visit(WhileStatement& node) = 0;
    virtual void visit(ForStatement& node) = 0;
    virtual void visit(Program& node) = 0;
};

} // namespace myndra

#endif // MYNDRA_AST_H