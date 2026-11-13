# 🚀 Getting Started with the Nyx Programming Language (v0.53.0)

Welcome to **Nyx**, a high-performance systems programming language designed for zero-GC determinism, bare-metal throughput, and clean developer ergonomics.

---

## ⚡ 1-Minute Quickstart (Precompiled Standalone Toolchain)

The public repository includes the precompiled, production-ready native compiler binary at `bin/nyx.exe`:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/9jaoncloud/nyx.git
   cd nyx
   ```

2. **Verify Compiler Version:**
   ```powershell
   .\bin\nyx.exe --version
   # Outputs: nyx 0.53.0
   ```

3. **Compile & Run Your First Program:**
   ```powershell
   .\bin\nyx.exe -i examples\hello_world.nyx -r
   ```
   Output:
   ```text
   Compiled: examples/hello_world.exe
   ==================================================
   🌙 Welcome to the Nyx Programming Language (v0.53.0)!
   Zero-GC Region Inference · 0.00ms Pauses · Native x86_64
   ==================================================
   Quick test calculation (21 * 2): 42
   Ready to build high-performance systems!
   ```

4. *(Optional)* **Add Nyx to your System PATH:**
   ```powershell
   # In PowerShell (Current Session):
   $env:Path += ";$PWD\bin"
   
   # Now you can invoke nyx directly from any folder:
   nyx -i main.nyx -r
   ```

---

## 🛠️ Compiler Command-Line Interface (CLI)

The Nyx compiler provides an intuitive command-line interface for compiling, optimizing, and running native applications:

```text
Usage: nyx.exe [OPTIONS]

Options:
  -i, --input <INPUT>          Path to the entry source file (.nyx)
  -o, --output <OUTPUT>        Path to the output executable or library
  -r, --run                    Compile and execute immediately
  -g, --debug                  Generate debugging symbols (DWARF/PDB)
  -O, --opt-level <OPT_LEVEL>  Optimization level: 0, 1, 2, 3, or 's' (default: 2)
      --emit-c                 Emit optimized transpiled C intermediate code
  -S, --emit-asm               Emit native assembly
      --emit-llvm              Emit LLVM IR
      --target <TARGET>        Cross-compilation target triple
  -j, --jobs <JOBS>            Parallel worker threads (default: 1)
  -d, --dialect <DIALECT>      Dialect selection: auto, base, zephaniah, havilah, jude
  -h, --help                   Print help information
  -V, --version                Print compiler version
```

### Common Compiler Workflows:

* **Compile to Native Executable:**
  ```powershell
  nyx -i app.nyx -o app.exe
  ```

* **Compile with Maximum Optimization (-O3):**
  ```powershell
  nyx -i app.nyx -o app.exe -O 3
  ```

* **Inspect Generated C Source:**
  ```powershell
  nyx -i app.nyx --emit-c
  # Generates app.c for inspection
  ```

---

## 📂 Repository Structure

When you clone `https://github.com/9jaoncloud/nyx.git`, you receive a complete, self-contained development toolchain:

```text
nyx/
├── bin/                       # Precompiled native 6-phase compiler binaries
│   └── nyx.exe                # Nyx v0.53.0 compiler for Windows x86_64
├── runtime/                   # Core runtime C headers (rt_io.h, rt_vec.h, rt_math.h)
├── include/                   # Extended runtime headers
├── std/                       # Nyx Standard Library modules (.nyx)
│   ├── core/                  # Core primitives (collections, math, string, time)
│   ├── io.nyx                 # Console and stream IO
│   ├── math.nyx               # Vector, matrix, trigonometric math
│   ├── string.nyx             # UTF-8 string allocations & slices
│   └── vec.nyx                # Dynamic zero-GC vectors
├── examples/                  # Verified runnable example programs
│   ├── hello_world.nyx        # Quickstart program
│   ├── scientific_calculator.nyx # Full scientific calculator suite
│   ├── snake.nyx              # Console arcade snake game
│   └── whot.nyx               # Card game simulation
├── docs/                      # Technical documentation & benchmarks (Markdown)
│   ├── GETTING_STARTED.md     # This guide
│   ├── MEMORY_MODEL.md        # Zero-GC static region inference
│   ├── SYNTAX_AND_TYPES.md    # Language reference & type system
│   ├── CONCURRENCY.md         # CSP channels, nurseries, green threads
│   ├── BENCHMARKS.md          # 2026 Head-to-Head empirical benchmarks
│   ├── NYXOS.md               # Microkernel & OS integration
│   └── FAQ.md                 # Frequently Asked Questions & disambiguation
└── README.md                  # Project landing page
```

---

## 📖 Next Steps

* Learn how Nyx eliminates GC pauses without lifetime annotations: [**Zero-GC Memory Model**](MEMORY_MODEL.md)
* Explore syntax, structs, enums, and pattern matching: [**Syntax and Types**](SYNTAX_AND_TYPES.md)
* Review authentic empirical hardware benchmarks: [**Head-to-Head Benchmarks**](BENCHMARKS.md)
