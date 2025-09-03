# Myndra Programming Guide

*A comprehensive guide to programming in Myndra - from basics to advanced features*

---

## 🎯 Table of Contents

1. [Getting Started](#getting-started)
2. [Basic Syntax](#basic-syntax)
3. [Variables and Types](#variables-and-types)
4. [Functions](#functions)
5. [Control Flow](#control-flow)
6. [Data Structures](#data-structures)
7. [Context-Aware Programming](#context-aware-programming)
8. [Error Handling](#error-handling)
9. [Modules and Imports](#modules-and-imports)
10. [Advanced Features](#advanced-features)
11. [Best Practices](#best-practices)
12. [Common Patterns](#common-patterns)

---

## Getting Started

### Your First Myndra Program

Create a file named `hello.myn`:

```myndra
fn main() {
    print("Hello, Myndra!")
    print("Welcome to context-aware programming!")
}
```

Compile and run:
```bash
myndra hello.myn --run
```

### Interactive Development

Start the REPL for interactive programming:
```bash
myndra --interactive
```

Try these commands:
```myndra
myn> let greeting = "Hello"
myn> print(greeting)
myn> let numbers = [1, 2, 3, 4, 5]
myn> print(numbers)
```

---

## Basic Syntax

### Comments

```myndra
// Single line comment

/*
   Multi-line comment
   Spans multiple lines
*/

/// Documentation comment
/// Used for function and type documentation
fn documented_function() {
    // Implementation here
}
```

### Statements and Expressions

Myndra distinguishes between statements and expressions:

```myndra
// Statements perform actions
let x = 5;          // Variable declaration
print("hello");     // Function call

// Expressions evaluate to values
let y = if x > 0 { "positive" } else { "non-positive" }
let sum = a + b
```

### Identifiers

Valid identifier names:
```myndra
let my_variable = 42
let userName = "Alice"
let MAX_SIZE = 1000
let _private = "hidden"
```

---

## Variables and Types

### Variable Declaration

```myndra
// Immutable by default (recommended)
let name = "Alice"
let age = 25

// Mutable variables (use sparingly)
let mut counter = 0
counter = counter + 1

// Explicit type annotations
let price: float = 19.99
let is_valid: bool = true
```

### Basic Types

```myndra
// Integers
let small: i8 = 127          // 8-bit signed
let big: i64 = 9223372036854775807
let natural: uint = 42       // Unsigned integer

// Floating point
let pi: float = 3.14159
let precise: f64 = 2.718281828459045

// Boolean
let is_ready: bool = true
let is_done: bool = false

// Strings
let greeting: string = "Hello, world!"
let multiline: string = """
    This is a
    multi-line string
    with preserved formatting
"""

// Characters
let initial: char = 'A'
let emoji: char = '🚀'
```

### Type Inference

Myndra can infer types in most cases:

```myndra
let count = 42              // Inferred as int
let pi = 3.14159           // Inferred as float  
let message = "hello"       // Inferred as string
let items = [1, 2, 3]      // Inferred as Vec<int>
```

---

## Functions

### Function Definition

```myndra
// Basic function
fn greet(name: string) {
    print("Hello, " + name + "!")
}

// Function with return value
fn add(a: int, b: int) -> int {
    return a + b
}

// Expression-bodied function
fn multiply(a: int, b: int) -> int = a * b

// Multiple parameters and return values
fn divide_with_remainder(dividend: int, divisor: int) -> (int, int) {
    return (dividend / divisor, dividend % divisor)
}
```

### Function Calls

```myndra
// Simple call
greet("Alice")

// With return value
let sum = add(10, 20)
print("Sum: {}", sum)

// Destructuring return values
let (quotient, remainder) = divide_with_remainder(17, 5)
print("{} divided by {} is {} remainder {}", 17, 5, quotient, remainder)
```

### Higher-Order Functions

```myndra
// Function as parameter
fn apply_twice(f: fn(int) -> int, x: int) -> int {
    return f(f(x))
}

fn double(n: int) -> int = n * 2

// Usage
let result = apply_twice(double, 5)  // Returns 20
```

### Lambda Expressions

```myndra
// Simple lambda
let square = |x: int| -> int { x * x }

// Lambda with type inference
let is_even = |n| n % 2 == 0

// Multi-line lambda
let process = |data: Vec<int>| {
    let filtered = data.filter(|x| x > 0)
    return filtered.map(|x| x * 2)
}
```

---

## Control Flow

### Conditional Statements

```myndra
// Basic if-else
let temperature = 25
if temperature > 30 {
    print("It's hot!")
} else if temperature > 20 {
    print("Nice weather!")
} else {
    print("It's cool.")
}

// If as expression
let status = if age >= 18 { "adult" } else { "minor" }

// Condition with complex logic
if (age >= 18 and has_license) or is_emergency {
    print("Can drive")
}
```

### Pattern Matching

```myndra
// Basic match
let number = 42
match number {
    0 => print("zero"),
    1..10 => print("single digit"),
    42 => print("the answer!"),
    _ => print("some other number")
}

// Match with return values
let description = match weather {
    "sunny" => "Great day for a walk!",
    "rainy" => "Don't forget an umbrella!",
    "snowy" => "Perfect for skiing!",
    _ => "Weather is weather"
}

// Match with guards
match value {
    x if x < 0 => print("negative"),
    x if x == 0 => print("zero"),
    x if x > 100 => print("large positive"),
    x => print("small positive: {}", x)
}
```

### Loops

```myndra
// For loop with range
for i in 0..10 {
    print("Number: {}", i)
}

// For loop with collection
let names = ["Alice", "Bob", "Charlie"]
for name in names {
    print("Hello, {}!", name)
}

// While loop
let mut count = 0
while count < 5 {
    print("Count: {}", count)
    count = count + 1
}

// Infinite loop with break
loop {
    let input = read_line()
    if input == "quit" {
        break
    }
    print("You said: {}", input)
}

// Continue statement
for i in 0..10 {
    if i % 2 == 0 {
        continue
    }
    print("Odd number: {}", i)
}
```

---

## Data Structures

### Arrays and Vectors

```myndra
// Fixed-size arrays
let numbers: [int; 5] = [1, 2, 3, 4, 5]
let zeros = [0; 10]  // Array of 10 zeros

// Dynamic vectors
let mut items: Vec<string> = Vec::new()
items.push("apple")
items.push("banana")

// Vector literals
let fruits = ["apple", "banana", "orange"]
let lengths = fruits.map(|fruit| fruit.len())
```

### Strings

```myndra
// String creation
let name = "Alice"
let mut greeting = String::new()
greeting.push_str("Hello, ")
greeting.push_str(name)

// String interpolation
let age = 25
let message = "My name is {} and I'm {} years old"
print(message.format(name, age))

// String methods
let text = "Hello, World!"
print(text.to_upper())          // "HELLO, WORLD!"
print(text.to_lower())          // "hello, world!"
print(text.substring(0, 5))     // "Hello"
print(text.contains("World"))   // true
```

### Structs

```myndra
// Struct definition
struct Person {
    name: string,
    age: int,
    email: string
}

// Struct instantiation
let alice = Person {
    name: "Alice",
    age: 30,
    email: "alice@example.com"
}

// Accessing fields
print("Name: {}", alice.name)
print("Age: {}", alice.age)

// Struct methods
impl Person {
    fn greet(self) {
        print("Hello, my name is {}", self.name)
    }
    
    fn is_adult(self) -> bool {
        return self.age >= 18
    }
    
    fn have_birthday(mut self) {
        self.age = self.age + 1
    }
}

// Using methods
alice.greet()
if alice.is_adult() {
    print("{} is an adult", alice.name)
}
```

### Enums

```myndra
// Basic enum
enum Status {
    Pending,
    InProgress,
    Complete,
    Failed
}

// Enum with data
enum Message {
    Text(string),
    Image(string, int, int),  // path, width, height
    Video(string, int)        // path, duration
}

// Pattern matching with enums
let current_status = Status::InProgress
match current_status {
    Status::Pending => print("Waiting to start"),
    Status::InProgress => print("Working on it"),
    Status::Complete => print("All done!"),
    Status::Failed => print("Something went wrong")
}

// Matching enum data
let msg = Message::Image("photo.jpg", 800, 600)
match msg {
    Message::Text(content) => print("Text: {}", content),
    Message::Image(path, w, h) => print("Image: {} ({}x{})", path, w, h),
    Message::Video(path, duration) => print("Video: {} ({}s)", path, duration)
}
```

---

## Context-Aware Programming

### Understanding Contexts

Myndra recognizes three main execution contexts:
- **dev**: Development/debugging
- **test**: Testing environment  
- **prod**: Production deployment

```myndra
fn main() {
    print("Application starting...")
    
    // Context-aware logging
    log("Debug info: initializing systems") if context == "dev"
    log("Test run started") if context == "test"
    
    // Context-based configuration
    let database_url = if context == "prod" {
        "postgres://prod-server/myapp"
    } else if context == "test" {
        "postgres://test-server/myapp_test"
    } else {
        "sqlite://dev.db"
    }
    
    connect_to_database(database_url)
}
```

### Context-Aware Functions

```myndra
fn setup_logging() {
    match context {
        "dev" => {
            enable_debug_logging()
            enable_file_logging("debug.log")
        },
        "test" => {
            enable_minimal_logging()
        },
        "prod" => {
            enable_structured_logging()
            enable_error_reporting()
        }
    }
}

fn get_api_timeout() -> int {
    return if context == "test" { 1000 } else { 5000 }
}
```

### Conditional Compilation

```myndra
// Code that only exists in development
fn debug_dump_state(app_state: AppState) if context == "dev" {
    print("=== DEBUG STATE DUMP ===")
    print("Users: {}", app_state.users.len())
    print("Active sessions: {}", app_state.sessions.len())
    print("========================")
}

// Performance monitoring only in production
fn track_performance(operation: string, duration: int) if context == "prod" {
    send_metrics("operation_duration", operation, duration)
}
```

### Environment Variables and Context

```myndra
fn determine_context() -> string {
    return env::get("MYNDRA_CONTEXT").unwrap_or("dev")
}

fn load_config() -> Config {
    let config_file = match context {
        "prod" => "config/production.toml",
        "test" => "config/testing.toml", 
        _ => "config/development.toml"
    }
    
    return Config::from_file(config_file)
}
```

---

## Error Handling

### Result Type

```myndra
// Function that can fail
fn divide(a: float, b: float) -> Result<float, string> {
    if b == 0.0 {
        return Err("Division by zero")
    }
    return Ok(a / b)
}

// Handling results
match divide(10.0, 2.0) {
    Ok(result) => print("Result: {}", result),
    Err(error) => print("Error: {}", error)
}

// Using ? operator for early return
fn calculate_average(numbers: Vec<float>) -> Result<float, string> {
    if numbers.is_empty() {
        return Err("Cannot calculate average of empty list")
    }
    
    let sum = numbers.iter().sum()?
    return Ok(sum / numbers.len() as float)
}
```

### Option Type

```myndra
// Function that may not return a value
fn find_user(id: int) -> Option<User> {
    // Search logic here
    if user_exists(id) {
        return Some(get_user_by_id(id))
    } else {
        return None
    }
}

// Handling options
match find_user(123) {
    Some(user) => print("Found user: {}", user.name),
    None => print("User not found")
}

// Using unwrap_or for defaults
let user = find_user(456).unwrap_or(get_default_user())

// Chaining operations
let email_domain = find_user(789)
    .map(|user| user.email)
    .map(|email| email.split("@").last())
    .unwrap_or("unknown")
```

### Custom Error Types

```myndra
// Define custom error types
enum DatabaseError {
    ConnectionFailed(string),
    QueryFailed(string),
    RecordNotFound(int),
    InvalidData(string)
}

impl DatabaseError {
    fn message(self) -> string {
        match self {
            DatabaseError::ConnectionFailed(msg) => "Connection failed: " + msg,
            DatabaseError::QueryFailed(query) => "Query failed: " + query,
            DatabaseError::RecordNotFound(id) => "Record not found: " + id.to_string(),
            DatabaseError::InvalidData(data) => "Invalid data: " + data
        }
    }
}

// Using custom error types
fn get_user_from_db(id: int) -> Result<User, DatabaseError> {
    let connection = connect_to_db()
        .map_err(|e| DatabaseError::ConnectionFailed(e.message()))?
    
    let user = connection.query("SELECT * FROM users WHERE id = ?", [id])
        .map_err(|e| DatabaseError::QueryFailed(e.to_string()))?
    
    if user.is_empty() {
        return Err(DatabaseError::RecordNotFound(id))
    }
    
    return Ok(parse_user(user))
}
```

---

## Modules and Imports

### Module Definition

```myndra
// File: math_utils.myn
pub fn add(a: int, b: int) -> int {
    return a + b
}

pub fn multiply(a: int, b: int) -> int {
    return a * b
}

fn internal_helper() -> int {
    // Private function, not accessible from outside
    return 42
}

pub struct Calculator {
    memory: int
}

impl Calculator {
    pub fn new() -> Calculator {
        return Calculator { memory: 0 }
    }
    
    pub fn add_to_memory(mut self, value: int) {
        self.memory = self.memory + value
    }
}
```

### Importing Modules

```myndra
// Basic import
import math_utils

fn main() {
    let result = math_utils::add(5, 3)
    print("Result: {}", result)
}

// Selective import
import math_utils::{add, Calculator}

fn main() {
    let sum = add(10, 20)
    let calc = Calculator::new()
}

// Import with alias
import math_utils::Calculator as Calc

fn main() {
    let my_calc = Calc::new()
}

// Import all public items
import math_utils::*

fn main() {
    let product = multiply(6, 7)
}
```

### Capability-Based Imports

```myndra
// Import with specific capabilities
import file_system with capabilities("read", "write")
import network with capabilities("http_client")
import crypto with capabilities("hash", "encrypt")

// Restricted import (read-only)
import database with capabilities("read")

fn backup_data() {
    // Can read from database
    let data = database::get_all_records()
    
    // Can write to file system
    file_system::write_file("backup.json", data.to_json())
    
    // Cannot write to database - would cause compile error
    // database::insert_record(new_record)  // ❌ Compile error
}
```

---

## Advanced Features

### Generic Functions

```myndra
// Generic function
fn swap<T>(mut a: T, mut b: T) -> (T, T) {
    return (b, a)
}

// Usage with different types
let (x, y) = swap(1, 2)           // int
let (first, second) = swap("hello", "world")  // string

// Generic with constraints
fn print_debug<T: Debug>(value: T) {
    print("Debug: {:?}", value)
}

// Multiple type parameters
fn merge<K, V>(map1: Map<K, V>, map2: Map<K, V>) -> Map<K, V>
where K: Hash + Eq {
    // Implementation here
}
```

### Traits

```myndra
// Define a trait
trait Drawable {
    fn draw(self) -> string
    
    fn area(self) -> float
    
    // Default implementation
    fn describe(self) -> string {
        return "A drawable shape with area " + self.area().to_string()
    }
}

// Implement trait for a struct
struct Rectangle {
    width: float,
    height: float
}

impl Drawable for Rectangle {
    fn draw(self) -> string {
        return "Rectangle({}x{})".format(self.width, self.height)
    }
    
    fn area(self) -> float {
        return self.width * self.height
    }
}

// Using traits
fn print_shape_info<T: Drawable>(shape: T) {
    print("Shape: {}", shape.draw())
    print("Area: {}", shape.area())
    print("Description: {}", shape.describe())
}
```

### Operator Overloading

```myndra
struct Vector2D {
    x: float,
    y: float
}

impl Vector2D {
    fn new(x: float, y: float) -> Vector2D {
        return Vector2D { x, y }
    }
}

// Implement Add trait for + operator
impl Add for Vector2D {
    type Output = Vector2D
    
    fn add(self, other: Vector2D) -> Vector2D {
        return Vector2D {
            x: self.x + other.x,
            y: self.y + other.y
        }
    }
}

// Usage
let v1 = Vector2D::new(1.0, 2.0)
let v2 = Vector2D::new(3.0, 4.0)
let sum = v1 + v2  // Now works with + operator
```

---

## Best Practices

### Code Organization

```myndra
// Good: Clear function names and single responsibility
fn calculate_user_discount(user: User, purchase_amount: float) -> float {
    if user.is_premium_member() {
        return purchase_amount * 0.1
    } else {
        return 0.0
    }
}

// Good: Use context awareness appropriately
fn log_performance(operation: string, duration: int) {
    if context == "dev" {
        print("DEBUG: {} took {}ms", operation, duration)
    }
    
    if context == "prod" {
        send_to_monitoring_system(operation, duration)
    }
}
```

### Error Handling

```myndra
// Good: Use Result types for operations that can fail
fn parse_config_file(path: string) -> Result<Config, ConfigError> {
    let content = read_file(path)
        .map_err(|e| ConfigError::FileReadError(e))?
    
    let config = parse_toml(content)
        .map_err(|e| ConfigError::ParseError(e))?
    
    return Ok(config)
}

// Good: Provide meaningful error messages
fn validate_user_input(input: string) -> Result<UserData, string> {
    if input.is_empty() {
        return Err("User input cannot be empty")
    }
    
    if input.len() > 1000 {
        return Err("User input is too long (max 1000 characters)")
    }
    
    return Ok(parse_user_data(input))
}
```

### Performance Considerations

```myndra
// Good: Use iterators for efficient data processing
fn process_large_dataset(data: Vec<Record>) -> Vec<ProcessedRecord> {
    return data
        .iter()
        .filter(|record| record.is_valid())
        .map(|record| process_record(record))
        .collect()
}

// Good: Avoid unnecessary allocations
fn count_vowels(text: string) -> int {
    let vowels = "aeiouAEIOU"
    return text.chars().filter(|c| vowels.contains(*c)).count()
}
```

---

## Common Patterns

### Builder Pattern

```myndra
struct HttpRequest {
    url: string,
    method: string,
    headers: Map<string, string>,
    body: Option<string>
}

impl HttpRequest {
    fn builder() -> HttpRequestBuilder {
        return HttpRequestBuilder::new()
    }
}

struct HttpRequestBuilder {
    request: HttpRequest
}

impl HttpRequestBuilder {
    fn new() -> HttpRequestBuilder {
        return HttpRequestBuilder {
            request: HttpRequest {
                url: "",
                method: "GET",
                headers: Map::new(),
                body: None
            }
        }
    }
    
    fn url(mut self, url: string) -> HttpRequestBuilder {
        self.request.url = url
        return self
    }
    
    fn method(mut self, method: string) -> HttpRequestBuilder {
        self.request.method = method
        return self
    }
    
    fn header(mut self, key: string, value: string) -> HttpRequestBuilder {
        self.request.headers.insert(key, value)
        return self
    }
    
    fn body(mut self, body: string) -> HttpRequestBuilder {
        self.request.body = Some(body)
        return self
    }
    
    fn build(self) -> HttpRequest {
        return self.request
    }
}

// Usage
let request = HttpRequest::builder()
    .url("https://api.example.com/users")
    .method("POST")
    .header("Content-Type", "application/json")
    .body("{\"name\": \"Alice\"}")
    .build()
```

### State Machine

```myndra
enum ConnectionState {
    Disconnected,
    Connecting,
    Connected,
    Error(string)
}

struct Connection {
    state: ConnectionState,
    address: string
}

impl Connection {
    fn new(address: string) -> Connection {
        return Connection {
            state: ConnectionState::Disconnected,
            address
        }
    }
    
    fn connect(mut self) -> Result<(), string> {
        match self.state {
            ConnectionState::Disconnected => {
                self.state = ConnectionState::Connecting
                // Attempt connection
                if connection_successful() {
                    self.state = ConnectionState::Connected
                    return Ok(())
                } else {
                    self.state = ConnectionState::Error("Connection failed")
                    return Err("Failed to connect")
                }
            },
            _ => return Err("Cannot connect from current state")
        }
    }
    
    fn disconnect(mut self) {
        self.state = ConnectionState::Disconnected
    }
    
    fn is_connected(self) -> bool {
        return match self.state {
            ConnectionState::Connected => true,
            _ => false
        }
    }
}
```

### Observer Pattern with Context Awareness

```myndra
trait EventListener {
    fn on_event(self, event: Event)
}

struct Logger;
impl EventListener for Logger {
    fn on_event(self, event: Event) {
        if context == "dev" {
            print("DEBUG: Event occurred: {:?}", event)
        }
        
        if context == "prod" {
            log_to_file("events.log", event.to_string())
        }
    }
}

struct EventBus {
    listeners: Vec<Box<dyn EventListener>>
}

impl EventBus {
    fn new() -> EventBus {
        return EventBus { listeners: Vec::new() }
    }
    
    fn subscribe(mut self, listener: Box<dyn EventListener>) {
        self.listeners.push(listener)
    }
    
    fn emit(self, event: Event) {
        for listener in self.listeners {
            listener.on_event(event.clone())
        }
    }
}
```

---

This comprehensive programming guide covers the essential aspects of Myndra programming. As the language develops, more features and patterns will be added to this guide.

For the latest updates and examples, check the [examples/](../examples/) directory and the [language reference](language_reference.md).