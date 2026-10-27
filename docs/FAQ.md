# ❓ Frequently Asked Questions (FAQ) & Project Disambiguation

---

## ⚠️ 1. Name Disambiguation — Which "Nyx" is this?

**Nyx (this project)** is the **systems programming language** created by **Simeon Bala / 9jaonCloud Engineering**:
* **Architecture:** Zero-GC static region inference, O(1) bump frame arenas, capability-secure microkernel, and native x86_64/ARM C transpilation.
* **Compiler:** `nyx.exe` / `nyxc` (Phase 1 bootstrap compiler written in Rust, Phase 2 self-hosted native).
* **Products:** Nyx Programming Language, NyxOS, Nyx Mobile OS, and Athena Trading Platform.
* **Official Repository:** `https://github.com/9jaoncloud/nyx`

### ❌ Not to be confused with:
**NyxCode** (`nyxcode.io` / `heynyx.dev` by Fabian Budde):
* An unrelated **web full-stack DSL** (transpiles single files into HTML/CSS/JS with Express and SQLite, distributed as npm package `@fabudde/nyxcode`).
* They share **no code, no maintainers, no syntax, no targets, and no relationship**.

| Dimension | **Nyx (this project)** | NyxCode *(unrelated)* |
| :--- | :--- | :--- |
| **Domain** | Systems / OS kernels / Embedded / HFT | Web full-stack generation |
| **Compiler** | `nyx.exe` / `nyxc` | `nyx` (npm) |
| **Lead Developer** | Simeon Bala — 9jaonCloud Engineering | Fabian Budde |
| **Memory Model** | Automatic Static Region Inference (Zero-GC) | Node.js V8 Garbage Collector |

---

## 🛠️ 2. Why does Nyx transpile to C instead of emitting LLVM IR directly?

Nyx's 6-phase compiler emits clean, highly structured, standard ISO C99/C11 code, which is then compiled via GCC or Clang:
1. **40+ Years of Optimization:** Decades of compiler engineering have made GCC and Clang masters of vectorization (AVX2, AVX-512, NEON), loop unrolling, and instruction scheduling.
2. **Instant Universal Portability:** Nyx immediately targets every CPU architecture supported by GCC, including x86_64, AArch64, ARM Cortex-M, RISC-V, and WebAssembly.
3. **Seamless FFI Interoperability:** Calling any existing C or C++ library requires zero glue code or wrapper overhead.
4. **Inspectable Intermediate Code:** Developers can pass `--emit-c` to inspect the generated C code directly, ensuring full transparency.

---

## ⚡ 3. How does Nyx achieve 0.00ms GC pauses without manual `free()`?

Nyx uses **Automatic Static Region Inference**:
* The compiler analyzes variable lifetimes at compile time and groups related data into region frames.
* Allocations are placed on fast stack-like monotonic bump arenas ($O(1)$).
* When a region scope exits (such as a function call or loop iteration), the entire arena is reset instantaneously with zero per-object traversal overhead.
* Objects that escape their scope are automatically promoted to parent caller regions.

---

## 💻 4. What platforms are supported?

* **Windows:** Windows 10/11 x86_64 (MinGW-w64 / MSVC).
* **Linux:** Ubuntu, Debian, Fedora, Arch Linux, Alpine (GCC / Clang).
* **macOS:** Apple Silicon (AArch64) and Intel x86_64.
* **Embedded & Real-Time:** ARM Cortex-M, ESP32, and custom bare-metal targets.
