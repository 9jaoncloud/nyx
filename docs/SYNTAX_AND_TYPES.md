# 📐 Nyx Language Reference: Syntax, Types & Patterns

The Nyx syntax combines the clarity and readability of modern high-level languages with the explicit control required for bare-metal systems programming.

---

## 🔤 Primitive Types

| Type | Description | Machine Width |
| :--- | :--- | :--- |
| `Int` | Default signed architecture integer (`int64_t` on 64-bit platforms) | 64-bit |
| `Float` | IEEE 754 double-precision floating-point number (`double`) | 64-bit |
| `Bool` | Boolean truth value (`true` or `false`) | 8-bit |
| `Char` | Single UTF-8 unicode scalar value | 32-bit |
| `String` | Immutable UTF-8 slice backed by region memory | 16 bytes (ptr + len) |
| `U8`, `U16`, `U32`, `U64` | Explicit unsigned integer types | 8, 16, 32, 64-bit |
| `I8`, `I16`, `I32`, `I64` | Explicit signed integer types | 8, 16, 32, 64-bit |

---

## 🏗️ Variables & Mutability

Variables are immutable by default. Use `mut` to allow mutation:

```nyx
let speed: Float = 120.5;      // Immutable
let mut counter: Int = 0;       // Mutable
counter = counter + 1;
```

Type inference allows omitting types when obvious:
```nyx
let threshold = 500;            // Inferred as Int
let label = "Sensor A";         // Inferred as String
```

---

## 📦 Structs & Methods

Structs define custom data layouts with deterministic memory layout:

```nyx
struct Vector3 {
    x: Float,
    y: Float,
    z: Float
}

impl Vector3 {
    fn new(x: Float, y: Float, z: Float) -> Vector3 {
        Vector3 { x: x, y: y, z: z }
    }

    fn length_squared(self: &Vector3) -> Float {
        return (self.x * self.x) + (self.y * self.y) + (self.z * self.z);
    }

    fn scale(self: &mut Vector3, factor: Float) {
        self.x = self.x * factor;
        self.y = self.y * factor;
        self.z = self.z * factor;
    }
}
```

---

## 🔄 Enums & Pattern Matching

Enums support tagged algebraic data types (ADTs) and can carry typed payloads:

```nyx
enum TaskStatus {
    Pending,
    Running(Int),       // Carries percentage completed
    Failed(String),     // Carries error description
    Completed
}

fn describe_task(status: TaskStatus) {
    match status {
        TaskStatus::Pending => {
            println("Task is queued in nursery.");
        },
        TaskStatus::Running(pct) => {
            println("Execution underway: ", pct);
        },
        TaskStatus::Failed(err) => {
            println("Critical failure occurred: ", err);
        },
        TaskStatus::Completed => {
            println("Task completed successfully.");
        }
    }
}
```

---

## 🔁 Control Flow

### If / Else Conditionals
```nyx
if score >= 90 {
    println("Tier 1 Alpha");
} else if score >= 70 {
    println("Tier 2 Standard");
} else {
    println("Disqualified");
}
```

### Loops
```nyx
// While Loop
let mut i = 0;
while i < 10 {
    println("Tick: ", i);
    i = i + 1;
}

// For-in Collection Loop
let items = [10, 20, 30, 40];
for item in items {
    println("Element: ", item);
}
```

---

## 🧩 Standard Data Structures

* **`Vec<T>`**: Dynamic growable vector allocated within the current region arena:
  ```nyx
  let mut numbers = Vec::new();
  numbers.push(42);
  numbers.push(99);
  println("First item: ", numbers.get(0));
  ```

* **`Option<T>`**: Explicit representation of optional values without `null` pointer dereferences:
  ```nyx
  let val: Option<Int> = Some(100);
  match val {
      Some(n) => println("Value: ", n),
      None => println("Missing value")
  }
  ```

* **`Result<T, E>`**: Explicit error propagation without heavy exception unwinding:
  ```nyx
  fn divide(a: Float, b: Float) -> Result<Float, String> {
      if b == 0.0 {
          return Err("Division by zero");
      }
      return Ok(a / b);
  }
  ```
