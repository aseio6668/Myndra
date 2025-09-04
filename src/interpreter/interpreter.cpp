#include "interpreter.h"
#include <iostream>
#include <stdexcept>
#include <sstream>

namespace myndra {

// Environment implementation
Environment::Environment(std::shared_ptr<Environment> parent) : parent_(parent) {}

void Environment::define(const std::string& name, const RuntimeValue& value) {
    variables_[name] = value;
}

RuntimeValue Environment::get(const std::string& name) const {
    auto it = variables_.find(name);
    if (it != variables_.end()) {
        return it->second;
    }
    
    if (parent_) {
        return parent_->get(name);
    }
    
    throw std::runtime_error("Undefined variable '" + name + "'");
}

void Environment::assign(const std::string& name, const RuntimeValue& value) {
    auto it = variables_.find(name);
    if (it != variables_.end()) {
        it->second = value;
        return;
    }
    
    if (parent_) {
        parent_->assign(name, value);
        return;
    }
    
    throw std::runtime_error("Undefined variable '" + name + "'");
}

// Interpreter implementation
Interpreter::Interpreter() : environment_(std::make_shared<Environment>()) {
    setupBuiltins();
}

void Interpreter::execute(Program& program) {
    program.accept(*this);
}

void Interpreter::visit(IntegerLiteral& node) {
    lastValue_ = node.value;
}

void Interpreter::visit(FloatLiteral& node) {
    lastValue_ = node.value;
}

void Interpreter::visit(StringLiteral& node) {
    lastValue_ = node.value;
}

void Interpreter::visit(BooleanLiteral& node) {
    lastValue_ = node.value;
}

void Interpreter::visit(Identifier& node) {
    lastValue_ = environment_->get(node.name);
}

void Interpreter::visit(BinaryExpression& node) {
    // Evaluate left operand
    node.left->accept(*this);
    RuntimeValue left = lastValue_;
    
    // Evaluate right operand  
    node.right->accept(*this);
    RuntimeValue right = lastValue_;
    
    // Perform operation based on operator
    switch (node.op) {
        case BinaryOperator::Add: {
            if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
                lastValue_ = std::get<int64_t>(left) + std::get<int64_t>(right);
            } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                lastValue_ = std::get<double>(left) + std::get<double>(right);
            } else if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                lastValue_ = std::get<std::string>(left) + std::get<std::string>(right);
            } else {
                throw std::runtime_error("Invalid operands for addition");
            }
            break;
        }
        case BinaryOperator::Sub: {
            if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
                lastValue_ = std::get<int64_t>(left) - std::get<int64_t>(right);
            } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                lastValue_ = std::get<double>(left) - std::get<double>(right);
            } else {
                throw std::runtime_error("Invalid operands for subtraction");
            }
            break;
        }
        case BinaryOperator::Mul: {
            if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
                lastValue_ = std::get<int64_t>(left) * std::get<int64_t>(right);
            } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                lastValue_ = std::get<double>(left) * std::get<double>(right);
            } else {
                throw std::runtime_error("Invalid operands for multiplication");
            }
            break;
        }
        case BinaryOperator::Div: {
            if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
                int64_t r = std::get<int64_t>(right);
                if (r == 0) throw std::runtime_error("Division by zero");
                lastValue_ = std::get<int64_t>(left) / r;
            } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                double r = std::get<double>(right);
                if (r == 0.0) throw std::runtime_error("Division by zero");
                lastValue_ = std::get<double>(left) / r;
            } else {
                throw std::runtime_error("Invalid operands for division");
            }
            break;
        }
        case BinaryOperator::Eq: {
            lastValue_ = (left == right);
            break;
        }
        case BinaryOperator::Ne: {
            lastValue_ = (left != right);
            break;
        }
        case BinaryOperator::Lt: {
            if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
                lastValue_ = std::get<int64_t>(left) < std::get<int64_t>(right);
            } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                lastValue_ = std::get<double>(left) < std::get<double>(right);
            } else {
                throw std::runtime_error("Invalid operands for comparison");
            }
            break;
        }
        case BinaryOperator::Gt: {
            if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
                lastValue_ = std::get<int64_t>(left) > std::get<int64_t>(right);
            } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                lastValue_ = std::get<double>(left) > std::get<double>(right);
            } else {
                throw std::runtime_error("Invalid operands for comparison");
            }
            break;
        }
        case BinaryOperator::Le: {
            if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
                lastValue_ = std::get<int64_t>(left) <= std::get<int64_t>(right);
            } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                lastValue_ = std::get<double>(left) <= std::get<double>(right);
            } else {
                throw std::runtime_error("Invalid operands for comparison");
            }
            break;
        }
        case BinaryOperator::Ge: {
            if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
                lastValue_ = std::get<int64_t>(left) >= std::get<int64_t>(right);
            } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                lastValue_ = std::get<double>(left) >= std::get<double>(right);
            } else {
                throw std::runtime_error("Invalid operands for comparison");
            }
            break;
        }
        case BinaryOperator::And: {
            lastValue_ = isTruthy(left) && isTruthy(right);
            break;
        }
        case BinaryOperator::Or: {
            lastValue_ = isTruthy(left) || isTruthy(right);
            break;
        }
        default:
            throw std::runtime_error("Unsupported binary operator");
    }
}

void Interpreter::visit(UnaryExpression& node) {
    node.operand->accept(*this);
    RuntimeValue operand = lastValue_;
    
    switch (node.op) {
        case UnaryOperator::Neg: {
            if (std::holds_alternative<int64_t>(operand)) {
                lastValue_ = -std::get<int64_t>(operand);
            } else if (std::holds_alternative<double>(operand)) {
                lastValue_ = -std::get<double>(operand);
            } else {
                throw std::runtime_error("Invalid operand for negation");
            }
            break;
        }
        case UnaryOperator::Not: {
            lastValue_ = !isTruthy(operand);
            break;
        }
        default:
            throw std::runtime_error("Unsupported unary operator");
    }
}

void Interpreter::visit(FunctionCall& node) {
    // Extract function name from the function expression (should be an Identifier)
    auto* identifier = dynamic_cast<Identifier*>(node.function.get());
    if (!identifier) {
        throw std::runtime_error("Function calls with complex expressions not yet supported");
    }
    
    std::string functionName = identifier->name;
    
    // Handle built-in print function
    if (functionName == "print") {
        std::vector<RuntimeValue> args;
        for (auto& arg : node.arguments) {
            arg->accept(*this);
            args.push_back(lastValue_);
        }
        lastValue_ = callPrint(args);
        return;
    }
    
    // Handle built-in input function
    if (functionName == "input") {
        std::vector<RuntimeValue> args;
        for (auto& arg : node.arguments) {
            arg->accept(*this);
            args.push_back(lastValue_);
        }
        lastValue_ = callInput(args);
        return;
    }
    
    // Handle built-in length function
    if (functionName == "length") {
        std::vector<RuntimeValue> args;
        for (auto& arg : node.arguments) {
            arg->accept(*this);
            args.push_back(lastValue_);
        }
        lastValue_ = callLength(args);
        return;
    }
    
    // Handle built-in substring function
    if (functionName == "substring") {
        std::vector<RuntimeValue> args;
        for (auto& arg : node.arguments) {
            arg->accept(*this);
            args.push_back(lastValue_);
        }
        lastValue_ = callSubstring(args);
        return;
    }
    
    // TODO: Handle user-defined functions
    throw std::runtime_error("Function '" + functionName + "' is not defined");
}

void Interpreter::visit(ArrayAccess& node) {
    // TODO: Implement array access
    throw std::runtime_error("Array access not yet implemented");
}

void Interpreter::visit(MemberAccess& node) {
    // TODO: Implement member access
    throw std::runtime_error("Member access not yet implemented");
}

void Interpreter::visit(ContextConditional& node) {
    // TODO: Implement context conditionals
    throw std::runtime_error("Context conditionals not yet implemented");
}

void Interpreter::visit(ExpressionStatement& node) {
    node.expression->accept(*this);
}

void Interpreter::visit(VariableDeclaration& node) {
    RuntimeValue value;
    if (node.initializer) {
        node.initializer->accept(*this);
        value = lastValue_;
    } else {
        // Default initialization based on type or use null/default value
        value = int64_t(0); // Default to 0 for now
    }
    
    environment_->define(node.name, value);
}

void Interpreter::visit(Block& node) {
    // Create new environment for block scope
    auto previous = environment_;
    environment_ = std::make_shared<Environment>(environment_);
    
    try {
        for (auto& stmt : node.statements) {
            stmt->accept(*this);
        }
    } catch (...) {
        environment_ = previous; // Restore environment
        throw;
    }
    
    environment_ = previous; // Restore environment
}

void Interpreter::visit(FunctionDefinition& node) {
    // TODO: Implement function definitions
    // For now, just store the function in the environment
    // We need to implement callable values for full function support
    std::cout << "Function '" << node.name << "' defined (not yet executable)" << std::endl;
}

void Interpreter::visit(ReturnStatement& node) {
    // TODO: Implement return statements with exception-based control flow
    throw std::runtime_error("Return statements not yet implemented");
}

void Interpreter::visit(IfStatement& node) {
    node.condition->accept(*this);
    if (isTruthy(lastValue_)) {
        node.then_branch->accept(*this);
    } else if (node.else_branch) {
        node.else_branch->accept(*this);
    }
}

void Interpreter::visit(WhileStatement& node) {
    while (true) {
        node.condition->accept(*this);
        if (!isTruthy(lastValue_)) {
            break;
        }
        node.body->accept(*this);
    }
}

void Interpreter::visit(ForStatement& node) {
    // TODO: Implement for loops
    throw std::runtime_error("For loops not yet implemented");
}

void Interpreter::visit(Program& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

std::string Interpreter::valueToString(const RuntimeValue& value) const {
    return std::visit([](const auto& v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, int64_t>) {
            return std::to_string(v);
        } else if constexpr (std::is_same_v<T, double>) {
            return std::to_string(v);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return v;
        } else if constexpr (std::is_same_v<T, bool>) {
            return v ? "true" : "false";
        }
        return "unknown";
    }, value);
}

bool Interpreter::isTruthy(const RuntimeValue& value) const {
    return std::visit([](const auto& v) -> bool {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, bool>) {
            return v;
        } else if constexpr (std::is_same_v<T, int64_t>) {
            return v != 0;
        } else if constexpr (std::is_same_v<T, double>) {
            return v != 0.0;
        } else if constexpr (std::is_same_v<T, std::string>) {
            return !v.empty();
        }
        return false;
    }, value);
}

void Interpreter::setupBuiltins() {
    // Built-in functions are handled directly in visit(FunctionCall&)
    // No need to store them in the environment for now
}

RuntimeValue Interpreter::callPrint(const std::vector<RuntimeValue>& args) {
    for (size_t i = 0; i < args.size(); ++i) {
        if (i > 0) std::cout << " ";
        std::cout << valueToString(args[i]);
    }
    std::cout << std::endl;
    return int64_t(0); // Return 0 as success indicator
}

RuntimeValue Interpreter::callInput(const std::vector<RuntimeValue>& args) {
    // Print prompt if provided
    if (!args.empty()) {
        std::cout << valueToString(args[0]);
    }
    
    std::string input;
    std::getline(std::cin, input);
    return input;
}

RuntimeValue Interpreter::callLength(const std::vector<RuntimeValue>& args) {
    if (args.size() != 1) {
        throw std::runtime_error("length() expects exactly 1 argument");
    }
    
    const auto& value = args[0];
    if (std::holds_alternative<std::string>(value)) {
        return static_cast<int64_t>(std::get<std::string>(value).length());
    } else {
        throw std::runtime_error("length() can only be called on strings");
    }
}

RuntimeValue Interpreter::callSubstring(const std::vector<RuntimeValue>& args) {
    if (args.size() < 2 || args.size() > 3) {
        throw std::runtime_error("substring() expects 2 or 3 arguments: substring(string, start, [length])");
    }
    
    if (!std::holds_alternative<std::string>(args[0])) {
        throw std::runtime_error("substring() first argument must be a string");
    }
    if (!std::holds_alternative<int64_t>(args[1])) {
        throw std::runtime_error("substring() second argument must be an integer");
    }
    
    const std::string& str = std::get<std::string>(args[0]);
    int64_t start = std::get<int64_t>(args[1]);
    
    if (start < 0 || start >= static_cast<int64_t>(str.length())) {
        return std::string(""); // Return empty string for out-of-bounds
    }
    
    if (args.size() == 3) {
        if (!std::holds_alternative<int64_t>(args[2])) {
            throw std::runtime_error("substring() third argument must be an integer");
        }
        int64_t length = std::get<int64_t>(args[2]);
        if (length < 0) {
            return std::string("");
        }
        return str.substr(static_cast<size_t>(start), static_cast<size_t>(length));
    } else {
        return str.substr(static_cast<size_t>(start));
    }
}

} // namespace myndra