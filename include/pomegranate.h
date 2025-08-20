#ifndef POMEGRANATE_H
#define POMEGRANATE_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <chrono>
#include <variant>

namespace pomegranate {

// Forward declarations
class Token;
class AST;
class SemanticAnalyzer;
class CodeGenerator;
class Runtime;
class ReactiveEngine;
class TemporalSystem;
class CapsuleManager;
class ContextManager;
class CapabilitySystem;
class DIDResolver;
class DSLEngine;
class PackageManager;

// Core language types
using Hash = std::string;
using ContextType = std::string;
using CapabilitySet = std::vector<std::string>;
using TimePoint = std::chrono::steady_clock::time_point;
using Duration = std::chrono::milliseconds;

// Language value types
struct Value {
    enum Type {
        NIL, BOOL, INT, FLOAT, STRING, FUNCTION, OBJECT,
        REACTIVE, TEMPORAL, CAPSULE, DID_IDENTITY
    };
    
    Type type;
    std::variant<
        std::nullptr_t,
        bool,
        int64_t,
        double,
        std::string,
        std::function<Value(std::vector<Value>)>,
        std::unordered_map<std::string, Value>
    > data;
    
    Value() : type(NIL), data(nullptr) {}
    explicit Value(bool b) : type(BOOL), data(b) {}
    explicit Value(int64_t i) : type(INT), data(i) {}
    explicit Value(double f) : type(FLOAT), data(f) {}
    explicit Value(const std::string& s) : type(STRING), data(s) {}
};

// Context-aware execution
struct ExecutionContext {
    ContextType type; // "dev", "prod", "test", "runtime"
    std::unordered_map<std::string, Value> variables;
    CapabilitySet capabilities;
    TimePoint timestamp;
};

// Reactive programming primitives
class Observable {
public:
    using Callback = std::function<void(const Value&)>;
    
    virtual ~Observable() = default;
    virtual void subscribe(Callback callback) = 0;
    virtual void unsubscribe(Callback callback) = 0;
    virtual void notify(const Value& value) = 0;
};

// Temporal types
class TemporalValue {
public:
    virtual ~TemporalValue() = default;
    virtual Value get_value_at(TimePoint time) const = 0;
    virtual void transition_to(const Value& target, Duration duration) = 0;
    virtual bool is_transitioning() const = 0;
};

// Live code capsules
class Capsule {
public:
    virtual ~Capsule() = default;
    virtual Value execute(const std::vector<Value>& args) = 0;
    virtual bool reload(const std::string& new_code) = 0;
    virtual std::string get_source() const = 0;
    virtual Hash get_hash() const = 0;
};

// Capability-based security
struct Capability {
    std::string name;
    std::vector<std::string> permissions;
    bool is_allowed(const std::string& operation) const;
};

// Decentralized identity
struct DIDDocument {
    std::string id;
    std::vector<std::string> public_keys;
    std::unordered_map<std::string, Value> proofs;
    bool verify_proof(const std::string& claim) const;
};

// Package system
struct Package {
    Hash hash;
    std::string name;
    std::vector<Hash> dependencies;
    CapabilitySet required_capabilities;
    std::unordered_map<std::string, std::string> metadata;
};

// Semantic tags for navigation
struct SemanticTag {
    std::string name;
    size_t line;
    size_t column;
    std::string description;
};

// Error handling and fallback strategies
struct FallbackStrategy {
    enum Type { RETRY, DEFAULT_VALUE, ALTERNATIVE_FUNCTION, IGNORE };
    Type type;
    int retry_count = 0;
    Value default_value;
    std::function<Value(const std::vector<Value>&)> alternative;
};

// Execution model annotations
enum class ExecutionModel {
    SYNC,
    ASYNC,
    PARALLEL,
    REACTIVE,
    TEMPORAL
};

// DSL block
struct DSLBlock {
    std::string language; // "shader", "query", "markup", etc.
    std::string code;
    std::unordered_map<std::string, Value> bindings;
};

// Main compiler interface
class Compiler {
public:
    struct Options {
        ContextType target_context = "dev";
        bool enable_live_reload = true;
        bool enable_reactive = true;
        bool enable_temporal = true;
        bool enable_did = true;
        std::vector<std::string> capability_whitelist;
    };
    
    explicit Compiler(const Options& opts = {});
    ~Compiler();
    
    // Core compilation pipeline
    bool compile_file(const std::string& filename);
    bool compile_string(const std::string& source);
    
    // Runtime execution
    Value execute();
    Value execute_capsule(const std::string& name, const std::vector<Value>& args);
    
    // Live features
    bool reload_capsule(const std::string& name, const std::string& new_code);
    bool update_context(const ExecutionContext& new_context);
    
    // Package management
    bool install_package(const Hash& package_hash);
    bool import_module(const std::string& module_name, const CapabilitySet& capabilities);
    
    // Reactive programming
    std::shared_ptr<Observable> create_observable(const Value& initial_value);
    bool bind_reactive(const std::string& var_name, std::shared_ptr<Observable> observable);
    
    // Error handling
    void set_global_fallback(const FallbackStrategy& strategy);
    std::vector<std::string> get_errors() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> pimpl;
};

// Utility functions
namespace utils {
    Hash calculate_hash(const std::string& content);
    std::string format_error(const std::string& message, size_t line, size_t column);
    bool is_valid_did(const std::string& did);
    ExecutionContext get_current_context();
    std::vector<SemanticTag> extract_semantic_tags(const std::string& source);
}

} // namespace pomegranate

#endif // POMEGRANATE_H
