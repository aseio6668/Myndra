# Pomegranate Language Reference

## Table of Contents

1. [Language Philosophy](#language-philosophy)
2. [Basic Syntax](#basic-syntax)
3. [Context-Aware Features](#context-aware-features)
4. [Live Code Capsules](#live-code-capsules)
5. [Reactive Programming](#reactive-programming)
6. [Temporal Types](#temporal-types)
7. [Capability-Based Security](#capability-based-security)
8. [Decentralized Identity](#decentralized-identity)
9. [Inline DSL Blocks](#inline-dsl-blocks)
10. [Self-Healing Error Handling](#self-healing-error-handling)
11. [Semantic Navigation](#semantic-navigation)
12. [Package Management](#package-management)
13. [Execution Models](#execution-models)

## Language Philosophy

Pomegranate is designed to be:

- **Context-Aware**: Code adapts to execution environment
- **Live and Dynamic**: Functions can be modified at runtime
- **Reactive by Design**: Built-in reactive programming primitives
- **Secure by Default**: Capability-based permission system
- **Privacy-First**: Decentralized identity with zero-knowledge proofs
- **Developer-Friendly**: Intuitive syntax with powerful features
- **Machine Learning Ready**: Designed for ML workflows

## Basic Syntax

### Variables and Constants

```pomegranate
let mutable_var = 42
let immutable_const: const int = 100
let inferred_type = "Hello"  // Type inferred as string

// Temporal types (change over time)
let score: evolving<int> = 0
let position: evolving<float> = 0.0
```

### Functions

```pomegranate
fn greet(name: string) -> string {
    return "Hello, " + name + "!"
}

// With fallback strategy
fn fetch_data(url: string) -> Data 
    fallback retry(3) or return default_data() {
    // Function body
}

// Execution model annotations
@async
fn download_file(url: string) -> Promise<File> {
    // Async function
}

@parallel
fn process_data(items: [Item]) -> [ProcessedItem] {
    // Parallel processing
}
```

### Control Flow

```pomegranate
if condition {
    // if block
} else if other_condition {
    // else if block
} else {
    // else block
}

while condition {
    // loop body
}

for item in collection {
    // iteration
}

for i in 0..10 {
    // range iteration
}
```

## Context-Aware Features

Code can adapt based on execution context without external configuration:

```pomegranate
import std::logging with capabilities("write")

fn process_request(request: Request) -> Response {
    // Debug logging only in development
    log("Processing request: " + request.id) if context == "dev"
    
    // Performance monitoring in production
    let start_time = now() if context == "prod"
    
    let result = handle_request(request)
    
    // Different error handling per context
    if context == "prod" {
        // Log errors to monitoring system
        monitor::track_performance(now() - start_time)
    } else if context == "test" {
        // Detailed error information for testing
        validate_result_structure(result)
    }
    
    return result
}
```

### Available Contexts

- `dev`: Development environment with debug features
- `prod`: Production environment with optimizations
- `test`: Testing environment with validation
- `runtime`: General runtime context

## Live Code Capsules

Capsules are live, editable code blocks that can be modified at runtime:

```pomegranate
#tag:ui
capsule user_profile {
    fn render(user: User) -> Element {
        return ui::div([
            ui::img(user.avatar_url),
            ui::h2(user.name),
            ui::p(user.bio)
        ])
    }
    
    // This styling can be modified live
    fn get_styles() -> Styles {
        return {
            background: "#f5f5f5",
            border_radius: "8px",
            padding: "16px"
        }
    }
}

// Reload a capsule at runtime
fn update_user_profile() {
    let new_code = load_from_file("updated_profile.pom")
    capsule_manager::reload("user_profile", new_code)
}
```

## Reactive Programming

Built-in reactive primitives for data flows and event-driven systems:

```pomegranate
import std::reactive with capabilities("observe", "emit")

@reactive
fn data_dashboard() {
    // Create observables
    let sensor_data = observable<float>(0.0)
    let filtered_data = observable<float>(0.0)
    let alerts = observable<string>("")
    
    // Reactive pipeline
    sensor_data.subscribe { value ->
        let filtered = apply_filter(value)
        filtered_data.emit(filtered)
        
        if filtered > threshold {
            alerts.emit("High value: " + filtered.to_string())
        }
    }
    
    // Multiple subscribers
    alerts.subscribe { message ->
        log_alert(message)
        notify_users(message) if context == "prod"
    }
    
    return { sensor_data, filtered_data, alerts }
}

// Reactive UI components
@reactive
fn counter_component() {
    let count = observable<int>(0)
    
    let increment = { count.emit(count.current() + 1) }
    let decrement = { count.emit(count.current() - 1) }
    
    return ui::div([
        ui::h2("Count: " + count.current().to_string()),
        ui::button("++", onclick: increment),
        ui::button("--", onclick: decrement)
    ])
}
```

## Temporal Types

Variables that evolve over time, perfect for animations and state transitions:

```pomegranate
import std::temporal with capabilities("interpolate")

fn create_animation() {
    // Variables that change over time
    let x: evolving<float> = 0.0
    let y: evolving<float> = 0.0
    let opacity: evolving<float> = 1.0
    
    // Smooth transitions
    x -> 100.0 over 2s          // Move to x=100 over 2 seconds
    y -> 50.0 over 1.5s         // Move to y=50 over 1.5 seconds
    opacity -> 0.0 over 3s      // Fade out over 3 seconds
    
    // Chained transitions
    x -> 100.0 over 1s then x -> 0.0 over 1s  // Move right, then left
    
    // Easing functions
    x -> 100.0 over 2s with easing::ease_in_out
    
    // Conditional transitions
    if user_interaction {
        opacity -> 1.0 over 300ms
    }
    
    return { x, y, opacity }
}

// Physics simulation with temporal types
fn physics_ball() {
    let position: evolving<vec2> = (0, 100)
    let velocity: evolving<vec2> = (50, 0)
    
    // Update physics every frame
    @temporal(16ms) {
        velocity.y -> velocity.y - 9.81  // Gravity
        position -> position + velocity
        
        // Bounce on ground
        if position.y <= 0 {
            velocity.y -> -velocity.y * 0.8  // Bounce with damping
            position.y -> 0
        }
    }
}
```

## Capability-Based Security

Modules declare their required permissions, providing fine-grained security:

```pomegranate
// Import with specific capabilities
import net::http with capabilities("fetch", "post")
import fs::file with capabilities("read")
import db::postgres with capabilities("read", "write")

// Function-level capability checking
fn save_user_data(user: User) 
    requires capabilities("db:write", "validation:check") {
    
    validate_user_data(user)  // Requires validation capability
    db::save(user)           // Requires db:write capability
}

// Capability inheritance
fn admin_operation() 
    requires capabilities("admin:full") 
    grants capabilities("db:read", "db:write", "user:manage") {
    
    // This function can grant limited capabilities to called functions
    user_manager::list_users()      // Gets db:read
    user_manager::create_user(data) // Gets db:write, user:manage
}

// Dynamic capability checking
fn process_file(filename: string) {
    if capability_check("fs:read") {
        let content = fs::read_file(filename)
        
        if capability_check("fs:write") {
            fs::write_file(filename + ".processed", process(content))
        } else {
            log("No write permission, skipping save")
        }
    } else {
        throw PermissionError("Cannot read file: insufficient capabilities")
    }
}
```

## Decentralized Identity

Native support for DIDs and zero-knowledge proofs:

```pomegranate
import did::core with capabilities("verify", "resolve")
import crypto::zk with capabilities("prove", "verify")

fn authenticate_user(did_token: string) -> User? {
    // Resolve DID document
    let did_doc = did::resolve(did_token)
    
    if !did_doc.is_valid() {
        return nil
    }
    
    // Verify identity without revealing personal data
    let identity = did::verify(did_token)
    
    // Check zero-knowledge proofs
    if identity.has_proof("age > 18") and 
       identity.has_proof("verified_email") and
       identity.has_proof("reputation > 0.8") {
        
        // Create session with minimal data
        return User {
            session_id: generate_session_id(),
            capabilities: derive_capabilities(identity),
            verified: true
        }
    }
    
    return nil
}

// Create proofs without revealing data
fn create_age_proof(birthdate: Date) -> ZKProof {
    return zk::prove {
        statement: "age > 18",
        private_input: birthdate,
        public_output: true  // Only reveals that condition is met
    }
}

// Verify proofs
fn verify_eligibility(user_did: string) -> bool {
    let identity = did::verify(user_did)
    
    return identity.has_proof("age > 21") and
           identity.has_proof("country == 'US'") and
           identity.has_proof("criminal_background_check == 'clean'")
}
```

## Inline DSL Blocks

Embed domain-specific languages directly in Pomegranate code:

```pomegranate
import graphics::shader with capabilities("compile", "execute")
import db::sql with capabilities("query")
import markup::html with capabilities("render")

// Shader DSL
fn create_gradient_shader() -> Shader {
    dsl shader {
        precision mediump float;
        uniform float uTime;
        uniform vec2 uResolution;
        
        void main() {
            vec2 uv = gl_FragCoord.xy / uResolution.xy;
            float gradient = sin(uv.x * 3.14159 + uTime);
            gl_FragColor = vec4(gradient, 0.5, 1.0 - gradient, 1.0);
        }
    }
}

// SQL DSL
fn get_user_posts(user_id: int) -> [Post] {
    let query = dsl sql {
        SELECT posts.*, users.username, COUNT(likes.id) as like_count
        FROM posts
        JOIN users ON posts.user_id = users.id
        LEFT JOIN likes ON posts.id = likes.post_id
        WHERE posts.user_id = :user_id
        AND posts.published = true
        GROUP BY posts.id
        ORDER BY posts.created_at DESC
        LIMIT 20
    }
    
    return db::execute(query, { user_id }).map(Post::from_row)
}

// HTML DSL
fn render_user_card(user: User) -> Element {
    dsl html {
        <div class="user-card">
            <img src="${user.avatar}" alt="${user.name}" />
            <h3>${user.name}</h3>
            <p class="bio">${user.bio}</p>
            <div class="stats">
                <span>Posts: ${user.post_count}</span>
                <span>Followers: ${user.follower_count}</span>
            </div>
        </div>
    }
}

// CSS DSL
fn get_component_styles() -> Styles {
    dsl css {
        .user-card {
            background: linear-gradient(45deg, #667eea 0%, #764ba2 100%);
            border-radius: 12px;
            padding: 20px;
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);
            transition: transform 0.3s ease;
        }
        
        .user-card:hover {
            transform: translateY(-5px);
        }
        
        .user-card img {
            width: 60px;
            height: 60px;
            border-radius: 50%;
            object-fit: cover;
        }
    }
}
```

## Self-Healing Error Handling

Built-in fallback strategies and automatic error recovery:

```pomegranate
// Function with multiple fallback strategies
fn fetch_user_data(user_id: string) -> User
    fallback retry(3) or return cached_user(user_id) or return default_user() {
    
    let response = http::get("/api/users/" + user_id)
    
    if response.status != 200 {
        throw HttpError("User not found")
    }
    
    return User::parse(response.body)
}

// Circuit breaker pattern
fn external_api_call(endpoint: string) -> ApiResponse
    fallback circuit_breaker(failure_threshold: 5, timeout: 30s) {
    
    return http::get("https://external-api.com" + endpoint)
}

// Progressive degradation
fn render_dashboard() -> Element
    fallback graceful_degradation {
    
    let live_data = fetch_live_data()  // Might fail
    let charts = render_charts(live_data)  // Might fail
    
    return ui::div([
        ui::header("Dashboard"),
        charts,  // Will show error message if failed
        ui::footer("Last updated: " + now().format())
    ])
}

// Automatic retry with exponential backoff
fn send_notification(message: string) -> bool
    fallback exponential_backoff(max_attempts: 5, base_delay: 1s) {
    
    return notification_service::send(message)
}

// Custom error recovery
fn process_payment(payment: Payment) -> PaymentResult
    fallback {
        log_payment_failure(payment)
        refund_customer(payment.customer_id, payment.amount)
        return PaymentResult::failed("Payment processing unavailable")
    } {
    
    return payment_gateway::process(payment)
}
```

## Semantic Navigation

Use semantic tags for better code organization and navigation:

```pomegranate
#tag:auth
fn login(credentials: Credentials) -> Session? {
    // Authentication logic
}

#tag:auth
fn logout(session: Session) {
    // Logout logic
}

#tag:ui:components
capsule login_form {
    fn render() -> Element {
        // Login form UI
    }
}

#tag:ui:components
capsule user_profile {
    fn render(user: User) -> Element {
        // User profile UI
    }
}

#tag:data:validation
fn validate_email(email: string) -> bool {
    // Email validation
}

#tag:data:validation
fn validate_password(password: string) -> ValidationResult {
    // Password validation
}

#tag:api:endpoints
fn get_user_api(user_id: string) -> ApiResponse {
    // API endpoint
}

#tag:api:endpoints
fn update_user_api(user_id: string, data: UserUpdate) -> ApiResponse {
    // API endpoint
}

#tag:performance:critical
fn process_large_dataset(data: [DataPoint]) -> ProcessedData {
    // Performance-critical function
}

#tag:security:sensitive
fn handle_sensitive_data(data: SensitiveData) -> ProcessedData {
    // Security-sensitive function
}
```

## Package Management

Hash-based package system with semantic understanding:

```pomegranate
// Import packages by hash and intent
import "ui-components" from hash("a1b2c3d4e5f6...") 
    with intent("reactive-ui-v2")
    with capabilities("render", "event-handling")

import "ml-utils" from hash("f1e2d3c4b5a6...") 
    with intent("tensorflow-bindings-v1")
    with capabilities("gpu-compute", "model-training")

// Package definition in pom.toml
/*
[package]
name = "my-awesome-lib"
hash = "generated-from-content"
intent = "data-processing-v1"

[dependencies]
"std.collections" = { 
    hash = "abc123...", 
    intent = "collections-optimized-v2" 
}

[capabilities]
required = ["memory-management", "multi-threading"]
optional = ["gpu-acceleration"]
*/

// Install packages by intent resolution
fn setup_project() {
    // Package manager finds compatible versions
    package_manager::install_intent("ui-framework", "modern-reactive")
    package_manager::install_intent("database", "nosql-fast")
    package_manager::install_intent("auth", "oauth2-secure")
}
```

## Execution Models

Choose how code runs with simple annotations:

```pomegranate
// Synchronous (default)
fn compute_result(data: Data) -> Result {
    return process(data)
}

// Asynchronous
@async
fn download_file(url: string) -> Promise<File> {
    let response = await http::get(url)
    return response.to_file()
}

// Parallel processing
@parallel
fn process_images(images: [Image]) -> [ProcessedImage] {
    return images.map(apply_filters)  // Automatically parallelized
}

// Reactive (responds to input changes)
@reactive
fn update_search_results(query: string) -> [SearchResult] {
    // Automatically re-runs when query changes
    return search_engine::search(query)
}

// Temporal (time-based execution)
@temporal(interval: 1s)
fn update_clock() {
    current_time -> now()
}

// Combined annotations
@async @parallel
fn batch_process_data(datasets: [Dataset]) -> Promise<[ProcessedDataset]> {
    // Async parallel processing
    return Promise.all(datasets.map(process_dataset))
}

// Conditional execution based on context
@reactive @if(context == "dev")
fn debug_monitor(app_state: AppState) {
    // Only runs reactively in development
    debug_panel::update(app_state)
}
```

## Best Practices

1. **Use semantic tags** to organize code logically
2. **Leverage capability-based security** to minimize attack surface
3. **Prefer reactive patterns** for UI and data flow
4. **Use temporal types** for smooth animations and transitions
5. **Implement fallback strategies** for robust error handling
6. **Use context-aware features** to reduce configuration complexity
7. **Design with live reloading** in mind for better development experience
8. **Hash-based packages** ensure reproducible builds
9. **DID integration** for privacy-preserving authentication
10. **Inline DSLs** for domain-specific problems

## Example: Complete Application

Here's a complete example showing multiple Pomegranate features:

```pomegranate
import std::ui with capabilities("render", "event-handling")
import std::reactive with capabilities("observe", "emit")
import net::api with capabilities("fetch")
import did::auth with capabilities("verify")

#tag:app:main
@reactive
capsule todo_app {
    let todos: evolving<[Todo]> = []
    let filter: evolving<string> = "all"
    let user: evolving<User?> = nil
    
    // Authentication
    #tag:auth
    fn authenticate(did_token: string) fallback show_error("Login failed") {
        let identity = did::verify(did_token)
        
        if identity.has_proof("verified_user") {
            user -> User::from_identity(identity) over 300ms
            load_user_todos()
        }
    }
    
    // Data loading with fallbacks
    #tag:data
    @async
    fn load_user_todos() fallback retry(3) or use_cached_todos() {
        let response = api::get("/todos?user=" + user.id)
        todos -> Todo::parse_list(response.body) over 200ms
    }
    
    // Todo operations
    #tag:operations
    fn add_todo(text: string) {
        let new_todo = Todo {
            id: generate_id(),
            text: text,
            completed: false,
            created_at: now()
        }
        
        todos -> todos + [new_todo] over 150ms
        
        @async {
            api::post("/todos", new_todo.to_json())
        }
    }
    
    #tag:operations
    fn toggle_todo(id: string) {
        todos -> todos.map { todo ->
            if todo.id == id {
                Todo { ...todo, completed: !todo.completed }
            } else {
                todo
            }
        } over 100ms
    }
    
    // Filtered todos (reactive)
    @reactive
    fn filtered_todos() -> [Todo] {
        return todos.filter { todo ->
            filter == "all" or
            (filter == "active" and !todo.completed) or
            (filter == "completed" and todo.completed)
        }
    }
    
    // UI rendering
    #tag:ui
    fn render() -> Element {
        if !user {
            return render_login()
        }
        
        return ui::div([
            ui::h1("Pomegranate Todos"),
            ui::input_field("Add todo...", on_submit: add_todo),
            
            ui::div(filtered_todos().map(render_todo)),
            
            ui::div([
                ui::button("All", onclick: { filter -> "all" }),
                ui::button("Active", onclick: { filter -> "active" }),
                ui::button("Completed", onclick: { filter -> "completed" })
            ])
        ])
    }
    
    #tag:ui:components
    fn render_todo(todo: Todo) -> Element {
        return ui::div([
            ui::checkbox(todo.completed, onchange: { toggle_todo(todo.id) }),
            ui::span(todo.text, class: todo.completed ? "completed" : ""),
            ui::button("×", onclick: { remove_todo(todo.id) })
        ])
    }
    
    #tag:ui:auth
    fn render_login() -> Element {
        return ui::div([
            ui::h2("Login with DID"),
            ui::input_field("DID Token", on_submit: authenticate),
            ui::p("Enter your decentralized identity token to continue")
        ])
    }
}

// Main application entry point
fn main() {
    let app = todo_app.render()
    ui::mount(app, "#app")
    
    log("Todo app started") if context == "dev"
}
```

This example demonstrates:
- Context-aware logging
- Live code capsules
- Reactive programming
- Temporal transitions
- DID authentication
- Self-healing error handling
- Semantic tags
- Async operations
- UI components

The Pomegranate language brings together cutting-edge programming concepts in an intuitive, developer-friendly package that's ready for the future of software development.
