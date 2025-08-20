#include "pomegranate.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void print_usage(const char* program_name) {
    std::cout << "Pomegranate Programming Language v1.0.0\n";
    std::cout << "Usage: " << program_name << " [options] <file>\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help              Show this help message\n";
    std::cout << "  -v, --version           Show version information\n";
    std::cout << "  -c, --context <type>    Set execution context (dev|prod|test)\n";
    std::cout << "  -i, --interactive       Start interactive REPL\n";
    std::cout << "  -r, --run               Run the program immediately\n";
    std::cout << "  --no-live-reload        Disable live code reloading\n";
    std::cout << "  --no-reactive           Disable reactive programming\n";
    std::cout << "  --no-temporal           Disable temporal types\n";
    std::cout << "  --no-did                Disable DID integration\n";
    std::cout << "  --capability <cap>      Add capability to whitelist\n";
    std::cout << "\nFeatures:\n";
    std::cout << "  • Context-aware syntax\n";
    std::cout << "  • Live code capsules\n";
    std::cout << "  • Reactive programming\n";
    std::cout << "  • Temporal types\n";
    std::cout << "  • Capability-based security\n";
    std::cout << "  • Decentralized identity\n";
    std::cout << "  • Inline DSL blocks\n";
    std::cout << "  • Self-healing error handling\n";
    std::cout << "  • Semantic code navigation\n";
    std::cout << "  • Hash-based package management\n";
}

void print_version() {
    std::cout << "Pomegranate Programming Language\n";
    std::cout << "Version: 1.0.0\n";
    std::cout << "Built with: C++20, LLVM\n";
    std::cout << "Features: All advanced features enabled\n";
}

void start_repl(pomegranate::Compiler& compiler) {
    std::cout << "Pomegranate Interactive REPL\n";
    std::cout << "Type 'exit' to quit, 'help' for commands\n\n";
    
    std::string line;
    while (true) {
        std::cout << "pom> ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        
        if (line == "exit" || line == "quit") {
            break;
        }
        
        if (line == "help") {
            std::cout << "REPL Commands:\n";
            std::cout << "  help                    Show this help\n";
            std::cout << "  exit/quit               Exit REPL\n";
            std::cout << "  context <type>          Change context (dev|prod|test)\n";
            std::cout << "  reload <capsule>        Reload a capsule\n";
            std::cout << "  packages                List installed packages\n";
            std::cout << "  capabilities            Show current capabilities\n";
            std::cout << "  tags                    Show semantic tags\n";
            continue;
        }
        
        if (line.empty()) {
            continue;
        }
        
        // Handle REPL commands
        if (line.substr(0, 7) == "context") {
            std::string context_type = line.substr(8);
            pomegranate::ExecutionContext new_context;
            new_context.type = context_type;
            new_context.timestamp = std::chrono::steady_clock::now();
            
            if (compiler.update_context(new_context)) {
                std::cout << "Context changed to: " << context_type << "\n";
            } else {
                std::cout << "Failed to change context\n";
            }
            continue;
        }
        
        // Compile and execute the line
        if (compiler.compile_string(line)) {
            try {
                auto result = compiler.execute();
                std::cout << "=> "; // Print result representation
                // TODO: Implement proper value printing
                std::cout << "\n";
            } catch (const std::exception& e) {
                std::cout << "Runtime error: " << e.what() << "\n";
            }
        } else {
            auto errors = compiler.get_errors();
            for (const auto& error : errors) {
                std::cout << "Error: " << error << "\n";
            }
        }
    }
}

int main(int argc, char* argv[]) {
    pomegranate::Compiler::Options options;
    std::string filename;
    bool interactive = false;
    bool run_immediately = false;
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            print_version();
            return 0;
        } else if (arg == "-i" || arg == "--interactive") {
            interactive = true;
        } else if (arg == "-r" || arg == "--run") {
            run_immediately = true;
        } else if (arg == "-c" || arg == "--context") {
            if (i + 1 < argc) {
                options.target_context = argv[++i];
            } else {
                std::cerr << "Error: --context requires an argument\n";
                return 1;
            }
        } else if (arg == "--no-live-reload") {
            options.enable_live_reload = false;
        } else if (arg == "--no-reactive") {
            options.enable_reactive = false;
        } else if (arg == "--no-temporal") {
            options.enable_temporal = false;
        } else if (arg == "--no-did") {
            options.enable_did = false;
        } else if (arg == "--capability") {
            if (i + 1 < argc) {
                options.capability_whitelist.push_back(argv[++i]);
            } else {
                std::cerr << "Error: --capability requires an argument\n";
                return 1;
            }
        } else if (arg[0] != '-') {
            filename = arg;
        } else {
            std::cerr << "Error: Unknown option " << arg << "\n";
            return 1;
        }
    }
    
    try {
        pomegranate::Compiler compiler(options);
        
        if (interactive) {
            start_repl(compiler);
            return 0;
        }
        
        if (filename.empty()) {
            std::cerr << "Error: No input file specified\n";
            print_usage(argv[0]);
            return 1;
        }
        
        // Check if file exists
        std::ifstream file(filename);
        if (!file.good()) {
            std::cerr << "Error: Cannot open file '" << filename << "'\n";
            return 1;
        }
        file.close();
        
        std::cout << "Compiling " << filename << " with context '" 
                  << options.target_context << "'...\n";
        
        if (!compiler.compile_file(filename)) {
            auto errors = compiler.get_errors();
            std::cerr << "Compilation failed:\n";
            for (const auto& error : errors) {
                std::cerr << "  " << error << "\n";
            }
            return 1;
        }
        
        std::cout << "Compilation successful!\n";
        
        if (run_immediately) {
            std::cout << "Executing...\n";
            try {
                auto result = compiler.execute();
                std::cout << "Execution completed successfully.\n";
            } catch (const std::exception& e) {
                std::cerr << "Runtime error: " << e.what() << "\n";
                return 1;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
