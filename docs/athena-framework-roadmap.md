# Athena Native Framework — Engineering Report & Roadmap

> Scope: findings from building the **Athena AI Trading Engine** native GUI on
> `nyx-bootstrap`, plus a forward-looking plan for a robust, futuristic Nyx
> language + framework. This document consolidates what we learned, the issues
> we hit, the library gaps, and the upgrades required.

---

## 1. What We Learned

### 1.1 Two compilers, only one works for emitting binaries
- **`nyx-bootstrap`** (`nyx-bootstrap/target/debug/nyx-bootstrap.exe`) is the
  *only* compiler in this environment that actually produces a runnable `.exe`
  from Nyx source. Build command:
  `cargo run --bin nyx-bootstrap --manifest-path nyx-bootstrap/Cargo.toml -- <file>.nyx --run`
- **`nyxc/main.exe`** (the self-hosted / "real" compiler) prints
  `Compilation successful` but emits **no binary**. It is currently
  non-functional for code emission here and must not be relied on.
- **Implication:** the bootstrap compiler is the de-facto toolchain, and it has
  a noticeably smaller/buggier codegen surface than the language semantics imply.

### 1.2 A real native UI framework already exists
- `std/ui.nyx` is a large, genuine **Material Design 3** implementation backed by
  Skia (`domain/skia/nyx_skia.c`, `nyx-bootstrap/runtime/rt_ui.c`):
  `Window`, `Canvas`, `MaterialTheme`, `MaterialRenderer`, `CardWidget`,
  `ButtonWidget`, `BadgeWidget`, `NavItem`, `NavigationRailWidget`,
  `TopAppBarWidget`, `TextWidget`, `SliderWidget`, `ProgressBarWidget`,
  `SwitchWidget`, `CheckboxWidget`, `Color`, enums, etc.
- `examples/demo_material_dashboard.nyx`, `demo_material_interactive.nyx`,
  `demo_material_gpu.nyx` **compile and run** (exit 0, "rendered
  successfully"). So the native rendering path is real and the runtime/SDL/Skia
  backing is solid.
- The doc `docs/response-to-assessment.html` claims Phases 1–9 (incl. native MD3)
  are done — partly aspirational, but `std/ui.nyx` + the demos are genuinely
  functional.

### 1.3 The HTML dashboard path is fully working
- `athena/src/main.nyx` → emits `ATHENA_JSON {...}` (with a `~` quote-placeholder
  workaround) → `_export.bat` redirects to `data.json` + stamps `ATHENA_TIME`.
- `athena/dashboard.html` fetches `data.json` over `http.server` and renders
  real ticks/signals/orders/news/score across 5 tabs. Verified end-to-end.

---

## 2. Issues Faced (the debugging war-story)

Every issue below was found empirically by bisecting a segfault/compile failure.
They are **`nyx-bootstrap` codegen/lexer bugs**, not logic errors in our app.

| # | Symptom | Root cause | Workaround used |
|---|---------|-----------|-----------------|
| 1 | `\"` in source produced a literal backslash-quote, breaking JSON output | `lexer.c` converts `\"` → `\` + `"` | Use `~` as a JSON-quote placeholder, convert `~`→`"` in consumer (JS / `_export.bat`) |
| 2 | Returning `Vec<Struct>` from a fn crashed at runtime | Bootstrap can't codegen growing/returning `Vec<Struct>` | Build the `Vec` inline in the caller; never return it |
| 3 | `module`-qualified types (`std.db.Something`) failed to codegen | Bootstrap doesn't resolve module-qualified type names | Avoid; keep types in scope |
| 4 | `for x in &Vec` failed to bind struct fields (local `&Vec`) | Loop codegen only resolves field access when the `&Vec` is a **function parameter** | Put loops that read struct fields inside a helper that takes `&Vec` as a param |
| 5 | `canvas.draw_text(...)` emitted as bare `draw_text(...)` (implicit-decl) | Method call on a **reference receiver** (e.g. `&mut Canvas` param) loses its type prefix | Pass `canvas`/`renderer` as **value** receivers (verified by a minimal probe) |
| 6 | `&mut *canvas` where `canvas` is already `&mut Canvas` → "void value not ignored" | Reborrow codegen bug | Pass the value/param directly; don't double-deref |
| 7 | `std undeclared` / `Center_new` mismatch | Name clash: `Center` struct vs `Alignment::Center` variant in `std/ui.nyx` | Renamed `Center` → `CenterBox` in `std/ui.nyx` |
| 8 | **Segfault** when concatenating a struct-field `String` (`"  " + s.symbol`) | `rt_string_concat` corrupts the field's buffer / lifetime when an operand is a struct-field `String` (or its `.to_string()` clone) | Draw `String` fields **directly** (no concat); build combined text only from `literal + float.to_string()` chains |
| 9 | Stale artifacts: recompiles silently reused old `.c`/`.exe` | Bootstrap caches aggressively | `rm -f <x>.exe <x>.c` before recompiling |

**Net result:** the native Athena GUI (`athena/src/athena_native.nyx`) now
compiles and runs (exit 0, "rendered successfully"), rendering the same real
engine data as the HTML dashboard, using a native Skia/MD3 window.

---

## 3. Library & Language Gaps We Must Cover

These are concrete missing/buggy capabilities blocking a robust framework:

### 3.1 Compiler / codegen
- **Self-hosting compiler (`nyxc`) emits no binary.** The language cannot
  bootstrap itself; we are pinned to the Rust-based bootstrap.
- **No `Vec<Struct>` return** — must be fixed for any non-trivial data flow.
- **`for` loop field-binding bug** for non-parameter `&Vec` (Items 4/5).
- **Reference-receiver method codegen** drops the type prefix (Item 5).
- **String lifetime/concat bug** with struct-field strings (Item 8) — memory
  unsafety in `rt_string_concat` path.
- **Lexer `\"` handling** (Item 1) breaks any real string-escaping / JSON / SQL.
- **Module-qualified types unsupported** (Item 3) — blocks clean namespacing.
- **No incremental/cached build invalidation** (Item 9) — wastes debugging time.
- **No real error reporting** from the compiler in many cases (falls back to C
  compiler errors on generated `.c`).

### 3.2 Standard library
- **`std.db` not wired in bootstrap** — SQLite FFI exists in `nyxc` runtime but
  `nyx-bootstrap` cannot codegen it. Athena currently uses an in-memory
  snapshot instead of the intended `athena.db`.
- **No `std.async` / `std.net` / `std.http`** in bootstrap → the engine's
  "live data", "stealth news feed", and "executor/orders" are stubbed.
- **No JSON (de)serialization in-language** — had to hand-roll `ATHENA_JSON`
  with the `~` hack.
- **`String` has no safe `concat`/`format`/`join` that handles struct fields.**
- **`Vec` lacks ergonomic iteration that always binds fields** (see Item 4).
- **No `Result`/`Option` or error-propagation (`?`)** — error handling is ad-hoc
  `println` + sentinel values.
- **No `Map`/`Set`/`HashMap`** collections used by real trading logic.

### 3.3 UI framework (`std/ui`)
- **No declarative UI layer** (`ui.App` / `Column` / reactive widgets) — the
  aspirational `nyx2/athena/src/dashboard.nyx` references an API that does not
  exist. Everything is imperative draw-calls.
- **No charts/plots/candlesticks** — critical for a trading UI.
- **No tables / data-grid** for orders/signals.
- **No layout engine** (only manual x/y coordinates) — fragile, non-responsive.
- **No theming persistence / design-token export.**
- **`Center` clash** already fixed; audit for other name collisions.
- **No input event handlers wired in the `nyx-bootstrap` path** (demos render one
  frame and exit); a real window needs `should_close`/`poll_events` loops that
  actually run.

### 3.4 Tooling / DX
- **No LSP that understands Nyx** — the editor LSP chokes on generated C/lexer
  files; no real completion/type-info for Nyx.
- **No test runner integrated with bootstrap** (`athena/tests` exists but relies
  on the broken path).
- **No formatter / linter.**
- **No package manager / registry** — `import` is file/path based.
- **No build manifest** (`nyx.toml`) — invocation is a long `cargo run` one-liner.

---

## 4. Upgrades for a Robust, Futuristic Framework & Language

Prioritized, with futuristic appeal. Each item lists the *why* and a concrete
first step.

### 4.1 Make the compiler trustworthy (P0)
1. **Fix the bootstrap codegen bugs** (Items 4, 5, 8) — these are memory-safety
   and correctness blockers. Add regression tests in `nyx-bootstrap/tests/`.
2. **Self-host: make `nyxc` emit binaries** (LLVM or C backend). A language that
   cannot compile itself is not production-credible. Start by having `nyxc`
   lower to the *same* C runtime `nyx-bootstrap` uses, then add an LLVM target.
3. **LLVM backend** for native, optimized, multi-target code (Windows/Linux/macOS)
   — enables SIMD for the quant/orderbook math and removes the C-generation
   fragility.
4. **Real diagnostics**: surface Nyx-level type/borrow errors instead of dumping
   generated C errors. A span-based error reporter is table-stakes.
5. **Proper build cache with content hashing** (kill Item 9) + a `nyx build`
   CLI and optional `nyx.toml`.

### 4.2 Language ergonomics (P0/P1)
6. **Fix the lexer** (`\"`, unicode, raw strings `r"..."`, multi-line strings).
7. **`String` overhaul**: safe `format!`, `join`, `concat` with correct
   lifetimes (ARC or copy-on-write), so Item 8 never recurs.
8. **`Result` / `Option` + `?` operator + `match` on them** for real error flow.
9. **Generics / traits** so `Vec<T>`, `Map<K,V>`, and UI widgets are
   type-safe and reusable.
10. **Ownership/borrow model** (Rust-like or GC) made explicit and documented, so
    the reference-receiver bug (Item 5) is impossible by construction.
11. **Collection library**: `Vec`, `HashMap`, `Set`, `Deque`, with iteration that
    *always* binds fields regardless of how the collection is passed.
12. **Async/await + channels** for streaming market data and the executor.

### 4.3 Standard library (P1)
13. **Wire `std.db` (SQLite) into bootstrap** — Athena's persistence story. Add a
    tiny migration/ORM layer (`Table` derive, typed queries).
14. **`std.net` / `std.http` / `std.ws`** for live feeds, REST, and websockets
    (the "stealth news" + "executor" need these).
15. **`std.json`** (typed `to_json`/`from_json` derive) — replace the `~` hack.
16. **`std.time` / `std.sched`** for tick scheduling and backtests.
17. **`std.ai`** — the docs claim an "AI Model Hub"; provide a real inference
    FFI (ONNX/Triton) so signals are model-driven, not hardcoded.

### 4.4 Native UI: from imperative to declarative + trading-grade (P1/P2)
18. **Declarative UI DSL** (`ui.App { Column { ... } }`) with a **reactive
    signal/diffing renderer** — the current imperative API is unscalable.
19. **Layout engine**: flexbox/grid, responsive, DPI-aware (kill manual x/y).
20. **Charts module**: candlesticks, line/area, volume, heatmaps — GPU-accelerated
    via the existing Skia path. This is the heart of a trading UI.
21. **Data-grid / virtualized table** for orders, fills, signals.
22. **Theming system**: design tokens, light/dark, brand skins, runtime switching.
23. **Real windowing loop + input** (keyboard/mouse/gesture) wired through
    bootstrap so apps stay alive and interactive.
24. **WebAssembly target** for the same UI to run in-browser with no server —
    unifying the HTML dashboard and native app.

### 4.5 Tooling & ecosystem (P2)
25. **Nyx LSP** (completion, hover, go-to-def, inline errors) — the current UX is
    blind.
26. **Formatter (`nyx fmt`) + linter (`nyx check`)**.
27. **Test framework** (`nyx test`) integrated with both compilers.
28. **Package registry + `nyx add`** for `std` extensions and community crates.
29. **Hot-reload** for UI and strategy code — crucial for trading iteration.
30. **Observability**: structured logs, metrics, tracing for the engine.

### 4.6 Futuristic differentiators (P2/P3 — the "appeal")
31. **AI-native language features**: first-class agents (`agent { ... }`),
    tool-calling, prompt/context types, and an embedded model-runtime so Athena
    can self-describe and self-tune.
32. **Effect system / capabilities** for safe I/O, network, and DB access —
    auditable for finance compliance.
33. **Polyglot FFI** (call Rust/Python/C easily) to reuse quant/ML ecosystems.
34. **Distributed runtime**: actors/erlang-style supervisors for always-on
    trading nodes; the docs already hint at "structured nursery tasks".
35. **WASM + native parity** so one codebase ships to desktop, web, and edge.
36. **Visual strategy builder** that emits Nyx (low-code for quants) — pairs with
    the native MD3 UI for a polished, modern product surface.

---

## 5. Immediate Next Steps (recommended order)
1. Fix bootstrap codegen bugs #4, #5, #8 + add regression tests. (unblocks everything)
2. Wire `std.db` + `std.json` into bootstrap; replace `~` hack. (real persistence)
3. Add `String` format/join with safe lifetimes. (kills Item 8 permanently)
4. Declarative UI + layout + charts. (makes Athena a real trading product)
5. Self-host `nyxc` (C-backend first, then LLVM). (credibility + performance)
6. LSP + `nyx build/test/fmt`. (developer experience)
7. WASM target + AI/agent primitives. (futuristic appeal)

> **Status today:** native GUI runs (exit 0) but is a single-frame snapshot;
> HTML dashboard is the fully-working live view. The gaps above are what stand
> between the current prototype and a robust, futuristic trading framework.

---

## 6. Compiler Fixes Applied (working session)

These were fixed directly in `nyx-bootstrap/src/codegen_c.rs` and verified by
recompiling the bootstrap + running generated binaries. **IMPORTANT: the
bootstrap driver caches generated C in `.nyx_cache/` keyed by source hash, so
after editing the compiler you MUST `rm -rf .nyx_cache athena/.nyx_cache` (and
delete stale `*.c`/`*.exe`) or codegen is silently skipped (Item #9).**

### 6.1 String-escape bug (Item #1) — FIXED
`c_esc_string()` now unescapes Nyx escape sequences (`\"`, `\\`, `\n`, …) into
logical characters and re-escapes them for the generated C, so `"\""` produces a
real `"` instead of a literal backslash-quote. Verified:
`println("a\"b")` → `a"b`, `"\n"` → real newline.

### 6.2 `~` JSON-quote hack — REMOVED
`athena/src/main.nyx` now emits real `"` characters; `dashboard.html` was
updated. The emitted JSON was validated with `JSON.parse` (keys:
score, ticks, signals, news, orders).

### 6.3 `?` (try) operator — FIXED (codegen)
Added `normalize_c_type()` (maps Nyx names `Int`/`Float`/`Bool`/`Char`/`String`
→ C types `int64_t`/`double`/`int`/`char`/`rt_string_t`) and used it in the `?`
expansion so the unwrapped value and the `let` binding both get a valid C type
(previously emitted the Nyx name `Int`, which is not a C type). Verified the
generated C now reads `int64_t v = ({ NyxResult _qres = ... (int64_t)_qres.data.int_val; })`.

### 6.4 `c_type()` primitive-name mapping — FIXED
`c_type(Type::Custom("Int"))` (and `Float`/`Bool`/`Char`) now map to the correct
C type instead of falling through to the `"void*"` default. This is what lets
enum/Result payloads bind as the right C type.

### 6.5 `match` on `Result`/`Option` with primitive payloads — KNOWN LIMITATION (NOT yet fixed)
When you write `match r { Result.Ok(v) => … }` where the payload is a primitive
(Int/Float/Bool), the generated binding reads the wrong union field
(`void* v = __match_val.data.ptr_val`) and segfaults at runtime. Root cause:
`ENUM_DEFS` records `Result.Ok`/`Err` (and `Option.Some`) payloads as
`Type::Unknown`, and the concrete `Result<Int,String>` type is not propagated
from the resolver to codegen (the scrutinee's inferred type comes back empty).
String-payload matches DO work, because the existing name heuristic maps `e`/
`err`/`msg` → `rt_string_t`. Struct-payload matches also work. Only **primitive
(payload) types in `match` arms** are broken.

**Workaround that already works:** design std APIs as `Result<T, String>` /
`Result<Struct, String>` (string or struct error/payload) so match arms bind to
a known type. The `?` operator and `Result` *return/propagation* are fine.

**Real fix (next):** thread the generic args of `Result<T,E>`/`Option<T>` from
the resolver into codegen (store concrete payload types in `ENUM_DEFS` instead
of `Type::Unknown`, or look them up from the scrutinee's full type at the match
site). This is the key gate before the existing `std/net.nyx`, `std/json.nyx`,
`std/db.nyx` (which are Result/enum-heavy) can be wired into bootstrap.

