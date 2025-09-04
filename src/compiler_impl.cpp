#include "../include/myndra.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "interpreter/interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace myndra {

// Implementation details
class Compiler::Impl {
public:
    Options options;
    std::vector<std::string> errors;
    std::string current_source;
    std::unique_ptr<Program> ast;
    std::unique_ptr<Interpreter> interpreter;
    
    explicit Impl(const Options& opts) : options(opts), interpreter(std::make_unique<Interpreter>()) {}
};

// Constructor
Compiler::Compiler(const Options& opts) : pimpl(std::make_unique<Impl>(opts)) {
    std::cout << "Myndra Compiler initialized with context: " 
              << opts.target_context << std::endl;
    
    if (opts.enable_live_reload) {
        std::cout << "✓ Live code reloading enabled" << std::endl;
    }
    if (opts.enable_reactive) {
        std::cout << "✓ Reactive programming enabled" << std::endl;
    }
    if (opts.enable_temporal) {
        std::cout << "✓ Temporal types enabled" << std::endl;
    }
    if (opts.enable_did) {
        std::cout << "✓ Decentralized identity enabled" << std::endl;
    }
}

// Destructor
Compiler::~Compiler() = default;

// Core compilation pipeline
bool Compiler::compile_file(const std::string& filename) {
    std::cout << "Compiling file: " << filename << std::endl;
    
    // TODO: Read file content
    std::ifstream file(filename);
    if (!file.good()) {
        pimpl->errors.push_back("Cannot open file: " + filename);
        return false;
    }
    
    std::string source((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    
    return compile_string(source);
}

bool Compiler::compile_string(const std::string& source) {
    pimpl->current_source = source;
    pimpl->errors.clear();
    
    std::cout << "Compiling source code..." << std::endl;
    
    // Lexical analysis
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    if (lexer.has_errors()) {
        for (const auto& error : lexer.get_errors()) {
            pimpl->errors.push_back("Lexer error: " + error);
        }
        return false;
    }
    
    std::cout << "✓ Lexical analysis completed (" << tokens.size() << " tokens)" << std::endl;
    
    // Parsing
    Parser parser(tokens);
    pimpl->ast = parser.parseProgram();
    
    if (parser.hasErrors()) {
        for (const auto& error : parser.getErrors()) {
            pimpl->errors.push_back("Parse error: " + error);
        }
        return false;
    }
    
    std::cout << "✓ Parsing completed (" << pimpl->ast->statements.size() << " statements)" << std::endl;
    
    // For now, print the AST for debugging
    if (pimpl->options.target_context == "dev") {
        std::cout << "AST:\n" << pimpl->ast->to_string() << std::endl;
    }
    
    // Execute the AST with the interpreter
    std::cout << "✓ Semantic analysis completed (stub)" << std::endl;
    std::cout << "✓ Executing..." << std::endl;
    
    try {
        pimpl->interpreter->execute(*pimpl->ast);
        std::cout << "✓ Execution completed" << std::endl;
    } catch (const std::exception& e) {
        pimpl->errors.push_back("Runtime error: " + std::string(e.what()));
        return false;
    }
    
    return true;
}

// Runtime execution
Value Compiler::execute() {
    std::cout << "Executing compiled code..." << std::endl;
    // TODO: Implement execution
    return Value(); // Return nil for now
}

Value Compiler::execute_capsule(const std::string& name, const std::vector<Value>& args) {
    std::cout << "Executing capsule: " << name << " with " << args.size() << " arguments" << std::endl;
    // TODO: Implement capsule execution
    return Value();
}

// Live features
bool Compiler::reload_capsule(const std::string& name, const std::string& new_code) {
    std::cout << "Reloading capsule: " << name << std::endl;
    // TODO: Implement live reloading
    return true;
}

bool Compiler::update_context(const ExecutionContext& new_context) {
    std::cout << "Updating execution context to: " << new_context.type << std::endl;
    // TODO: Implement context switching
    return true;
}

// Package management
bool Compiler::install_package(const Hash& package_hash) {
    std::cout << "Installing package: " << package_hash << std::endl;
    // TODO: Implement package installation
    return true;
}

bool Compiler::import_module(const std::string& module_name, const CapabilitySet& capabilities) {
    std::cout << "Importing module: " << module_name << " with capabilities: ";
    for (const auto& cap : capabilities) {
        std::cout << cap << " ";
    }
    std::cout << std::endl;
    // TODO: Implement module importing
    return true;
}

// Reactive programming
std::shared_ptr<Observable> Compiler::create_observable(const Value& initial_value) {
    std::cout << "Creating observable with initial value" << std::endl;
    // TODO: Implement observable creation
    return nullptr;
}

bool Compiler::bind_reactive(const std::string& var_name, std::shared_ptr<Observable> observable) {
    std::cout << "Binding reactive variable: " << var_name << std::endl;
    // TODO: Implement reactive binding
    return true;
}

// Error handling
void Compiler::set_global_fallback(const FallbackStrategy& strategy) {
    std::cout << "Setting global fallback strategy" << std::endl;
    // TODO: Implement fallback strategy
}

std::vector<std::string> Compiler::get_errors() const {
    return pimpl->errors;
}

// Capability implementation
bool Capability::is_allowed(const std::string& operation) const {
    return std::find(permissions.begin(), permissions.end(), operation) != permissions.end();
}

// DID implementation
bool DIDDocument::verify_proof(const std::string& claim) const {
    // TODO: Implement ZK proof verification
    return proofs.find(claim) != proofs.end();
}

// Utility functions
namespace utils {
    Hash calculate_hash(const std::string& content) {
        // Simple hash implementation for demo
        std::hash<std::string> hasher;
        return std::to_string(hasher(content));
    }
    
    std::string format_error(const std::string& message, size_t line, size_t column) {
        return "Line " + std::to_string(line) + ", Column " + std::to_string(column) + ": " + message;
    }
    
    bool is_valid_did(const std::string& did) {
        return did.substr(0, 4) == "did:";
    }
    
    ExecutionContext get_current_context() {
        ExecutionContext ctx;
        ctx.type = "dev"; // Default context
        ctx.timestamp = std::chrono::steady_clock::now();
        return ctx;
    }
    
    std::vector<SemanticTag> extract_semantic_tags(const std::string& source) {
        std::vector<SemanticTag> tags;
        // TODO: Implement semantic tag extraction
        return tags;
    }
}

} // namespace myndra
