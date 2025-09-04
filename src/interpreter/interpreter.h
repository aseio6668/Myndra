#ifndef MYNDRA_INTERPRETER_H
#define MYNDRA_INTERPRETER_H

#include "../parser/ast.h"
#include <unordered_map>
#include <variant>
#include <string>
#include <vector>
#include <memory>

namespace myndra {

// Runtime value type (internal to interpreter)
using RuntimeValue = std::variant<int64_t, double, std::string, bool>;

// Environment for variable storage
class Environment {
public:
    Environment(std::shared_ptr<Environment> parent = nullptr);
    
    void define(const std::string& name, const RuntimeValue& value);
    RuntimeValue get(const std::string& name) const;
    void assign(const std::string& name, const RuntimeValue& value);
    
    std::shared_ptr<Environment> getParent() const { return parent_; }
    
private:
    std::shared_ptr<Environment> parent_;
    std::unordered_map<std::string, RuntimeValue> variables_;
};

// Interpreter that executes AST
class Interpreter : public ASTVisitor {
public:
    Interpreter();
    
    // Execute a program
    void execute(Program& program);
    
    // ASTVisitor implementation
    void visit(IntegerLiteral& node) override;
    void visit(FloatLiteral& node) override;
    void visit(StringLiteral& node) override;
    void visit(BooleanLiteral& node) override;
    void visit(Identifier& node) override;
    void visit(BinaryExpression& node) override;
    void visit(UnaryExpression& node) override;
    void visit(FunctionCall& node) override;
    void visit(ArrayAccess& node) override;
    void visit(MemberAccess& node) override;
    void visit(ContextConditional& node) override;
    void visit(ExpressionStatement& node) override;
    void visit(VariableDeclaration& node) override;
    void visit(Block& node) override;
    void visit(FunctionDefinition& node) override;
    void visit(ReturnStatement& node) override;
    void visit(IfStatement& node) override;
    void visit(WhileStatement& node) override;
    void visit(ForStatement& node) override;
    void visit(Program& node) override;
    
    // Utility methods
    std::string valueToString(const RuntimeValue& value) const;
    bool isTruthy(const RuntimeValue& value) const;
    
private:
    std::shared_ptr<Environment> environment_;
    RuntimeValue lastValue_; // For expression results
    
    // Built-in functions
    void setupBuiltins();
    RuntimeValue callPrint(const std::vector<RuntimeValue>& args);
    RuntimeValue callInput(const std::vector<RuntimeValue>& args);
    RuntimeValue callLength(const std::vector<RuntimeValue>& args);
    RuntimeValue callSubstring(const std::vector<RuntimeValue>& args);
};

} // namespace myndra

#endif // MYNDRA_INTERPRETER_H