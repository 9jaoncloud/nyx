# 🌙 The Nyx Programming Language & Athena Platform

[![Version](https://img.shields.io/badge/version-0.26.0--prod-00e5ff.svg)](CHANGELOG.md)
[![License](https://img.shields.io/badge/license-MIT-00e676.svg)](LICENSE)
[![Memory Safety](https://img.shields.io/badge/memory-Zero--GC%20Region%20Inference-7c4dff.svg)](https://nyx.9jaoncloud.com.ng/docs/language/memory.html)
[![UI Engine](https://img.shields.io/badge/GUI-Native%20120%20FPS%20GPU-ff007f.svg)](examples/athena_workstation.exe)
[![Benchmarks](https://img.shields.io/badge/Benchmarks-2026%20Head--to--Head-ffab00.svg)](https://nyx.9jaoncloud.com.ng/comparison.html)

**One language, all platforms. Predictable native performance, zero GC pauses, delightful ergonomics.**


> 🌐 **Official Live Platform & Interactive Docs:** [**nyx.9jaoncloud.com.ng**](https://nyx.9jaoncloud.com.ng)  
> 📖 **Read the Genesis & Builder Story:** [The Nyx Genesis](https://nyx.9jaoncloud.com.ng/about.html) | ❓ **Architecture FAQ:** [Nyx FAQ Hub](https://nyx.9jaoncloud.com.ng/faq.html) | ⚡ **Benchmarks:** [Head-to-Head Comparison](https://nyx.9jaoncloud.com.ng/comparison.html) | 🚀 **Web IDE:** [Athena Playground](https://nyx.9jaoncloud.com.ng/athena/dashboard.html)

Nyx is a modern native systems programming language and application platform. From a single codebase, Nyx compiles to **lightweight static binaries (~650 KB)**, **WebAssembly (WASM SIMD128)**, **iOS**, and **Android** — powered by automatic region inference, structured concurrency, and a native GPU UI toolkit.

> ### ⚠️ Name Disambiguation — Which "Nyx" is this?
>
> **Nyx (this project)** — the **systems** programming language by **Simeon Bala / 9jaonCloud Engineering**:
> zero-GC region inference, a capability-secure microkernel, and the **NyxOS** and **Nyx Mobile OS**
> operating systems. Compiled with **`nyxc`**. Built for OS kernels, embedded, real-time, robotics and
> high-frequency systems.
>
> **Not to be confused with** other unrelated projects that use the name "Nyx" or the `.nyx` extension —
> most notably **NyxCode** (`nyxcode.io` / `heynyx.dev`), which is an unrelated **web full-stack DSL**
> (one `.nyx` file → HTML/CSS/JS + Express/SQLite, distributed as the `nyx` npm binary).
>
> They share **nothing** — no code, no maintainers, no organisation, no roadmap. Different domains,
> different grammars, different compilers.
>
> | | **Nyx (this project)** | NyxCode *(unrelated)* |
> |:--|:--|:--|
> | Domain | Systems / OS / embedded / real-time | Web full-stack / AI generation |
> | Compiler | `nyxc` | `nyx` (npm `@fabudde/nyxcode`) |
> | Owner | Simeon Bala — **9jaonCloud Engineering** | Fabian Budde |
> | Products | NyxOS, Nyx Mobile OS, Athena | nyxcode.io, NyxStatus |
>
> When searching or citing, prefer the unambiguous form: **"Nyx systems language (9jaonCloud)"** or
> **"NyxOS"**.

---

## 🏛️ Flagship Showcase: Athena Quantitative Trading Workstation

> **Built 100% in Nyx.** A full-featured institutional trading workstation running at **60–120 FPS** in a single standalone binary with zero external runtime dependencies.

```
┌─────────────────────────────────────────────────────────────────────────────────────────────┐
│ 📈 ATHENA INSTITUTIONAL TRADING TERMINAL (v0.26.0)                                          │
├─────────────────────────────────────────────────────────────────────────────────────────────┤
│ • Live 60 FPS Micro-Ticks: Real-time price fluctuations & green/red flash deltas             │
│ • Interactive Asset Switcher: GOOGL ($337.71), NVDA ($132.50), MSFT ($452.80), AAPL, AMZN    │
│ • High-Resolution Intraday Chart: 24-point continuous curve with volume & floating cursor   │
│ • Level 2 Orderbook Ladder: Real-time bid & ask volume depth with fast market dispatch      │
│ • StockVal Alpha & Graham Matrix: Compound growth, Margin of Safety & Confidence 8/10 gate  │
│ • Quantitative Signals Matrix: Master 0-9 Agreement Score, MACD (12/26/9), RSI-14 momentum  │
│ • Native Hardware Footprint: 650 KB binary, 0.00 ms GC pauses, full UTF-8 Unicode rendering │
└─────────────────────────────────────────────────────────────────────────────────────────────┘
```

**Run Athena on Windows:**
```powershell
.\examples\athena_workstation.exe
```

---

## 🚀 Why Developers Choose Nyx: 2026 Head-to-Head

| Metric | Nyx (v0.26.0) | Rust | Go | C++ | Flutter |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Memory Model** | **Region Bump (82.4%) + ARC** | Borrow Checker (`'a`) | Tracing GC | Manual `malloc/free` | Tracing Dart GC |
| **Max GC Pause Time** | **0.00 ms (Zero GC)** | 0.00 ms | 1.84 ms – 45 ms | 0.00 ms | 8.0 ms – 16.0 ms |
| **Cold Build Time** | **0.42 s (Clean)** | 2.61 s (6.2x slower) | 0.85 s | 1.10 s | 14.2 s |
| **Static Binary Size** | **~650 KB** | ~3.2 MB | ~14.2 MB | ~1.8 MB | ~28.5 MB |
| **Native GUI Engine** | **Built-in (Skia/GDI GPU)** | External (Slint/Iced) | Third-party bindings | Qt / ImGui | Built-in (Heavy) |
| **Hardware NPU Tensors** | **Native INT8/INT4 GEMM** | External bindings | External C bindings | LibTorch / ONNX | N/A |

👉 **Read the full benchmark analysis:** [2026 Head-to-Head Benchmark Matrix](https://nyx.9jaoncloud.com.ng/comparison.html)

---

## ⚡ 4-Step Quick Start Guide

### Step 1: Install or Build the Toolchain
```bash
# Clone the repository
git clone https://github.com/9jaoncloud/nyx.git
cd nyx

# Compile all production targets in one command
cargo run --manifest-path build_helper/Cargo.toml
```

### Step 2: Initialize a New Nyx Project
```bash
nypm init my_trading_app
cd my_trading_app
```

### Step 3: Write a Full-Stack Web API with Type-Safe ORM
```nyx
import std.http.app;
import std.db.orm;
import std.time;

fn main() {
    let app = App.new();
    app.use(middleware::cors());
    app.use(middleware::jwt_auth("secret_key"));

    app.get("/api/v1/stocks/:ticker", |req, res| {
        let ticker = req.param("ticker");
        let stock = Stock::query()
            .where("symbol", "=", ticker)
            .fetch_one()?;

        res.json({
            "ticker": ticker,
            "fair_value": stock.alpha_fair_value(),
            "timestamp": DateTime::now_utc().to_iso8601()
        });
    });

    app.listen(8080);
}
```

### Step 4: Run, Format & Lint in One Command
```bash
# Run with instant caching & zero GC pauses
nyx run src/main.nyx

# Format source code
nyx fmt

# Run static analysis linter
nyx lint
```

---

## 📦 Complete Tier 1 Standard Library Ecosystem

- **`std.time`**: ISO-8601 parsing/formatting, timezones (UTC, EST, PST, JST), Duration arithmetic.
- **`std.collections`**: PriorityQueue (Binary Max/Min Heap), Deque (Circular buffer), $O(1)$ LRUCache, HashSet.
- **`std.db.orm`**: Chainable query builder and declarative schema auto-migrator across SQLite, MySQL, Postgres, MSSQL.
- **`std.http.app`**: Express/FastAPI-style routing, parameter extraction (`:ticker`), middleware, HMAC-SHA256 JWT auth.
- **`std.net.actor`**: Distributed Raft consensus state replication & lock-free ring-buffer actor mailboxes.
- **`std.ml.npu`**: Native scale-calibrated INT8/INT4 GEMM tensor multiplication & local Transformer attention.
- **`std.ui`**: Google Material Design 3 120 FPS GPU rendering engine with Gaussian blur shadows and additive glows.
- **`std.sec`**: Enterprise AES-256-GCM encrypted vaults, NIDS packet inspection, and Ed25519 signing.
- **`std.gis`**: OGC vector geometry, WGS84 & Web Mercator CRS projections, DEM hillshading.
- **`std.cloud`**: Multi-cloud object storage (AWS S3 SigV4, GCP GCS, Azure Blob) & Connect RPC.

---

## 📜 Academic Research & Publications

- **PLDI 2026 Paper**: [*"Sound Region Inference and Escape-Directed Memory Compaction for Zero-GC Multi-Target Systems"*](https://nyx.9jaoncloud.com.ng/docs/language/memory.html)
- **LaTeX Source Package**: [`docs/paper/nyx_region_inference_pldi2026.tex`](docs/paper/nyx_region_inference_pldi2026.tex)
- **Honest Viability Assessment**: [`docs/assessment.html`](docs/assessment.html)
- **Response to Assessment**: [`docs/response-to-assessment.html`](docs/response-to-assessment.html)

---

## 📄 License
Licensed under the [MIT License](LICENSE).
