# Changelog — Nyx Systems Programming Language

All notable changes to the Nyx Programming Language, compiler (`nyx.exe` / `nyxc`), standard library (`std`), and multi-dialect runtime ecosystem are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [0.53.0] — 2026-10-16

### ⚡ Language Server Protocol (LSP Daemon)
- **Native `nyx lsp` Daemon**: Integrated JSON-RPC 2.0 language server communicating over standard I/O for editor integration (VS Code, Neovim, Helix, Zed, Emacs):
  - `textDocument/didOpen` & `textDocument/didChange`: Real-time incremental synchronization and syntax validation.
  - `textDocument/publishDiagnostics`: Instant type-checking feedback and dialect-aware diagnostic reporting.
  - `textDocument/hover`: Quick hover documentation for Nyx keywords, memory primitives (`region`, `@gen`), and types.
  - `textDocument/completion`: Context-aware keyword and snippet autocompletion.

### 🧠 Compile-Time Function Execution (`comptime`)
- **CTFE & Constant Evaluator (`comptime.rs`)**: Implemented Zig/Odin-style `#comptime` blocks and expressions.
- **Phase 5 Constant Folding**: Arithmetic, unary expressions, let-bindings, and deterministic blocks inside `comptime { ... }` evaluate at compile time and fold into pure C numeric/boolean/string literals in emitted code with zero runtime overhead.

### 📚 Built-in Documentation Generator (`nyx doc`)
- **Native Doc Generator (`docgen.rs`)**: Added `nyx doc <target> [-o <out_dir>]` compiler subcommand.
  - Automatically parses doc comments (`///`) across functions, structs, enums, traits, and African dialect declarations.
  - Generates a standalone, dark-themed HTML documentation site with real-time JavaScript search filtering and symbol indexing (`search_index.json`).
  - Documented 2,604 items across 187 standard library modules into `docs/api/`.

### 💻 1-Click Editor Extension (`.vsix`)
- **Packaged Official Extension (`nyx-lang-0.53.0.vsix`)**:
  - Full TextMate syntax highlighting grammar for standard Nyx, memory region primitives, and African language dialects (Èdè Yorùbá, Harshen Hausa, Asụsụ Igbo, Nigerian Pidgin).
  - Built-in contextual hover explanations, snippets (`comptime`, `region`, `defer`, dialect functions), and direct connection to the `nyx lsp` daemon.
  - Published for direct 1-click installation in VS Code, VSCodium, Cursor, Windsurf, and Zed.

### 📦 Sovereign Package Manager (`nyx pkg`)
- **`nyx pkg` CLI Tool**: Integrated package manager for the Nyx ecosystem:
  - `nyx pkg init [name]`: Generates a standard `nyx.pkg.json` package manifest.
  - `nyx pkg install <pkg_name>`: Automatically fetches and configures Nyx modules and language dialect libraries (such as `@nyx-yoruba`, `@nyx-hausa`, `@nyx-igbo`).
  - `nyx pkg list`: Inspects installed workspace dependencies and manifests.

### 🚀 Direct Toolchain & Code Generation
- **DWARF Source-Level Debugging**: Emitted C `#line 1 "<source>.nyx"` directives into transpiled C source prior to user item emission. In combination with `-g` DWARF symbol generation, debuggers (GDB, LLDB, and VS Code C/C++ / Native Debug) now map breakpoints, instruction pointers, and call stacks directly to `.nyx` source files.
- **`-S, --emit-asm` Native Assembly Flag**: Added compiler flag to generate clean, annotated x86-64 assembly (`.s`) directly via `gcc -S -fverbose-asm` with user-selected optimization levels.
- **`--emit-c` Transpilation Inspection**: Added direct flag to stop after phase 6 code generation, allowing developers to inspect C emitted files without triggering the host toolchain link phase.
- **`--emit-llvm` IR Generation**: Added direct flag to emit LLVM Intermediate Representation (`.ll`) via Clang.
- **`-O<level>` Optimization Control**: Supported explicit compiler optimization flags (`-O0`, `-O1`, `-O2`, `-O3`, `-Os`), defaulting to `-O2` for release builds.

### 🌐 Multilingual Localized Compiler Diagnostics
- **Native African Language Diagnostic Engine**: Upgraded `DiagnosticEngine` in `diagnostics.rs` to detect and report compilation errors, warnings, notes, and hints in the developer's selected language:
  - **Èdè Yorùbá**: Header `=== Àwọn Àkíyèsí Amúkọ̀wé Nyx ===`, severity tags `àṣìṣe`, `ìkìlọ̀`, `àkíyèsí`, `ìrànwọ́`, and translated messages (e.g. `Ayípadà tí kò tíì fìdí múlẹ̀`, `Àìbámu irúfẹ́`).
  - **Harshen Hausa**: Header `=== Binciken Mai Fassara Nyx ===`, severity tags `kuskure`, `gargadi`, `lura`, `taimako`, and translated messages (e.g. `Babu ma'anar wannan mai canzawa`, `Nau'o'in ba su dace ba`).
  - **Asụsụ Igbo**: Header `=== Nchọpụta Nchịkọta Nyx ===`, severity tags `mperi`, `ịdọ_aka_na_ntị`, `ndetu`, `enyemaka`, and translated messages (e.g. `Agbanwe akọwaghị akọwa`, `Ụdị adabaghị`).
  - **Nigerian Pidgin**: Header `=== Nyx Compiler Wahala Report ===`, severity tags `wahala`, `warning`, `notice`, `wetin-fit-help`.
  - **English (Default)**: Standard systems compiler diagnostic labels.
- **Automatic Dialect Detection**: Diagnostician inspects raw input source and automatically matches language keywords without requiring explicit `--dialect` flags.

### 📦 African Language Ecosystem Packages
- Published and distributed standalone scoped packages:
  - `@nyx-yoruba`: Èdè Yorùbá systems keyword standard dictionary and idioms.
  - `@nyx-hausa`: Harshen Hausa systems keyword standard dictionary and idioms.
  - `@nyx-igbo`: Asụsụ Igbo systems keyword standard dictionary and idioms.
  - `@nyx-i18n`: Multi-dialect synchronization and dictionary bridging engine.
- Synced standard library i18n dictionaries in `std/i18n/` covering 25 systems primitives (`region`, `enum`, `trait`, `gen`, `conclude`, `strict`).

### 🛡️ Memory Safety & Dialect Invariants
- **Path-Based Borrowing**: Integrated disjoint struct field borrowing to avoid premature promotion of short-lived allocations into parent region arenas.
- **Optional Generational Pointers (`@gen`)**: Safe dynamic cache pointer invalidation without tracing GC overhead.
- **Higher RAII for Linear Invariants (`.hav`)**: Enforces mandatory conclusion calls (`conclude`) for critical state machines (cryptographic sessions, database transactions) in the Havilah dialect.

### 🏛️ Archival & Reproducibility
- Preserved complete compiler source code for `v0.53.0` in `compiler/archive/v0.53.0/` adhering to the Historical Archival Protocol.

---

## [0.52.0] — 2026-10-12

### Added
- **Havilah Dialect Support (`.hav`)**: First-class support for linear capabilities and provable transaction conclusions.
- **Linear State Machines**: Static verification of un-dropped resources.
- **Cross-Dialect Type Resolution**: Interoperability between `.nyx`, `.zeph`, and `.hav` AST representations.

### Optimized
- Region memory arena reuse in tight loop iterations.
- Parallel module dependency resolution in `driver.rs`.

---

## [0.51.0] — 2026-10-01

### Added
- **Odin-Inspired Native Execution Context (`NyxContext`)**: Thread-local nursery allocator and thread-safe logger context embedded into generated binaries.
- **Incremental Cache Pipeline**: FNV-1a content hashing of source AST modules to eliminate redundant C transpilation.
- **Multi-File Project Compilation**: Recursive import tree discovery and automatic forward declaration collection.

---

## [0.50.3] — 2026-09-15

### Added
- **6-Phase Native Compiler Pipeline**:
  - Phase 1: Lexical analysis and Pratt parsing.
  - Phase 2: Bidirectional type checking and exhaustiveness pattern verification.
  - Phase 3: Monomorphization of generic functions and structs.
  - Phase 4: Zero-GC Region Inference and lifetime propagation.
  - Phase 5: Monadic intermediate optimizations and constant folding.
  - Phase 6: Emitted C23-compliant systems code with zero external runtime dependencies.
- Native x86-64 PE/ELF linking via host GCC/Clang toolchain.
