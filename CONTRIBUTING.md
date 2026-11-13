# Contributing to Nyx

Thank you for your interest in contributing to the Nyx programming language!

## Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/YOUR_USERNAME/nyx`
3. Build the project: `cd nyx && ./scripts/bootstrap.sh`

## Project Structure

See [README.md](README.md) for the full project structure.

- `nyx-bootstrap/` — Rust Phase 1 compiler (lexer, parser, typechecker, C codegen, C runtime)
- `nyxc/` — Nyx self-hosted compiler (work here for core compiler features)
- `nyx-cli/` — CLI tools (work here for package manager, build system)
- `std/` — Standard library (30 .nyx modules fully written, waiting on import resolution)
- `domain/` — C wrappers (work here for new domain integrations)
- `examples/` — 27 demo programs (all compile & run via the bootstrap compiler)
- `scripts/` — Build scripts, dev tooling

## Current Priorities

### 1. Module Import Resolution (CRITICAL)
The bootstrap compiler parses `import` statements but does not resolve them. The 30 stdlib `.nyx` modules are fully written. To restore the original 8 complex demos to their full form, implement:

- **Module loader** in `nyx-bootstrap/src/driver.rs` — given `import std.http`, locate `std/http.nyx` in the stdlib path
- **Recursive compilation** — parse + typecheck imported files, merge `pub` symbols into the importing scope
- **Circular import detection** — reject cycles with a clear error
- **Codegen linkage** — emit C functions for each module, link all generated `.c` files

See `docs/internals/compiler-pipeline.html` for the design.

### 2. Standard Library Hardening
- Fill in C runtime stubs: real HTTP server/client (`rt_http.c`), SQLite via SQLite3 C API (`rt_db.c`)
- Portability: test on Windows/MSYS2, macOS, Linux

### 3. Region Inference
- Move from simplified escape detection to full lifetime analysis
- Benchmark against Rust ownership for correctness

### 4. Async Runtime
- Implement M:N work-stealing scheduler in a new `rt_async.c`
- Integrate epoll/kqueue/IOCP for true async I/O

## Development Workflow

1. Create a feature branch
2. Make your changes
3. Run tests: `cargo test` (for Rust) or `nyx test` (for Nyx)
4. **Validation gate**: All 27 demo programs must compile and run —
   ```
   cd nyx-bootstrap && cargo run -- compile examples/
   ```
5. Submit a pull request

## Code Style

- Rust code follows standard `rustfmt` conventions
- Nyx code uses 4-space indentation
- All public APIs should be documented with doc comments

## Adding a New Standard Library Module

1. Write the Nyx module in `std/<name>.nyx` with `module <name>` declaration and `pub` exports
2. If it requires C bindings, add the corresponding `rt_<name>.c/h` in `nyx-bootstrap/runtime/`
3. Register any needed function return types in `codegen_c.rs`'s `FN_RET_TYPES`
4. Add a demo in `examples/` and verify it compiles via the bootstrap compiler

## Testing

- Bootstrap compiler: `cd nyx-bootstrap && cargo test`
- Nyx compiler: `cd nyxc && nyx test`
- Standard library: tests are in `std/*/tests/`
- Demo validation: `cd nyx-bootstrap && cargo run -- compile examples/` (all 27 must pass)
