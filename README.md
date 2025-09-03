# Myndra Programming Language

> **Status: Early Development** 🚧  
> A revolutionary programming language designed for the modern era - featuring context-aware syntax, live code capabilities, and modern programming paradigms.

## 🎯 What is Myndra?

Myndra is a next-generation programming language that adapts to your development context and provides powerful features for modern software development. Unlike traditional languages, Myndra understands *when* and *where* your code is running and can behave accordingly.

### Key Innovations

- **🎯 Context-Aware Programming**: Code that adapts to dev/prod/test environments
- **🔄 Live Code Capsules**: Hot-reload functions while your program runs
- **⚡ Built-in Reactive Programming**: First-class reactive data flows
- **⏰ Temporal Types**: Variables that change over time with built-in animations

---

## 🚀 Quick Start

### Installation

1. **Clone the repository**:
```bash
git clone https://github.com/yourusername/myndra.git
cd myndra
```

2. **Build the compiler**:
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

3. **Run your first program**:
```bash
# Create hello.myn
echo 'fn main() { print("Hello, Myndra!") }' > hello.myn

# Compile and run
./myndra hello.myn --run
```

### Interactive REPL

Try Myndra interactively:
```bash
./myndra --interactive
```

---

## 🌟 Core Language Features

### Context-Aware Programming
Write code that automatically adapts to its environment:

```myndra
fn main() {
    log("Starting application") if context == "dev"
    
    let config = if context == "prod" {
        load_prod_config()
    } else {
        load_dev_config()
    }
    
    run_app(config)
}
```

### Functions and Control Flow
Clean, expressive syntax for common programming constructs:

```myndra
fn fibonacci(n: int) -> int {
    if n <= 1 {
        return n
    }
    return fibonacci(n - 1) + fibonacci(n - 2)
}

fn main() {
    for i in 0..10 {
        print("fib({}) = {}", i, fibonacci(i))
    }
}
```

### Modern Type System
Strong typing with inference where it helps:

```myndra
fn process_data(items: Vec<string>) -> Vec<int> {
    return items
        .filter(|s| s.len() > 0)
        .map(|s| s.parse::<int>().unwrap_or(0))
        .collect()
}
```

---

## 📚 Documentation

| Document | Description |
|----------|-------------|
| [Core Development Plan](CORE_DEVELOPMENT_PLAN.md) | Current implementation roadmap |
| [Programming Guide](docs/programming_guide.md) | Complete language tutorial |
| [Language Reference](docs/language_reference.md) | Detailed language specification |
| [Quick Start Guide](docs/quick_start.md) | Getting started tutorial |
| [Examples](examples/) | Sample programs and tutorials |

---

## 🛠️ Development Status

### ✅ Currently Working
- **Lexical Analysis**: Tokenization and basic parsing
- **Build System**: CMake-based compilation
- **Project Structure**: Modular, extensible architecture
- **REPL**: Interactive development environment
- **Examples**: Sample programs and documentation

### 🚧 In Development (Core Phase)
- **Parser & AST**: Complete syntax tree generation
- **Type System**: Static type checking and inference  
- **Interpreter**: Tree-walking execution engine
- **Context System**: Environment-aware compilation
- **Standard Library**: Core functions and data types

### 📋 Planned Features
- **Live Code Capsules**: Hot-reloading functions
- **Reactive Programming**: Observable data flows
- **Temporal Types**: Time-varying values and animations
- **Advanced Type Features**: Generics, traits, pattern matching
- **Development Tools**: Debugger, profiler, language server

---

## 🎮 Try It Out

### Hello World
```myndra
fn main() {
    print("Hello, World!")
    log("This only shows in development!") if context == "dev"
}
```

### Calculator Example
```myndra
fn calculate(operation: string, a: float, b: float) -> float {
    match operation {
        "add" => a + b,
        "sub" => a - b,
        "mul" => a * b,
        "div" => if b != 0.0 { a / b } else { 0.0 },
        _ => 0.0
    }
}

fn main() {
    let result = calculate("add", 10.5, 5.2)
    print("Result: {}", result)
}
```

### Context-Aware Configuration
```myndra
fn get_database_url() -> string {
    return if context == "prod" {
        "postgres://prod-server/mydb"
    } else if context == "test" {
        "postgres://test-server/mydb_test" 
    } else {
        "sqlite://dev.db"
    }
}
```

---

## 🏗️ Project Structure

```
myndra/
├── 📁 src/
│   ├── 📁 lexer/          # Tokenization and lexical analysis
│   ├── 📁 parser/         # Syntax analysis (planned)
│   ├── 📁 semantics/      # Type checking (planned)
│   ├── 📁 runtime/        # Execution engine (planned)
│   └── 📄 main.cpp        # Compiler entry point
├── 📁 include/
│   └── 📄 myndra.h        # Main header file
├── 📁 tests/              # Unit and integration tests
├── 📁 examples/           # Sample programs (.myn files)
├── 📁 docs/               # Documentation and tutorials
├── 📄 CMakeLists.txt      # Build configuration
├── 📄 ROADMAP.md          # Long-term development plan
└── 📄 CORE_DEVELOPMENT_PLAN.md  # Current implementation focus
```

---

## 🤝 Contributing

We welcome contributions! Here's how you can help:

### For Developers
- **🐛 Bug Reports**: Found something broken? [Open an issue](issues)
- **💡 Feature Ideas**: Have a cool idea? [Start a discussion](discussions)
- **🔧 Code Contributions**: Check our [Core Development Plan](CORE_DEVELOPMENT_PLAN.md)
- **📝 Documentation**: Help improve our guides and examples

### For Language Enthusiasts  
- **🧪 Testing**: Try Myndra and report your experience
- **📖 Documentation**: Help us write better tutorials
- **🎯 Feedback**: Tell us what you love and what could be better

### Current Priority Tasks
1. **Parser Implementation**: Build the AST generation system
2. **Type System**: Implement type checking and inference
3. **Interpreter**: Create the execution engine
4. **Standard Library**: Build core functions and types
5. **Context System**: Implement environment-aware features

---

## 🎯 Design Philosophy

### Simplicity with Power
Myndra should be easy to learn but powerful enough for complex applications. We prioritize:
- **Readable code** over clever syntax
- **Clear error messages** over cryptic compiler output
- **Practical features** over academic purity
- **Developer productivity** over minimal language design

### Context-First Development
Traditional languages treat deployment environments as an afterthought. Myndra makes context awareness a first-class feature, enabling:
- **Automatic behavior adaptation** for different environments
- **Built-in debugging and logging** that knows when to activate
- **Configuration management** that's part of the language
- **Testing support** that's always available

---

## 📊 Comparison

| Feature | Myndra | Rust | Python | JavaScript |
|---------|--------|------|--------|------------|
| **Context Awareness** | ✅ Built-in | ❌ Manual | ❌ Manual | ❌ Manual |
| **Live Code Reloading** | 🚧 Planned | ❌ No | ⚠️ Limited | ⚠️ Limited |
| **Memory Safety** | 🚧 Planned | ✅ Yes | ✅ GC | ✅ GC |
| **Static Typing** | 🚧 Planned | ✅ Yes | ❌ Optional | ❌ Optional |
| **Performance** | 🚧 TBD | ✅ High | ⚠️ Medium | ⚠️ Medium |
| **Learning Curve** | 🎯 Gentle | ⚠️ Steep | ✅ Easy | ✅ Easy |

---

## 📈 Roadmap

### Current Focus (2024)
- ✅ Project setup and lexer
- 🚧 Parser and AST generation
- 📋 Type system implementation
- 📋 Basic interpreter

### Near Term (2025)
- Context-aware programming
- Standard library
- Development tools
- Live code capabilities

### Long Term (2026+)
- Reactive programming
- Temporal types
- Advanced tooling
- Ecosystem growth

See our detailed [Development Roadmap](ROADMAP.md) for the complete plan.

---

## 💬 Community

- **📧 Discussions**: [GitHub Discussions](discussions)
- **🐛 Issues**: [GitHub Issues](issues)
- **📖 Documentation**: [docs/](docs/)
- **💼 Professional**: [LinkedIn Group](#) (coming soon)

---

## 📜 License

MIT License - We believe in open source and collaborative development.

See [LICENSE](LICENSE) for full details.

---

## 🙏 Acknowledgments

Myndra is inspired by the best ideas from many languages:
- **Rust**: Memory safety and type system design
- **JavaScript**: Dynamic behavior and ecosystem approach
- **Python**: Developer friendliness and clear syntax
- **Erlang/Elixir**: Actor model and fault tolerance
- **ReactiveX**: Reactive programming patterns

---

*Ready to shape the future of programming? [Get involved](CONTRIBUTING.md) today!* 🚀