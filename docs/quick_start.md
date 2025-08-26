# Quick Start Guide - Myndra Programming Language

Welcome to Myndra! This guide will get you up and running with the most innovative programming language of 2025.

## Installation

### Prerequisites

- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
- CMake 3.16+
- Git

### Build from Source

```bash
# Clone the repository
git clone https://github.com/myndra-lang/myndra.git
cd myndra

# Build the compiler
./build.sh    # On Linux/macOS
# or
build.bat     # On Windows

# The compiler will be in build/myndra
```

## Your First Myndra Program

Create a file called `hello.myn`:

```myndra
import std::io with capabilities("write")

fn main() {
    print("Hello, Myndra!")
    log("First program executed successfully!") if context == "dev"
}
```

Run it:

```bash
./build/myndra hello.myn --run
```

## Language Features Overview

### 1. Context-Aware Programming

```myndra
fn process_request(data: RequestData) {
    // Automatic behavior based on context
    log("Processing: " + data.id) if context == "dev"
    
    let result = expensive_operation(data)
    
    // Production monitoring
    metrics::record("request_processed") if context == "prod"
    
    return result
}
```

### 2. Live Code Capsules

```myndra
#tag:ui
capsule button_component {
    fn render(text: string, onclick: fn()) -> Element {
        return ui::button(text) {
            style: {
                background: "#007acc",
                color: "white",
                padding: "10px 20px"
            },
            onclick: onclick
        }
    }
}

// This can be reloaded at runtime!
```

### 3. Reactive Programming

```myndra
@reactive
fn counter_app() {
    let count = observable<int>(0)
    
    // UI automatically updates when count changes
    count.subscribe { value ->
        ui::update_display("count", value.to_string())
    }
    
    return {
        increment: { count.emit(count.current() + 1) },
        decrement: { count.emit(count.current() - 1) }
    }
}
```

### 4. Temporal Types

```myndra
fn create_animation() {
    let x: evolving<float> = 0.0
    let opacity: evolving<float> = 1.0
    
    // Smooth transitions
    x -> 100.0 over 2s
    opacity -> 0.0 over 1s
    
    // Chained animations
    x -> 200.0 over 1s then x -> 0.0 over 1s
}
```

### 5. Capability-Based Security

```myndra
import net::http with capabilities("fetch", "read-only")
import fs::file with capabilities("read")

fn safe_operation() requires capabilities("data:read", "network:fetch") {
    let config = fs::read_file("config.json")  // Allowed
    let data = http::get("https://api.example.com")  // Allowed
    // fs::write_file(...) would fail - no write capability
}
```

### 6. Decentralized Identity

```myndra
fn authenticate_user(did_token: string) -> User? {
    let identity = did::verify(did_token)
    
    // Verify without revealing personal data
    if identity.has_proof("age > 18") and
       identity.has_proof("verified_email") {
        return create_user_session(identity)
    }
    
    return nil
}
```

### 7. Inline DSL

```myndra
fn create_shader() -> Shader {
    dsl shader {
        precision mediump float;
        uniform float uTime;
        
        void main() {
            float wave = sin(gl_FragCoord.x * 0.01 + uTime);
            gl_FragColor = vec4(wave, 0.5, 1.0 - wave, 1.0);
        }
    }
}

fn query_users() -> [User] {
    let query = dsl sql {
        SELECT * FROM users 
        WHERE active = true 
        ORDER BY created_at DESC 
        LIMIT 10
    }
    return db::execute(query)
}
```

### 8. Self-Healing Error Handling

```myndra
fn fetch_data(url: string) -> Data 
    fallback retry(3) or return cached_data() or return default_data() {
    
    let response = http::get(url)
    if response.status != 200 {
        throw HttpError("Request failed")
    }
    return Data::parse(response.body)
}
```

## Interactive Development

Start the REPL for interactive development:

```bash
./build/myndra --interactive
```

```myndra
pom> let x = 42
pom> x + 8
=> 50

pom> let greeting = "Hello, " + "Myndra!"
pom> greeting
=> "Hello, Myndra!"

pom> context dev
Context changed to: dev

pom> log("This will show in dev context")
This will show in dev context
```

## Package Management

Myndra uses hash-based packages for reproducible builds:

```myndra
// Install packages by semantic intent
import "ui-framework" from intent("modern-reactive-ui")
import "database" from intent("fast-nosql-db")

// Or by specific hash for exact reproducibility
import "crypto-utils" from hash("a1b2c3d4e5f6789...")
```

Create a package definition (`pom.toml`):

```toml
[package]
name = "my-awesome-lib"
description = "An awesome library"
intent = "data-processing-v1"

[dependencies]
"std.collections" = { intent = "collections-optimized" }

[capabilities]
required = ["data:read", "data:write"]
```

## Project Structure

Organize your Myndra project:

```
my-project/
├── pom.toml              # Package definition
├── src/
│   ├── main.myn          # Main entry point
│   ├── auth/             # Authentication module
│   │   ├── mod.myn
│   │   └── did.myn
│   ├── ui/               # UI components
│   │   ├── mod.myn
│   │   └── components.myn
│   └── data/             # Data processing
│       ├── mod.myn
│       └── processors.myn
├── tests/
│   └── integration.myn
└── examples/
    └── demo.myn
```

## Common Patterns

### Reactive UI Component

```myndra
#tag:ui:components
capsule todo_item {
    let completed: evolving<bool> = false
    let text: evolving<string> = ""
    
    fn render() -> Element {
        return ui::div([
            ui::checkbox(completed, onchange: { 
                completed -> !completed over 150ms 
            }),
            ui::span(text, class: completed ? "done" : "active"),
            ui::button("Delete", onclick: delete_item)
        ])
    }
    
    fn delete_item() {
        // Fade out animation before removal
        opacity -> 0.0 over 300ms then remove_from_parent()
    }
}
```

### API Service with Fallbacks

```myndra
#tag:api:services
fn user_service() {
    fn get_user(id: string) -> User 
        fallback retry(3) or return cached_user(id) or return anonymous_user() {
        
        let response = http::get("/api/users/" + id) {
            timeout: 5s,
            capabilities: ["network:fetch"]
        }
        
        return User::from_json(response.body)
    }
    
    @async
    fn update_user(id: string, data: UserUpdate) -> bool
        fallback log_failure_and_retry(data) {
        
        let response = http::put("/api/users/" + id, data.to_json())
        return response.status == 200
    }
    
    return { get_user, update_user }
}
```

### Machine Learning Pipeline

```myndra
#tag:ml:pipeline
@parallel
fn training_pipeline(dataset: Dataset) -> Model {
    // Preprocess data in parallel
    let cleaned_data = dataset.chunks(1000).map(clean_data)
    
    // Feature extraction
    let features = extract_features(cleaned_data)
    
    // Train model with temporal progress tracking
    let training_progress: evolving<float> = 0.0
    
    let model = train_model(features) {
        on_progress: { progress ->
            training_progress -> progress over 100ms
            ui::update_progress_bar(progress) if context == "dev"
        }
    }
    
    return model
}
```

## Development Tips

1. **Use `--interactive` mode** for experimenting
2. **Leverage semantic tags** (`#tag:ui`, `#tag:auth`) for navigation
3. **Design with contexts in mind** - your code should adapt to dev/prod/test
4. **Embrace reactive patterns** - let data flow drive your application
5. **Use temporal types** for smooth user experiences
6. **Start with capabilities** - think about what permissions your code needs
7. **Hash-based packages** ensure reproducible builds across environments

## Next Steps

1. Read the [Language Reference](docs/language_reference.md) for complete documentation
2. Explore the [examples/](examples/) directory for more complex examples
3. Check out the [Package Registry](https://packages.myndra-lang.org) for available packages
4. Join the [Community](https://community.myndra-lang.org) for support and discussion

## Getting Help

- **Documentation**: [docs.myndra-lang.org](https://docs.myndra-lang.org)
- **Community**: [community.myndra-lang.org](https://community.myndra-lang.org)
- **Issues**: [github.com/myndra-lang/myndra/issues](https://github.com/myndra-lang/myndra/issues)
- **Discord**: [discord.gg/myndra](https://discord.gg/myndra)

Welcome to the future of programming with Myndra! 🚀
