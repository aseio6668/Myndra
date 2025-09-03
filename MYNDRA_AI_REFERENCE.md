# Myndra Language - AI Programming Reference

*Quick reference for AI assistants programming in Myndra*

## 🎯 Current Implementation Status

### ✅ **Working Features:**
- Lexical analysis (tokenization)
- Basic compiler infrastructure
- REPL framework
- Build system (CMake)

### ❌ **NOT YET IMPLEMENTED:**
- Parser (completely missing)
- AST generation (stubbed)
- Type system (stubbed) 
- Code execution (stubbed)
- Most language features (stubbed)

**⚠️ IMPORTANT**: As of now, you can write Myndra syntax but it won't execute. Focus on syntax examples and theoretical code until Phase 1 completion.

---

## 🔤 Myndra Syntax Quick Reference

### **Basic Structure**
```myndra
// Comments
fn main() {
    print("Hello, Myndra!")
}
```

### **Variables & Types**
```myndra
let name = "Alice"           // Immutable (preferred)
let mut counter = 0          // Mutable
let age: int = 25           // Explicit type
let pi: float = 3.14159     // Floating point
let is_valid: bool = true   // Boolean
```

### **Functions**
```myndra
fn greet(name: string) {
    print("Hello, " + name + "!")
}

fn add(a: int, b: int) -> int {
    return a + b
}

fn multiply(a: int, b: int) -> int = a * b  // Expression body
```

### **Control Flow**
```myndra
// Conditionals
if age >= 18 {
    print("Adult")
} else {
    print("Minor")
}

// Loops
for i in 0..10 {
    print("Number: {}", i)
}

while count < 5 {
    count = count + 1
}

// Pattern matching
match value {
    0 => print("zero"),
    1..10 => print("single digit"),
    _ => print("other")
}
```

### **Context-Aware Programming** (Myndra's unique feature)
```myndra
fn main() {
    log("Debug info") if context == "dev"
    
    let config = if context == "prod" {
        "production.toml"
    } else {
        "development.toml"
    }
}
```

### **Data Structures**
```myndra
// Arrays/Vectors
let numbers = [1, 2, 3, 4, 5]
let mut items = Vec::new()

// Structs
struct Person {
    name: string,
    age: int
}

let alice = Person { name: "Alice", age: 30 }

// Enums
enum Status {
    Pending,
    Complete,
    Failed
}
```

### **Error Handling**
```myndra
fn divide(a: float, b: float) -> Result<float, string> {
    if b == 0.0 {
        return Err("Division by zero")
    }
    return Ok(a / b)
}

match divide(10.0, 2.0) {
    Ok(result) => print("Result: {}", result),
    Err(error) => print("Error: {}", error)
}
```

---

## 🎯 AI Programming Guidelines

### **When Writing Myndra Code:**

1. **Focus on Syntax Examples**: Since execution isn't implemented yet
2. **Use Context-Aware Features**: This is Myndra's main differentiator
3. **Follow Rust-like Patterns**: But with Myndra's unique features
4. **Include Error Handling**: Use Result<T, E> and Option<T> patterns
5. **Document Context Usage**: Explain when code runs in dev/prod/test

### **Example AI Response Format:**
```myndra
// Context-aware logging example
fn process_data(items: Vec<string>) -> Result<Vec<ProcessedItem>, ProcessError> {
    log("Processing {} items", items.len()) if context == "dev"
    
    let mut results = Vec::new()
    
    for item in items {
        match validate_item(item) {
            Ok(validated) => {
                let processed = transform_item(validated)
                results.push(processed)
            },
            Err(e) => {
                log("Failed to process item: {}", e) if context != "prod"
                return Err(ProcessError::ValidationFailed(e))
            }
        }
    }
    
    log("Successfully processed {} items", results.len()) if context == "dev"
    return Ok(results)
}
```

### **Common AI Mistakes to Avoid:**
- ❌ Don't mix JavaScript/Python syntax
- ❌ Don't forget context-aware features when relevant
- ❌ Don't use features not in this reference
- ❌ Don't assume network/async features work yet
- ✅ Do use Rust-like syntax with Myndra additions
- ✅ Do emphasize context-aware programming
- ✅ Do use proper error handling patterns

### **File Extensions:**
- Source files: `.myn`
- Examples: `hello_world.myn`, `calculator.myn`

### **Current Working Examples:**
- See `/examples/*.myn` for complete working syntax examples
- All examples are syntactically correct but won't execute until parser is implemented

---

## 📁 Directory Structure for AI Reference

```
myndra/
├── docs/
│   ├── programming_guide.md     # Complete tutorial
│   ├── language_reference.md    # Detailed specification  
│   └── quick_start.md          # Getting started
├── examples/                    # Working .myn code samples
│   ├── hello_world.myn
│   ├── calculator.myn
│   └── context_aware_demo.myn
├── CORE_DEVELOPMENT_PLAN.md     # Implementation roadmap
├── README.md                    # Project overview
└── MYNDRA_AI_REFERENCE.md      # This file
```

---

*Last Updated: Current implementation status - Use this as the authoritative reference for AI programming assistance*