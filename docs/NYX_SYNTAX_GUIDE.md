# 🌙 Nyx Programming Language — Complete Syntax Guide

Nyx is a modern, unified systems programming language designed for cross-platform application development (Desktop, WebAssembly, Mobile, and Cloud).

---

## 1. Program & Module Structure

Nyx uses explicit module declarations and import paths:

```nyx
module athena.strategy

import std.io
import std.vec
import std.map
import std.math
```

---

## 2. Variables & Constants

```nyx
// Immutable variable (default)
let price: f64 = 150.25

// Mutable variable
let mut portfolio_value: f64 = 10000.0
portfolio_value = portfolio_value + 250.50

// Constants
const MAX_POSITIONS: i32 = 10
```

---

## 3. Data Types & Collections

### Primitive Types
- `i32`, `i64`, `u32`, `u64`: Signed & unsigned integers
- `f32`, `f64`: Floating-point numbers
- `bool`: `true` / `false`
- `String`: UTF-8 heap-allocated text
- `char`: Unicode scalar value

### Vector (Resizable Array)
```nyx
let mut ticks = Vec.new()
ticks.push(150.25)
ticks.push(150.80)
let count = ticks.len()
```

### Map (Key-Value Hash Table)
```nyx
let mut holdings = Map.new()
holdings.insert("AAPL", 80.0)
holdings.insert("NVDA", 45.0)
```

---

## 4. Structs & Implementations

```nyx
pub struct Order {
    pub id: String,
    pub symbol: String,
    pub price: f64,
    pub qty: f64,
}

impl Order {
    pub fn new(id: String, symbol: String, price: f64, qty: f64) -> Order {
        Order {
            id: id,
            symbol: symbol,
            price: price,
            qty: qty,
        }
    }

    pub fn total_cost(self: &Order) -> f64 {
        self.price * self.qty
    }
}
```

---

## 5. Functions & Method Calls

```nyx
pub fn calculate_kelly_size(win_rate: f64, win_loss_ratio: f64) -> f64 {
    let q = 1.0 - win_rate
    let kelly = win_rate - (q / win_loss_ratio)
    kelly
}
```

---

## 6. C Foreign Function Interface (`extern "C"`)

Interoperating directly with native C libraries (DLL, SO, DyLib):

```nyx
extern "C" {
    fn sqlite3_open(path: *u8, db: **u8) -> i32
    fn sqlite3_exec(db: *u8, sql: *u8, cb: *u8, arg: *u8, err: **u8) -> i32
    fn sqlite3_close(db: *u8) -> i32
}
```

---

## 7. Control Flow & Pattern Matching

```nyx
// If-else expression
let status = if price > 150.0 {
    "BULLISH"
} else {
    "BEARISH"
}

// For loop
for i in 0..10 {
    println("Iteration: " + i.to_string())
}

// Match expression
match signal {
    1 => println("BUY"),
    2 => println("SELL"),
    _ => println("HOLD"),
}
```

---

## 8. Compiler Diagnostics & CLI Commands

```powershell
# Compile a file to Windows binary (.exe)
nyx-bootstrap my_file.nyx

# Compile and run immediately
cargo run --bin nyx-bootstrap -- my_file.nyx --run
```
