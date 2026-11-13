The Nyx Language

A Complete Book — from first program to sovereign systems

Edition 1.0 · Verified against Nyx v0.53.0 (nyx.exe, Windows x86_64) Author of the language: Simeon Bala — 9jaonCloud Engineering This book: a single, self-contained, printable guide. Read it top to bottom and you will know the language.

How this book was written (and why you can trust the code in it)

Every code example in this book was compiled and executed with the real compiler before it was printed here. Where the language behaves in a surprising way, this book says so and shows you the actual compiler message. The verification was done on:

$ .\nyx.exe --version
nyx 0.53.0

Examples were run with:

.\nyx.exe -i my_program.nyx -r

If a fact is marked [verified], it was observed on 2026-10-22 on Nyx v0.53.0. If something is marked [trap], it means the real compiler does not do what the older docs claimed, and this book tells you the truth so you don't lose a day to it.

A note on the two dialects. Nyx has grown two styles of code: the runnable dialect (small, standalone programs you compile directly — what beginners should use) and the modular dialect (library files with module, pub, and typed Vec[T]). This book teaches the runnable dialect in Parts I–VIII, then explains the modular dialect in Chapter 15. Do not mix them in one file until you understand the difference. Mixing them is the single biggest source of beginner confusion.

Printing this book

You can print this Markdown file directly, or convert it to PDF:

# Option A — VS Code: install "Markdown PDF" and Ctrl+Shift+P → "Markdown PDF: Export (pdf)"
# Option B — Pandoc (nicest typography):
pandoc NYX_LANGUAGE_BOOK.md -o NYX_LANGUAGE_BOOK.pdf --toc --number-sections

# Option C — Word (.docx) if you prefer editable:
pandoc NYX_LANGUAGE_BOOK.md -o NYX_LANGUAGE_BOOK.docx --toc --number-sections

Table of Contents

Part I — Foundations

Why Nyx Exists

The Toolchain: Compiling and Running

Your First Program

Part II — The Core Language

Variables, Types, and Mutability

Expressions and Operators

Making Decisions

Loops

Functions

Part III — Structuring Data

Structs and Methods

Enums and Pattern Matching

Modelling Failure

Arrays and Collections

Strings and Text

Part IV — Organising Code

Comments, Constants, and Style

Modules, Imports, and the Two Dialects

Part V — Memory and Safety

The Zero-GC Memory Model

The Optional Safety Dialects

Part VI — Concurrency

Nurseries and Channels

Part VII — Systems and Integration

Calling C (Foreign Function Interface)

GUIs, Web, Mobile, and the C Bridge

Part VIII — Mastery

Traps, Bugs, and Decoded Errors

Capstone: Build a Ledger CLI

Appendices A. Command-Line Reference B. Type Reference C. Error Message Decoder D. Verified Examples Index E. Glossary

Part I — Foundations

1. Why Nyx Exists

Every systems language for the last forty years has forced the same painful choice, which engineers call the systems programming trilemma:

Manual memory (`malloc`/`free` in C, `new`/`delete` in C++). Blisteringly fast, but the source of the majority of critical memory-safety bugs in history: use-after-free, double-free, buffer overflows.

Garbage collection (Go, Java, C#). Safe and easy, but the collector stops your program without warning. A 5–100 ms pause is fatal for a trading engine, an audio callback, or a robot motor loop.

Lifetimes and borrow checking (Rust). Safe without a collector, but you pay with a heavy cognitive tax — annotating lifetimes, fighting the borrow checker, and wrestling with self-referential structures.

Nyx proposes a fourth answer: Automatic Static Region Inference. You write clean, annotation-free code. The compiler figures out where every value should live, groups values into deterministic memory regions, and reclaims whole regions in constant time when they go out of scope. The result is safety like Rust, pause behaviour like C, and ergonomics like a scripting language.

Three pillars define the language:

Zero-GC determinism. No runtime garbage collector, no malloc/free in your code. Allocation is a monotonic bump of a region pointer; reclamation is a region reset. There is no per-object free, so there is no heap fragmentation.

Native performance. Nyx lowers to C, which GCC or Clang then optimizes with decades of vectorization (AVX2, AVX-512, NEON), loop unrolling, and instruction scheduling. From C you inherit every architecture those compilers support: x86_64, AArch64, ARM Cortex-M, RISC-V, and WebAssembly.

Capability-based security. Fine-grained token membranes (CAP_NET, CAP_FILE, CAP_MEM, CAP_SPAWN, CAP_SENSORY) let you grant a component exactly the authority it needs and nothing more.

Nyx is used to build high-frequency trading engines, the NyxOS microkernel, AI firewalls, embedded storage engines, and a national governance grid. But you don't need any of that to start. You need a text editor and nyx.exe.

2. The Toolchain: Compiling and Running

The compiler is a single executable, nyx.exe. It compiles a .nyx file, optionally runs it, and can show you the C it generates.

The one command you need

.\nyx.exe -i path\to\program.nyx -r

-i is the input source file.

-r means compile and run immediately.

That's it. When you run it, you'll see the compiler report that it produced an executable, followed by your program's output:

Compiled: path\to\program.exe
hello
calc: 42

[verified] The .exe is written next to your .nyx source. Run it again without editing and it will overwrite the previous build.

Compiling without running

# Build an executable only
.\nyx.exe -i program.nyx -o program.exe

# Build with maximum optimization
.\nyx.exe -i program.nyx -o program.exe -O 3

# See the C the compiler generated (great for learning / auditing)
.\nyx.exe -i program.nyx --emit-c

The full command-line surface

-i, --input <INPUT>          Path to the entry source file (.nyx)
-o, --output <OUTPUT>        Path to the output executable or library
-r, --run                    Compile and execute immediately
-g, --debug                  Generate debugging symbols (DWARF/PDB)
-O, --opt-level <LEVEL>      Optimization level: 0, 1, 2, 3, or 's' (default: 2)
    --emit-c                 Emit optimized transpiled C intermediate code
-S, --emit-asm               Emit native assembly
    --emit-llvm              Emit LLVM IR
    --target <TARGET>        Cross-compilation target triple
-j, --jobs <JOBS>            Parallel worker threads (default: 1)
-d, --dialect <DIALECT>      Dialect selection: auto, base, zephaniah, havilah, jude
-h, --help                   Print help information
-V, --version                Print compiler version

Optional: put it on your PATH

# Current PowerShell session only
$env:Path += ";$PWD\bin"

# Now you can just type:
nyx -i main.nyx -r

Checkpoint. You should be able to run .\nyx.exe --version and see nyx 0.53.0. If you can, the rest of this book is a straight path.

3. Your First Program

Create a file called hello.nyx:

fn main() {
    println("hello");
}

Compile and run:

.\nyx.exe -i hello.nyx -r

Output [verified]: hello

Congratulations — you just wrote, compiled, and ran a native program. Let's look at each piece.

fn main() { ... } declares a function named main. Every executable needs exactly one main; it is where your program begins.

println(...) prints a line of text to the console. It is variadic — it accepts one or more values separated by commas.

The "..." is a string literal.

Making main return an exit code

A program can optionally return an integer status code. 0 means success.

fn add(a: Int, b: Int) -> Int {
    return a + b;
}

fn main() -> Int {
    let s: Int = add(2, 3);
    println("sum: ", s);
    return 0;
}

Output [verified]:

sum: 5

Both forms of main are valid — with and without a return type. Use the plain form for learning programs, and the -> Int form when a shell or script needs your exit status.

println in detail

println prints each argument, converts numbers to text for you, and ends with a newline.

fn main() {
    let x: Int = 21;
    let y: Int = 2;
    println("calc: ", x * y);   // prints:  calc: 42
    println("hello");           // prints:  hello
}

Notice two different ways to build output:

Comma style (preferred for mixed values): println("calc: ", x * y). Numbers are printed directly, no conversion needed.

Concatenation style: println("n=" + n.to_string()). Here you must convert the number with .to_string() because + only joins strings to strings.

[trap] Nyx has no ${variable} string interpolation. Writing println("n=${n}") prints the characters literally. Convert and concatenate, or use commas.

Exercises.

Print your name on one line and your favourite number (as an Int) on the next.

Write a main that returns 0 and a second function square(n: Int) -> Int that returns n * n. Print square(9).

Part II — The Core Language

4. Variables, Types, and Mutability

A variable is a named box holding a value. In Nyx you create one with let. By default the box is immutable — read-only. To make it changeable, add mut.

let price: Int = 150;        // immutable
let mut total: Int = 0;      // mutable
total = total + 25;          // allowed because of 'mut'

Trying to reassign an immutable variable is a compile error — a deliberate safety feature. If you find yourself needing mut, ask whether you actually do; most values never need to change.

Type annotations and inference

You can write the type after a colon, or let the compiler infer it:

let explicit: Int = 42;      // explicit
let inferred = 42;           // inferred as Int
let label = "Sensor A";      // inferred as String
let enabled: Bool = true;    // explicit

For clarity, annotate at least the public edges of your code. It also prevents a whole class of bug — see floats below.

The primitive types



[verified] Both Float and f64 compile and behave as 64-bit doubles. Use Float for readability or f64 if you're matching library signatures — they are interchangeable in practice.

The float trap (read this twice)

Integer division truncates. Floats need to be floats.

fn main() {
    let a: Float = 48.0 / 24.0;
    println("Float ratio: ", a);   // 2.000000
    let c = 7 / 2;                 // Int / Int
    println("integer-division: ", c);  // 3  — the .5 is gone!
}

Output [verified]:

Float ratio: 2.000000
integer-division: 3

[trap] If you write let r = 7 / 2; you get 3, not 3.5. Annotate one side as Float (7.0 / 2.0) whenever you want fractional math. Also note that floats print with six decimal places by default: 4.0 prints as 4.000000.

Exercise. Compute and print the average of 10, 20, 31 as a float. What do you get if you forget the float annotation?

5. Expressions and Operators

An expression produces a value; a statement does something. In Nyx, almost everything is an expression — including if and match, as you'll see.

Arithmetic

1 + 2      // 3
10 - 4     // 6
6 * 7      // 42
7 / 2      // 3   (integer division — truncates)
7 % 2      // 1   (remainder / modulo)

+ also joins strings, but only string-to-string. Convert numbers first with .to_string().

Comparison and logic

Comparisons produce a Bool:

x == y   // equal
x != y   // not equal
x > y    // greater than
x >= y   // greater or equal
x < y    // less than
x <= y   // less or equal

Boolean operators combine Bool values:

a && b   // AND — true only if both are true
a || b   // OR  — true if either is true
!a       // NOT — flips true/false

A complete, working example [verified]:

fn is_even(n: Int) -> Bool {
    if n % 2 == 0 { true } else { false }
}

fn main() {
    let a: Bool = true;
    let b: Bool = false;

    if a && b { println("both"); } else { println("not both"); }
    if !b { println("not b is true"); }
    if a || b { println("either"); }

    if is_even(4) {
        println("4 is even");
    }
}

Output [verified]:

not both
not b is true
either
4 is even

Notice is_even has no return keyword: the last expression of a function body (if ... { true } else { false }) is the return value. This "everything is an expression" style is idiomatic Nyx.

Operator precedence

Follow the usual arithmetic order: ! first, then * / %, then + -, then comparisons, then &&, then ||. When in doubt, add parentheses — they cost nothing and prevent silent bugs.

6. Making Decisions

if runs a block only when a condition is true, and can chain with else if and else.

fn grade(score: Int) -> String {
    if score >= 90 {
        "A"
    } else if score >= 70 {
        "B"
    } else {
        "C"
    }
}

Output [verified] for 95, 75, 50: A, B, C.

Two important facts:

`if` is an expression. The whole if/else if/else above evaluates to a value, which is why the function can simply return it. You can assign it: let tier = if n > 10 { "big" } else { "small" };

Conditions are booleans. In Nyx v0.53.0 you may write either if flag or if flag == true — both compile. Writing the explicit == true form is still recommended in shared code because it reads unambiguously and matches the style of the library dialect.

[verified — correction to older docs] Some older Nyx guides claim if flag { } is rejected by the compiler and that you must always write if flag == true { }. That is not true for v0.53.0: the plain form compiles and runs. Prefer explicit comparisons for readability, but you will not get an error for omitting them.

Truthiness

fn main() {
    let flag: Bool = true;
    if flag {
        println("implicit truthiness allowed");
    }
}

Output [verified]: implicit truthiness allowed

Exercise. Write fn sign(n: Int) -> String that returns "positive", "negative", or "zero".

7. Loops

while — repeat while a condition holds

fn main() {
    let mut i = 0;
    while i < 3 {
        println("tick ", i);
        i = i + 1;
    }
}

Output [verified]:

tick 0
tick 1
tick 2

Note the shape of every while loop you'll ever write:

initialise the counter (let mut i = 0),

test it (i < 3),

change it inside the body (i = i + 1).

Forget step 3 and you have an infinite loop.

for i in start..end — a counting loop

When you know how many times you want to repeat, a range is clearer than a counter:

fn main() {
    for i in 0..3 {
        println("i ", i);
    }
}

Output [verified]:

i 0
i 1
i 2

The range 0..3 is exclusive at the top — it yields 0, 1, 2. This is the idiomatic way to walk a fixed-size array by index (see Chapter 12).

[trap] Do not write for item in someArray { ... } over an array literal. In v0.53.0 that compiles to broken C and the compiler reports unknown type name 'TopLevel'. Iterate with an index and a while loop instead (Chapter 12 shows the exact pattern).

Exercise. Print the numbers 1 through 10, then print only the even ones.

8. Functions

A function packages a computation under a name so you can reuse it. The shape is:

fn name(param: Type, param2: Type) -> ReturnType {
    // body
    return value;
}

fn add(a: Int, b: Int) -> Int {
    return a + b;
}

fn main() {
    println("sum: ", add(2, 3));   // sum: 5
}

The two ways to return

fn with_keyword(x: Int) -> Int {
    return x * 2;          // explicit return
}

fn with_expression(x: Int) -> Int {
    x * 2                  // last expression IS the return value
}

Both are fine. Use return when it improves clarity or you're returning early; use the trailing-expression form for short functions.

Functions on values (methods)

You've already seen methods: n.to_string(), v.len(), acc.deposit(...). Those are functions attached to a type. You'll define your own in Chapter 9.

Assertions — writing your own tests

The runtime provides assert_true(condition). If the condition is false, the program fails loudly. Use it to prove your functions work.

fn main() {
    let result = add(2, 3);
    assert_true(result == 5);      // passes silently
    println("all assertions passed");
}

[verified] assert_true(result == 5) passes and prints nothing on success — exactly what you want in a test.

Exercise. Write fn max_of(a: Int, b: Int) -> Int and test it with assert_true for the cases (3, 7), (7, 3), and (5, 5).

Part III — Structuring Data

9. Structs and Methods

A struct bundles related values into one named thing. Where a variable holds one value, a struct holds a record.

struct Account {
    owner: String,
    balance: Int,
}

This says: an Account has an owner (text) and a balance (integer). Trailing commas in the field list are fine.

Creating a struct

let a = Account { owner: "Ada", balance: 1000 };

Attaching behaviour with impl

An impl block adds functions ("methods") to a struct:

impl Account {
    fn new(owner: String, opening: Int) -> Account {
        Account { owner: owner, balance: opening }
    }

    fn deposit(self: &mut Account, amount: Int) {
        self.balance = self.balance + amount;
    }

    fn withdraw(self: &mut Account, amount: Int) -> Bool {
        if amount > self.balance {
            false
        } else {
            self.balance = self.balance - amount;
            true
        }
    }
}

Read the receivers carefully — this is where Nyx's memory discipline shows up:

self: &Account — a read-only reference. The method may look at the struct but not change it.

self: &mut Account — a mutable reference. The method may change fields.

self: Account — takes the struct by value.

A method that changes state (deposit, withdraw) needs &mut. A method that only reads needs &. Getting this right is the whole point of the region model: the compiler can now prove that nothing else is mutating the value while you hold the reference.

Constructors by convention

Nyx has no special new keyword, but by convention a type offers a new function in its impl. You call it with a dot on the type name:

let mut acc = Account.new("Ada", 1000);
acc.deposit(500);

[verified] This "type-dot" constructor call works: Account.new("Ada", 1000) builds the struct and returns it.

A complete, verified program

struct User {
    id: Int,
    name: String,
}

impl User {
    fn new(id: Int, name: String) -> User {
        User { id: id, name: name }
    }

    fn label(self: &User) -> String {
        self.name + " (#" + self.id.to_string() + ")"
    }
}

fn main() {
    let u = User.new(1, "Ada");
    println(u.label());
}

Output [verified]: Ada (#1)

Naming rule when you transpile to C

Nyx compiles through C. Names that collide with C keywords or with the C runtime can break the build. [trap] Avoid naming your own methods or fields get, default, switch, case, sizeof, or new-like C builtins. When in doubt, prefix: lookup_key, fetch_value, initialize_defaults.

[trap] Likewise, if you write your own math helpers, prefix them (calc_sin, calc_sqrt) to avoid clashing with C's <math.h> symbols.

Exercise. Add a rename(self: &mut User, name: String) method that changes the user's name, and prove with assert_true that it changed.

10. Enums and Pattern Matching

An enum is a type whose value is exactly one of a fixed set of variants. It is how Nyx models "this thing is one of these possibilities".

enum Color {
    Red,
    Green,
    Blue,
    Custom(Int, Int, Int),
}

The first three variants carry nothing. Custom carries data — three integers. An enum variant may carry as much typed data as you like, which makes enums a way to describe structured alternatives (algebraic data types).

match — the companion to enum

match examines a value and runs the arm that fits:

fn main() {
    let c = Color.Green;
    let name = match c {
        Color.Red => "red",
        Color.Green => "green",
        Color.Blue => "blue",
        Color.Custom(r, g, b) => "custom",
    };
    println(name);
}

Output [verified]: green

Points to internalize:

A variant is named EnumName.Variant — a dot, not ::.

Arms are separated by commas. Each arm is pattern => value.

Color.Custom(r, g, b) binds the carried values to r, g, b for use in the arm.

The whole match is an expression — it produces the value assigned to name. This is why name didn't need a return.

Matching on integers and the wildcard

match works on any type, and _ means "anything else":

let s = 2;
let name = match s {
    1 => "one",
    2 => "two",
    _ => "many",
};

Output [verified]: two

The _ arm is essential: the compiler needs to know every possibility is covered. Using _ as a catch-all satisfies that.

Block arms

When an arm needs more than one statement, wrap it in braces:

match self.op {
    CalcOp.Add => { res = self.previous + current; },
    CalcOp.Subtract => { res = self.previous - current; },
    _ => { res = 0; },
}

(This exact pattern is used in examples/calculator.nyx in the repo.)

Exercise. Define enum Direction { North, South, East, West } and write fn turn_left(d: Direction) -> Direction using match.

11. Modelling Failure

Beginners coming from other languages look for null, nil, or exceptions. Nyx's idiom is an enum that carries the outcome. The most common is a two-variant Result-shaped enum:

enum Outcome {
    Ok(Int),
    Err(String),
}

Now a function that can fail returns Outcome, and the caller is forced by match to handle both cases. There is no way to forget the error path.

enum Outcome {
    Ok(Int),
    Err(String),
}

fn safe_div(a: Int, b: Int) -> Outcome {
    if b == 0 {
        return Outcome.Err("division by zero");
    }
    return Outcome.Ok(a / b);
}

fn main() {
    let r = safe_div(10, 2);
    match r {
        Outcome.Ok(v) => println("ok ", v),
        Outcome.Err(e) => println("err ", e),
    }
}

Output [verified]: ok 5

Why this matters

Compare the two worlds:

In C, a failing function sets errno or returns -1; nothing stops you from ignoring it, and the bug surfaces far away from the cause.

In Nyx, Outcome is an enum. The only way to get the value out is to match, and match must name every variant. The error cannot be silently skipped.

The generic forms you'll meet in libraries

The standard library generalises this to Result<T, E> (success carrying T, failure carrying E) and Option<T> (a value or nothing). In the modular dialect you'll see them written Result.Ok(v), Result.Err(e), Option.Some(v), Option.None.

[trap — verified advice] When you define your own result-like enums in the runnable dialect, prefer custom variant names such as Ok/Err or Good/Bad rather than Some/None. Variants literally named Some and None have historically tripped the type inference in this compiler. Custom names always work — the Outcome example above is proof.

Exercise. Write fn lookup(id: Int) -> Outcome that returns Ok(id * 10) for ids above zero and Err("bad id") otherwise, then handle both branches.

12. Arrays and Collections

Fixed-size arrays — the workhorse

A fixed-size array is written with square brackets:

let items = [10, 20, 30];

You can read its length, and read elements by index:

fn main() {
    let items = [10, 20, 30];
    println("first ", items[0]);    // first 10
    println("second ", items[1]);   // second 20

    let mut i = 0;
    while i < 3 {
        println("elem ", items[i]);
        i = i + 1;
    }
    println("len ", items.len());
}

Output [verified]:

first 10
second 20
elem 10
elem 20
elem 30
len 3

This is [verified] fully working: indexing items[i], .len(), and the index-based while loop all behave exactly as you'd expect. Learn to iterate arrays this way.

The canonical array loop

let scores = [88, 92, 79, 95];
let mut total = 0;
let mut i = 0;
while i < scores.len() {
    total = total + scores[i];
    i = i + 1;
}
println("total ", total);          // total 354

Use i < scores.len() rather than a hard-coded bound so the loop stays correct when the array changes size.

Growable vectors — and their current limits

Nyx also has a growable list, Vec, for when the size isn't known up front. Creating and appending works:

fn main() {
    let mut v = Vec.new();
    v.push(1);
    v.push(2);
    println("len: ", v.len());     // len: 2
}

Output [verified]: len: 2

So Vec.new(), .push(x), and .len() all work.

[trap — important, verified] In v0.53.0 the runnable dialect cannot yet read elements back out of a Vec reliably. Each of the following compiles but is wrong or breaks:



Practical rule: while learning, store your sequences in fixed-size arrays and index them. Use Vec only for build-up (push/len) until the element-read path is fixed. This book teaches arrays as the primary collection for that reason.

Deterministic pseudo-random numbers

There is no hidden global RNG you must seed; a deterministic formula is often enough and is fully reproducible:

let next = (prev * 7 + 13) % 100;

This kind of integer "hash" appears throughout the repo's games (see examples/snake.nyx) because it runs identically on every machine — essential when you need reproducible behaviour.

Exercise. Given let data = [4, 8, 15, 16, 23, 42];, compute the maximum value using a while loop and assert_true that it is 42.

13. Strings and Text

A String is UTF-8 text. Two operations cover 95% of your needs: joining and converting.

Joining

+ concatenates two strings:

let greeting = "Hello, " + "world";
println(greeting);                 // Hello, world

To put a number inside a string, convert it first:

let n: Int = 5;
println("n=" + n.to_string());     // n=5

Output [verified]: n=5

[trap] + will not glue a number onto a string. "n=" + n is a type error — the compiler stops you. Options: n.to_string() (concat style) or println("n=", n) (comma style). Prefer the comma style when you're just printing, and .to_string() when you're building a string to store or return.

Building strings inside methods

This is the pattern used by label() in Chapter 9:

self.name + " (#" + self.id.to_string() + ")"

Read it left to right: name, then " (#", then the id as text, then ")".

No interpolation

println("value=${n}");     // prints literally: value=${n}   ← NOT what you want

[trap] Nyx does not support ${...} interpolation. There is no f"..." string. Always convert-and-concatenate or use commas.

Exercise. Build a receipt line: "Item: Ledger, Qty: 3, Total: 450" where 3 and 450 come from Int variables.

Part IV — Organising Code

14. Comments, Constants, and Style

Comments

// A single-line comment.
// Comments start with two slashes and run to the end of the line.

Use comments to explain why, not what. i = i + 1;  // add one to i is noise; i = i + 1;  // step the cursor past the delimiter we just consumed is signal.

Constants

The cleanest way to name a fixed value in the runnable dialect is a let inside the function that uses it:

fn main() {
    let max_retries: Int = 3;
    println("max retries ", max_retries);
}

Output [verified]: max retries 3

[trap — verified] A top-level const MAX: Int = 3; in the runnable dialect does not work in v0.53.0: the compiler reports 'MAX' undeclared. Use a let inside the scope instead, or place constants inside a module (Chapter 15). This is one of the concrete differences between the two dialects.

Style guidelines

Declare variables as close as possible to where they're used.

Annotate types at function boundaries and on floats.

Prefer arrays + while for iteration today (see Chapter 12).

One concept per function. If you can't name it in a few words, split it.

Keep main thin: it should read like a summary of what the program does.

15. Modules, Imports, and the Two Dialects

This is the chapter that saves you the most pain. Nyx code exists in two dialects, and they are not interchangeable within one file.

Dialect A — the runnable dialect (what you've been writing)

Small, standalone programs compiled directly with nyx.exe -i file.nyx -r.

May start with a module a.b line, or omit it entirely. [verified] A module declaration is accepted for a standalone executable:

  module examples.p04_module

  fn main() {
      println("module declaration accepted");
  }

Output [verified]: module declaration accepted

Uses Int, Float, Bool, String.

Structs and enums may be plain (no pub).

Uses fixed arrays; Vec[T] type syntax is not accepted.

Used by everything in examples/ and by this book.

Dialect B — the modular dialect (library files)

Large, reusable library files under std/, sdk/, and projects/. These are namespaced modules with explicit visibility.

module std.apps.snake

import std.collections
import std.math

pub enum Direction {
    Up,
    Down,
    Left,
    Right,
}

pub struct Point {
    pub x: Int,
    pub y: Int,
}

(Reproduced from the repo's examples/snake.nyx.) The differences from Dialect A:



The golden rule

Match the dialect to the folder. Standalone executables and tests (examples/, tests/) use Dialect A. Library modules (std/, sdk/, projects/) use Dialect B. Never blend Vec[Point] type annotations into a runnable program, and don't expect a pub-less struct to be importable from a library.

What import does

In Dialect B, import std.collections; makes another module's public items available. The path mirrors the module name. In the runnable dialect you generally don't need imports for the basics — println, Vec, assert_true, and sha256_hex are available to you.

A note on sha256_hex

The runtime provides sha256_hex(String) -> String, useful for content hashing and integrity checks:

let digest: String = sha256_hex("data_to_hash");

[not exercised in this book's runnable probes] — treat it as available; verify in your own environment before relying on output values.

Exercise. Take the Account struct from Chapter 9 and write a module ledger.account line above it. Confirm it still compiles with -r.

Part V — Memory and Safety

16. The Zero-GC Memory Model

You have been using the memory model all along without knowing it. lets, arrays, and structs live in regions. The compiler decides, at compile time, which region each value belongs to and when the whole region is released. You never call free, and there is no collector thread.

How a region works

Think of a region as a stack of sticky notes. "Allocating" is adding a new note on top — a single pointer increment (arena_ptr += size), which is about the fastest memory operation a CPU can do. "Freeing" is peeling the whole pad back to a previous point — also a single pointer reset. There is no per-object free, so there is no fragmentation and no bookkeeping.

fn average() -> Float {
    // The array below is allocated in this function's region.
    let readings = [10.0, 20.0, 30.0, 40.0];

    let mut sum: Float = 0.0;
    let mut i = 0;
    while i < readings.len() {
        sum = sum + readings[i];
        i = i + 1;
    }

    return sum / 4.0;   // when average() returns, its region is reset in O(1)
}

Escape analysis

When a value must outlive the function that created it — because you return it, or store it somewhere longer-lived — the compiler performs escape analysis and promotes the allocation into the caller's region. You write nothing special; the promotion is automatic. This is why the region model needs no lifetime annotations like Rust's 'a.

What this buys you

Deterministic latency. No collector means no pause spikes. The pauses you might expect in a managed language simply do not occur.

No fragmentation. Monotonic bump allocation never leaves holes.

No leaks in the C sense. Regions are reset as a whole; there is no partial-free bug class.

You write less. No malloc, no free, no 'a, no Arc<Mutex<…>> just to share data.

Your one rule

Never write manual memory management in Nyx: no malloc, no free, no delete, no new. There is no such thing in the language — if you find yourself reaching for it, you are fighting the model, and the model will win. Let the compiler place your values.

Where the cost shows up

The trade-off of region memory is that a region lives until its scope ends, not until the last object in it is unused. For long-running loops that allocate on every iteration, the answer is sub-region checkpoints: the runtime can checkpoint and rewind a sub-region so steady-state memory stays flat while you stream millions of records. You'll meet this in the standard library's streaming APIs. For ordinary programs, the scope is small enough that it never matters.

17. The Optional Safety Dialects

The base language (the one this book teaches) handles ordinary systems and application work. For mission-critical code you can opt into one of three formal dialects, selected with -d on the command line:

.\nyx.exe -i safety_critical.nyx -o app.exe -d havilah

`-d zephaniah` — Gradual Ownership. Enforces move semantics and borrow tracking for data shared across threads, proving the absence of data races.

`-d havilah` — Linear Capability Tokens & Higher RAII. Proves that physical resources — file descriptors, sockets, GPU pipelines, DMA buffers — cannot be leaked, forgotten, or used out of order. A resource must be consumed by its designated closing operation.

`-d jude` — SMT / Z3 Formal Verification. Integrates a theorem prover to verify integer-overflow bounds, array-index boundaries, and state-machine transitions before machine code is emitted.

Add these dialects exactly where the stakes justify the extra proof obligations. Base Nyx remains the default (-d auto) and the right choice for the vast majority of code.

Part VI — Concurrency

18. Nurseries and Channels

Nyx's concurrency is structured. Instead of detached threads that can outlive the function that started them (a classic source of leaks and dangling pointers), tasks are spawned inside a lexically scoped nursery. The nursery cannot exit until every task inside it has finished.

fn parallel_computation() {
    nursery n {
        n.spawn(|| {
            println("Sensor 1 sampling...");
        });

        n.spawn(|| {
            println("Sensor 2 sampling...");
        });
    }
    // Reached only after BOTH tasks have cleanly finished.
    println("All sensor tasks synchronized.");
}

Guarantees this gives you:

No thread leaks. A parent cannot return while a child is running.

Deterministic cancellation. If one task fails, siblings are signalled to shut down in an orderly way.

Region scoping. Each task inherits a lightweight sub-region that is reclaimed automatically on exit.

Typed channels

Tasks talk to each other over bounded FIFO channels. Channel::bounded(n) returns a sender and a receiver:

fn channel_demo() {
    let (sender, receiver) = Channel::bounded(128);

    nursery n {
        n.spawn(move || {
            let mut i = 0;
            while i < 5 {
                sender.send(i);
                i = i + 1;
            }
        });

        n.spawn(move || {
            let mut received = 0;
            while received < 5 {
                let msg = receiver.recv();
                println("Received tick: ", msg);
                received = received + 1;
            }
        });
    }
}

The move keyword tells the closure to take ownership of the values it uses (sender, receiver), so the channel ends up with exactly one owner per end — a Single-Producer / Single-Consumer pattern enforced by the compiler.

Dialect note. The nursery and channel examples above are written in the modular dialect and are documented from the repo's concurrency guide rather than probed in this book's runnable harness. Use them as the design, and compile them under a library module (module) with the appropriate imports.

Ultra-low-latency queues

For the tightest loops — trading, audio DSP, motor control — the standard library provides lock-free, cache-line-aligned ring buffers with no syscalls and no mutexes. The public benchmark figure for the SPSC queue is a 42 ns median on the project's reference hardware (Intel Core i7-6700HQ @ 2.60 GHz); see docs/CONCURRENCY.md and the benchmark suite for the full methodology and tail figures. Reproduce it yourself before quoting it — this book makes no performance promises, only points you at the harness.

Part VII — Systems and Integration

19. Calling C (Foreign Function Interface)

Nyx can call any C library with no glue code, because it lowers to C. You declare the external functions with extern "C" and talk to them with raw pointers.

extern "C" {
    fn sqlite3_open(path: *u8, db: **u8) -> i32
    fn sqlite3_exec(db: *u8, sql: *u8, cb: *u8, arg: *u8, err: **u8) -> i32
    fn sqlite3_close(db: *u8) -> i32
}

Declarations have no body — they promise the linker that the symbol exists. Then you link the C library (a .dll, .so, .dylib, or static archive) at build time.

[trap — verified in repo notes] Calling GUI functions such as the Win32/rt_ui window API directly from .nyx is currently blocked by a codegen mismatch (*const u8 is emitted as int64_t* instead of char*, which breaks string arguments). The working pattern is the C bridge: keep your pure-Nyx engine and state machine in .nyx, and write the window in a small .c file linked against the prebuilt runtime archive. Chapter 20 shows the layout.

Safety note. FFI is the one place you step outside the region model's protection. Treat every pointer from C as untrusted until you have validated it. This is a system boundary — validate inputs here.

20. GUIs, Web, Mobile, and the C Bridge

There are three routes to a graphical application, and they differ in trust level today:



The decoupled architecture (the pattern the repo uses)

Keep the logic and the presentation in separate layers:

Logic (pure Nyx): your state machine, math, and rules live in a .nyx file (e.g. examples/scientific_calculator.nyx). It compiles and runs anywhere, and it's what you unit-test.

Native bridge (C): a small .c file (e.g. calculator_gui.c) drives the window and calls the prebuilt runtime (lib/libnyxrt.a) for drawing and input.

Web shell (HTML/Canvas): os-preview.html is the design of record for the OS-style UI. Other targets aim to reproduce it as-is.

The native frame loop

If you take the native-Win32 route, the loop shape is fixed and order matters:

void* win = nyx_window_create(WW, WH, "My App");
void* canvas = nyx_window_canvas(win);
while (!nyx_window_should_close(win)) {
    nyx_window_poll_events(win);       // 1. drain OS messages
    nyx_window_update_click(win);      // 2. edge-detect this frame's click
    if (nyx_window_mouse_clicked(win)) {
        int hit = nyx_hit_test((float)nyx_window_mouse_x(win),
                               (float)nyx_window_mouse_y(win));
        handle_press(hit);             // 3. act on the click
    }
    nyx_hit_begin_frame();             // 4. reset hit rects
    render(canvas);                    // 5. draw, and re-register widgets
    nyx_window_swap_buffers(win);      // 6. present (double-buffered, no flicker)
}
nyx_window_destroy(win);

Two hard-won details from the repo's calculator work:

Build path. On Windows the C bridge must be compiled with MSYS2's mingw64 bin and lib on PATH; otherwise gcc exits 1 with zero diagnostics, even for a trivial file.

UX rule. A calculator must keep the running expression visible (1 + 1 =) and show the result after =. Build UIs that follow the conventional behaviour of the app type — a correct-looking UI that breaks a familiar convention still reads as a bug to a user.

WebAssembly and mobile

Because Nyx targets the same architectures GCC does, the same program can be built for WebAssembly (nyx_mobile_core.wasm in the repo) and for mobile via the compatibility layer under templates/ and nyxmobile/. The web preview (os-preview.html) is the reference for how those UIs should look.

Part VIII — Mastery

21. Traps, Bugs, and Decoded Errors

This chapter is the most valuable one in the book. Every entry was reproduced on v0.53.0. Bookmark it.

Category 1 — Syntax that the parser rejects



Category 2 — Code that compiles but lies (the dangerous ones)



The rule that keeps you safe: store sequences in fixed-size arrays and read them by index. That path is fully verified and does exactly what it says. Reach for Vec only when you must grow a collection dynamically, and don't read its elements back until the element-read path is fixed.

Category 3 — Correct syntax, wrong result



Category 4 — Names that collide with C

Because Nyx transpiles through C, avoid naming your own items after C keywords or the C runtime: get, default, switch, case, sizeof. Prefix math helpers (calc_sin, calc_sqrt) to avoid <math.h>.

Category 5 — Build environment

On Windows, compiling a C bridge needs MSYS2 mingw64 bin and lib on PATH. Without them, gcc fails silently.

Floats must be annotated : Float when you need fractional arithmetic; unannotated arithmetic can truncate.

How to debug in Nyx

Shrink the program. Delete code until it compiles, then add back until it breaks. The last thing you added is the culprit.

Isolate the construct. If you suspect for x in v, write a five-line file with only that. This book's probes were each five to twenty lines for exactly this reason.

Read the C. Run --emit-c and open the generated file. When a construct misbehaves, the generated C usually shows you precisely why.

Assert your assumptions. assert_true turns a silent wrong answer into a loud crash at the exact line.

Check the dialect. Half of all "impossible" errors are Dialect A code mixed into Dialect B or vice versa (Chapter 15).

22. Capstone: Build a Ledger CLI

Let's combine everything — structs, methods, enums, pattern matching, arrays, loops, strings, and assertions — into one program. Below is the complete source, and its real output from v0.53.0.

// Capstone — Ledger CLI (verified runnable dialect)

struct Account {
    owner: String,
    balance: Int,
}

impl Account {
    fn new(owner: String, opening: Int) -> Account {
        Account { owner: owner, balance: opening }
    }

    fn deposit(self: &mut Account, amount: Int) {
        self.balance = self.balance + amount;
    }

    fn withdraw(self: &mut Account, amount: Int) -> Bool {
        if amount > self.balance {
            false
        } else {
            self.balance = self.balance - amount;
            true
        }
    }
}

enum Movement {
    Credit(Int),
    Debit(Int),
}

fn movement_label(m: Movement) -> String {
    match m {
        Movement.Credit(v) => "CREDIT " + v.to_string(),
        Movement.Debit(v) => "DEBIT  " + v.to_string(),
    }
}

fn main() {
    println("=== NYX LEDGER ===");
    let mut acc = Account.new("Ada", 1000);

    let credits = [500, 250, 100];
    let mut i = 0;
    while i < credits.len() {
        acc.deposit(credits[i]);
        println("credited ", credits[i]);
        i = i + 1;
    }

    let ok = acc.withdraw(1000);
    assert_true(ok == true);

    let over = acc.withdraw(999999);
    assert_true(over == false);

    println("label: " + movement_label(Movement.Credit(50)));
    println("final balance: " + acc.balance.to_string());
    assert_true(acc.balance == 850);
    println("=== DONE ===");
}

Output [verified]:

=== NYX LEDGER ===
credited 500
credited 250
credited 100
label: CREDIT 50
final balance: 850
=== DONE ===

Notice how each verified lesson shows up in one place:

Structs + methods: Account with new, deposit, withdraw, and the &mut receiver.

Enums + pattern matching: Movement.Credit/Debit and movement_label.

Arrays + index loop: credits[i] inside while i < credits.len().

Strings: concatenation with .to_string().

Assertions: the true/false branches of withdraw are proven, not assumed.

Your turn — extend the ledger

Add a Debit movement and print the whole day's movements by building a String with concatenation.

Add fn transfer(from: &mut Account, to: &mut Account, amount: Int) -> Bool and prove with assert_true that balances move correctly and that an over-limit transfer is refused.

Add a Ledger struct that holds an owner and a fixed array of the last three balances, and print a mini statement.

If you can do all three, you have the whole runnable dialect in your hands.

Appendices

Appendix A — Command-Line Reference

# Compile and run
.\nyx.exe -i program.nyx -r

# Compile only
.\nyx.exe -i program.nyx -o program.exe

# Optimize
.\nyx.exe -i program.nyx -o program.exe -O 3

# Inspect generated C
.\nyx.exe -i program.nyx --emit-c

# Emit assembly / LLVM IR
.\nyx.exe -i program.nyx -S
.\nyx.exe -i program.nyx --emit-llvm

# Choose a safety dialect
.\nyx.exe -i program.nyx -o program.exe -d havilah

# Version / help
.\nyx.exe --version
.\nyx.exe --help

Appendix B — Type Reference



Appendix C — Error Message Decoder



Appendix D — Verified Examples Index

Every example below was compiled and run on nyx 0.53.0. Files live in scratch/book_probe/ if you want to re-run them.



Appendix E — Glossary

Region — a block of memory whose whole lifetime is known at compile time; allocated by bumping a pointer, released by resetting it.

Arena / bump frame — the concrete implementation of a region.

Escape analysis — the compiler's job of deciding whether a value outlives its creating scope, and promoting it upward if so.

Dialect (A / B) — the runnable vs. modular styles of Nyx code (Chapter 15).

Receiver (self: &T, self: &mut T) — how a method gains access to the struct, and whether it may mutate.

Variant — one of an enum's possible values.

Capability — a token (CAP_NET, CAP_FILE, …) granting a component a specific authority.

Nursery — a scope that guarantees all spawned tasks finish before it exits.

SPSC — Single-Producer, Single-Consumer: a lock-free queue pattern used for the tightest loops.

Closing

You now hold the whole language. The path from here:

Rebuild the capstone from memory, then extend it with the exercises.

Read examples/ in the repo — calculator.nyx and hello_world.nyx are runnable-dialect programs you can compile today.

When you need a library, switch to the modular dialect (Chapter 15) and study examples/snake.nyx.

Before quoting any performance number, run the benchmark suite yourself. Nyx's claims are meant to be reproducible, and this book deliberately repeats none of them as fact.

End of book.