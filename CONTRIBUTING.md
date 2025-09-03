# Contributing to Myndra

Thank you for your interest in contributing to Myndra! This document provides guidelines and information for contributors.

---

## 🎯 Quick Start for Contributors

### 1. Set Up Development Environment

```bash
# Clone the repository
git clone https://github.com/yourusername/myndra.git
cd myndra

# Build the project
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug

# Run tests
ctest
```

### 2. Understanding the Codebase

| Directory | Purpose |
|-----------|---------|
| `src/` | Core compiler implementation |
| `src/lexer/` | Tokenization and lexical analysis |
| `include/` | Public header files |
| `tests/` | Unit and integration tests |
| `examples/` | Sample Myndra programs |
| `docs/` | Documentation and guides |

### 3. Current Development Focus

See our [Core Development Plan](CORE_DEVELOPMENT_PLAN.md) for the current implementation priorities:

1. **Parser & AST Implementation** (Critical)
2. **Type System & Semantic Analysis** (Critical) 
3. **Code Generation & Execution** (Critical)
4. **Context-Aware Features** (High Priority)

---

## 🛠️ Types of Contributions

### Code Contributions

#### Core Language Implementation
- **Parser Development**: Implement AST node generation
- **Type System**: Add type checking and inference
- **Interpreter**: Build the execution engine
- **Standard Library**: Core functions and data types

#### Language Features
- **Context-Aware Programming**: Environment detection and adaptation
- **Error Handling**: Improve error messages and recovery
- **REPL Enhancements**: Interactive development improvements

#### Testing & Quality
- **Unit Tests**: Add tests for new functionality
- **Integration Tests**: End-to-end testing scenarios
- **Performance Testing**: Benchmark and optimize
- **Documentation Tests**: Ensure examples work

### Documentation Contributions

- **Programming Guide**: Expand tutorials and examples
- **API Documentation**: Document functions and types
- **Architecture Documentation**: Explain design decisions
- **Tutorial Content**: Create learning materials

### Community Contributions

- **Bug Reports**: Detailed issue reporting
- **Feature Requests**: Well-researched suggestions
- **Community Support**: Help other users
- **Ecosystem Development**: Tools and integrations

---

## 📋 Development Process

### Getting Started with a Contribution

1. **Check Existing Issues**: Look for open issues or create a new one
2. **Discuss Your Approach**: Comment on the issue with your implementation plan
3. **Fork and Branch**: Create a feature branch for your work
4. **Implement Changes**: Follow our coding standards
5. **Add Tests**: Ensure your changes are tested
6. **Update Documentation**: Keep docs current with changes
7. **Submit Pull Request**: Use our PR template

### Issue Labels

We use labels to categorize issues:

- `good first issue`: Great for new contributors
- `help wanted`: We need community assistance
- `bug`: Something isn't working correctly
- `enhancement`: New feature or improvement
- `documentation`: Improvements to docs
- `priority:critical`: Must be fixed for core functionality
- `priority:high`: Important but not blocking
- `priority:low`: Nice to have

---

## 💻 Coding Standards

### C++ Code Style

```cpp
// Use descriptive names
class TokenParser {
public:
    explicit TokenParser(const std::string& source);
    
    // Document public methods
    /// Parses the next token from the input stream
    /// @return The next token, or EOF token if at end
    Token parseNextToken();
    
private:
    std::string source_;
    size_t position_;
    
    // Private methods use camelCase
    bool isIdentifierChar(char c) const;
};

// Namespace usage
namespace myndra {
    // Implementation here
} // namespace myndra
```

### Code Organization

```cpp
// Header files (.h)
#ifndef MYNDRA_PARSER_H
#define MYNDRA_PARSER_H

namespace myndra {
    class Parser {
        // Public interface only
    };
} // namespace myndra

#endif // MYNDRA_PARSER_H

// Implementation files (.cpp)
#include "parser.h"
#include <iostream>

namespace myndra {
    // Implementation here
} // namespace myndra
```

### Error Handling

```cpp
// Use Result types for operations that can fail
enum class ParseError {
    UnexpectedToken,
    InvalidSyntax,
    UnexpectedEOF
};

class ParseResult {
    // Implementation of Result<T, ParseError>
};

// Provide detailed error messages
ParseResult Parser::parseExpression() {
    if (current_token_.type != TokenType::IDENTIFIER) {
        return ParseError::UnexpectedToken(
            "Expected identifier, got " + 
            tokenTypeToString(current_token_.type),
            current_token_.line,
            current_token_.column
        );
    }
    // ...
}
```

---

## 🧪 Testing Guidelines

### Writing Tests

```cpp
// Unit test example
#include "gtest/gtest.h"
#include "lexer.h"

namespace myndra {
namespace test {

class LexerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code
    }
};

TEST_F(LexerTest, TokenizesBasicIdentifiers) {
    Lexer lexer("let variable_name = 42");
    auto tokens = lexer.tokenize();
    
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::LET);
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].value, "variable_name");
    EXPECT_EQ(tokens[2].type, TokenType::ASSIGN);
    EXPECT_EQ(tokens[3].type, TokenType::INTEGER);
}

} // namespace test
} // namespace myndra
```

### Integration Tests

```cpp
// Test complete compilation and execution
TEST(CompilerIntegrationTest, BasicProgramExecution) {
    std::string source = R"(
        fn main() {
            let result = 2 + 3
            print(result)
        }
    )";
    
    Compiler compiler;
    ASSERT_TRUE(compiler.compile_string(source));
    
    // Capture output and verify
    auto output = captureOutput([&]() {
        compiler.execute();
    });
    
    EXPECT_EQ(output, "5\n");
}
```

### Running Tests

```bash
# Build and run all tests
cd build
make test

# Run specific test suite
./test_lexer
./test_parser

# Run with verbose output
ctest --verbose
```

---

## 📚 Documentation Standards

### Code Documentation

```cpp
/// Brief description of the class
/// 
/// Detailed description explaining the purpose,
/// usage patterns, and any important considerations.
///
/// @example
/// ```cpp
/// Parser parser("let x = 42");
/// auto ast = parser.parseProgram();
/// ```
class Parser {
public:
    /// Parses a complete program
    /// 
    /// @param source The source code to parse
    /// @return AST representing the program structure
    /// @throws ParseError if syntax is invalid
    std::unique_ptr<ProgramNode> parseProgram(const std::string& source);
};
```

### Documentation Updates

When making changes, update:

1. **API Documentation**: Function/class descriptions
2. **User Guide**: If user-facing features change
3. **Examples**: Keep examples working and current
4. **README**: Update status and feature lists
5. **CHANGELOG**: Document breaking changes

---

## 🔄 Pull Request Process

### Before Submitting

- [ ] Code follows style guidelines
- [ ] All tests pass
- [ ] New functionality includes tests
- [ ] Documentation is updated
- [ ] Commit messages are clear
- [ ] Branch is up to date with main

### PR Template

```markdown
## Description
Brief description of changes and their purpose.

## Type of Change
- [ ] Bug fix
- [ ] New feature  
- [ ] Breaking change
- [ ] Documentation update

## Testing
- [ ] Unit tests added/updated
- [ ] Integration tests pass
- [ ] Manual testing completed

## Checklist
- [ ] Code follows style guidelines
- [ ] Self-review completed
- [ ] Documentation updated
- [ ] No new warnings introduced
```

### Review Process

1. **Automated Checks**: CI must pass
2. **Code Review**: At least one maintainer approval
3. **Testing**: All tests must pass
4. **Documentation**: Updates reviewed
5. **Merge**: Squash and merge preferred

---

## 🏆 Recognition

### Contributors

We recognize contributors in several ways:

- **Commit Attribution**: Your commits preserve your authorship
- **CONTRIBUTORS.md**: Listed as a project contributor  
- **Release Notes**: Significant contributions highlighted
- **Community Recognition**: Mentioned in announcements

### Becoming a Maintainer

Regular contributors may be invited to become maintainers with:

- **Commit Access**: Direct push to repository
- **Review Rights**: Can approve pull requests
- **Issue Triage**: Help manage the issue tracker
- **Release Process**: Participate in releases

---

## 🤝 Community Guidelines

### Code of Conduct

We are committed to providing a welcoming and inclusive environment:

- **Be Respectful**: Treat all participants with dignity
- **Be Constructive**: Focus on improving the project
- **Be Collaborative**: Work together toward common goals
- **Be Patient**: Remember everyone has different experience levels

### Communication

- **Issues**: Technical discussions and bug reports
- **Pull Requests**: Code-specific conversations
- **Discussions**: General questions and ideas
- **Email**: Private concerns to maintainers

### Getting Help

- **Documentation**: Start with our guides
- **Examples**: Check existing code samples
- **Issues**: Search before creating new ones
- **Discussions**: Ask questions in the community

---

## 📈 Project Roadmap

### Current Phase: Core Language (2024)
Focus on fundamental language features:

- Parser and AST generation
- Type system implementation
- Basic interpreter
- Context-aware features

### Next Phase: Advanced Features (2025)
Build on the foundation:

- Live code capabilities
- Reactive programming
- Enhanced tooling
- Standard library expansion

### Future: Ecosystem (2026+)
Grow the community:

- Package management
- IDE integrations
- Performance optimization
- Specialized domains

---

## 📞 Contact

- **General Questions**: [GitHub Discussions](discussions)
- **Bug Reports**: [GitHub Issues](issues)
- **Security Issues**: security@myndra-lang.org
- **Maintainers**: maintainers@myndra-lang.org

---

Thank you for contributing to Myndra! Together, we're building the future of programming. 🚀