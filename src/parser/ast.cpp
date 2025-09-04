#include "ast.h"
#include <sstream>

namespace myndra {

// IntegerLiteral
void IntegerLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// FloatLiteral
void FloatLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// StringLiteral
void StringLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// BooleanLiteral
void BooleanLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// Identifier
void Identifier::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// BinaryExpression
void BinaryExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string BinaryExpression::to_string() const {
    return "(" + left->to_string() + " " + operator_to_string(op) + " " + right->to_string() + ")";
}

std::string BinaryExpression::operator_to_string(BinaryOperator op) const {
    switch (op) {
        case BinaryOperator::Add: return "+";
        case BinaryOperator::Sub: return "-";
        case BinaryOperator::Mul: return "*";
        case BinaryOperator::Div: return "/";
        case BinaryOperator::Mod: return "%";
        case BinaryOperator::Eq: return "==";
        case BinaryOperator::Ne: return "!=";
        case BinaryOperator::Lt: return "<";
        case BinaryOperator::Le: return "<=";
        case BinaryOperator::Gt: return ">";
        case BinaryOperator::Ge: return ">=";
        case BinaryOperator::And: return "and";
        case BinaryOperator::Or: return "or";
        case BinaryOperator::Assign: return "=";
        default: return "<?>";
    }
}

// UnaryExpression
void UnaryExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string UnaryExpression::to_string() const {
    return "(" + operator_to_string(op) + operand->to_string() + ")";
}

std::string UnaryExpression::operator_to_string(UnaryOperator op) const {
    switch (op) {
        case UnaryOperator::Not: return "not ";
        case UnaryOperator::Neg: return "-";
        case UnaryOperator::Plus: return "+";
        default: return "<?>";
    }
}

// FunctionCall
void FunctionCall::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string FunctionCall::to_string() const {
    std::ostringstream oss;
    oss << function->to_string() << "(";
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << arguments[i]->to_string();
    }
    oss << ")";
    return oss.str();
}

// ArrayAccess
void ArrayAccess::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ArrayAccess::to_string() const {
    return array->to_string() + "[" + index->to_string() + "]";
}

// MemberAccess
void MemberAccess::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string MemberAccess::to_string() const {
    return object->to_string() + "." + member;
}

// ContextConditional
void ContextConditional::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ContextConditional::to_string() const {
    return expression->to_string() + " if context == \"" + context + "\"";
}

// ExpressionStatement
void ExpressionStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ExpressionStatement::to_string() const {
    return expression->to_string();
}

// VariableDeclaration
void VariableDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string VariableDeclaration::to_string() const {
    std::ostringstream oss;
    oss << "let ";
    if (is_mutable) oss << "mut ";
    oss << name;
    if (!type.empty()) oss << ": " << type;
    if (initializer) oss << " = " << initializer->to_string();
    return oss.str();
}

// Block
void Block::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string Block::to_string() const {
    std::ostringstream oss;
    oss << "{\n";
    for (const auto& stmt : statements) {
        oss << "  " << stmt->to_string() << "\n";
    }
    oss << "}";
    return oss.str();
}

// FunctionDefinition
void FunctionDefinition::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string FunctionDefinition::to_string() const {
    std::ostringstream oss;
    oss << "fn " << name << "(";
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << parameters[i].name << ": " << parameters[i].type;
    }
    oss << ")";
    if (!return_type.empty()) oss << " -> " << return_type;
    oss << " " << body->to_string();
    return oss.str();
}

// ReturnStatement
void ReturnStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ReturnStatement::to_string() const {
    if (value) {
        return "return " + value->to_string();
    } else {
        return "return";
    }
}

// IfStatement
void IfStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string IfStatement::to_string() const {
    std::ostringstream oss;
    oss << "if " << condition->to_string() << " " << then_branch->to_string();
    if (else_branch) {
        oss << " else " << else_branch->to_string();
    }
    return oss.str();
}

// WhileStatement
void WhileStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string WhileStatement::to_string() const {
    return "while " + condition->to_string() + " " + body->to_string();
}

// ForStatement
void ForStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ForStatement::to_string() const {
    return "for " + variable + " in " + start->to_string() + ".." + end->to_string() + " " + body->to_string();
}

// Program
void Program::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string Program::to_string() const {
    std::ostringstream oss;
    for (const auto& stmt : statements) {
        oss << stmt->to_string() << "\n";
    }
    return oss.str();
}

} // namespace myndra