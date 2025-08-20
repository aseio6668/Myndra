#include "../include/pomegranate.h"
#include <iostream>
#include <cassert>

using namespace pomegranate;

void test_compiler_creation() {
    std::cout << "Testing compiler creation..." << std::endl;
    
    Compiler::Options options;
    options.target_context = "test";
    options.enable_live_reload = true;
    options.enable_reactive = true;
    
    Compiler compiler(options);
    
    std::cout << "✓ Compiler creation test passed" << std::endl;
}

void test_simple_compilation() {
    std::cout << "Testing simple compilation..." << std::endl;
    
    std::string simple_code = R"(
        let x = 42
        let y = 3.14
        let message = "Hello, World!"
    )";
    
    Compiler compiler;
    bool result = compiler.compile_string(simple_code);
    
    if (!result) {
        auto errors = compiler.get_errors();
        for (const auto& error : errors) {
            std::cout << "Error: " << error << std::endl;
        }
    }
    
    assert(result);
    std::cout << "✓ Simple compilation test passed" << std::endl;
}

void test_function_compilation() {
    std::cout << "Testing function compilation..." << std::endl;
    
    std::string function_code = R"(
        fn greet(name: string) -> string {
            return "Hello, " + name + "!"
        }
        
        fn main() {
            let greeting = greet("Pomegranate")
            print(greeting)
        }
    )";
    
    Compiler compiler;
    bool result = compiler.compile_string(function_code);
    
    if (!result) {
        auto errors = compiler.get_errors();
        for (const auto& error : errors) {
            std::cout << "Error: " << error << std::endl;
        }
    }
    
    assert(result);
    std::cout << "✓ Function compilation test passed" << std::endl;
}

void test_reactive_compilation() {
    std::cout << "Testing reactive compilation..." << std::endl;
    
    std::string reactive_code = R"(
        @reactive
        fn update_ui() {
            let count = observable<int>(0)
            
            count.subscribe { value ->
                print("Count updated: " + value.to_string())
            }
            
            return count
        }
    )";
    
    Compiler compiler;
    bool result = compiler.compile_string(reactive_code);
    
    if (!result) {
        auto errors = compiler.get_errors();
        for (const auto& error : errors) {
            std::cout << "Error: " << error << std::endl;
        }
    }
    
    assert(result);
    std::cout << "✓ Reactive compilation test passed" << std::endl;
}

void test_temporal_compilation() {
    std::cout << "Testing temporal compilation..." << std::endl;
    
    std::string temporal_code = R"(
        fn animate() {
            let x: evolving<float> = 0.0
            let y: evolving<float> = 0.0
            
            x -> 100.0 over 2s
            y -> 50.0 over 1s
            
            return { x, y }
        }
    )";
    
    Compiler compiler;
    bool result = compiler.compile_string(temporal_code);
    
    if (!result) {
        auto errors = compiler.get_errors();
        for (const auto& error : errors) {
            std::cout << "Error: " << error << std::endl;
        }
    }
    
    assert(result);
    std::cout << "✓ Temporal compilation test passed" << std::endl;
}

void test_capsule_compilation() {
    std::cout << "Testing capsule compilation..." << std::endl;
    
    std::string capsule_code = R"(
        #tag:ui
        capsule button_component {
            fn render(text: string) -> Element {
                return ui::button(text)
            }
        }
        
        fn main() {
            let button = button_component.render("Click me!")
            ui::mount(button, "#app")
        }
    )";
    
    Compiler compiler;
    bool result = compiler.compile_string(capsule_code);
    
    if (!result) {
        auto errors = compiler.get_errors();
        for (const auto& error : errors) {
            std::cout << "Error: " << error << std::endl;
        }
    }
    
    assert(result);
    std::cout << "✓ Capsule compilation test passed" << std::endl;
}

void test_context_aware_compilation() {
    std::cout << "Testing context-aware compilation..." << std::endl;
    
    std::string context_code = R"(
        fn process_data(data: Data) {
            log("Processing data") if context == "dev"
            
            let result = transform(data)
            
            metrics::record("data_processed") if context == "prod"
            
            return result
        }
    )";
    
    Compiler compiler;
    bool result = compiler.compile_string(context_code);
    
    if (!result) {
        auto errors = compiler.get_errors();
        for (const auto& error : errors) {
            std::cout << "Error: " << error << std::endl;
        }
    }
    
    assert(result);
    std::cout << "✓ Context-aware compilation test passed" << std::endl;
}

void test_error_handling() {
    std::cout << "Testing error handling..." << std::endl;
    
    std::string invalid_code = R"(
        fn invalid_syntax( {
            let x = 
            return y
        }
    )";
    
    Compiler compiler;
    bool result = compiler.compile_string(invalid_code);
    
    // This should fail to compile
    assert(!result);
    
    auto errors = compiler.get_errors();
    assert(!errors.empty());
    
    std::cout << "✓ Error handling test passed (found " << errors.size() << " errors as expected)" << std::endl;
}

int main() {
    std::cout << "Running Pomegranate Compilation Tests..." << std::endl;
    std::cout << "=======================================" << std::endl;
    
    try {
        test_compiler_creation();
        test_simple_compilation();
        test_function_compilation();
        test_reactive_compilation();
        test_temporal_compilation();
        test_capsule_compilation();
        test_context_aware_compilation();
        test_error_handling();
        
        std::cout << std::endl;
        std::cout << "✓ All compilation tests passed!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Test failed: " << e.what() << std::endl;
        return 1;
    }
}
