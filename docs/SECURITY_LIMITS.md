# Nyx Security Limits: Formal Guarantees & Non-Guarantees

**Target Audience:** Chief Information Security Officers (CISOs), Enterprise Security Architects, Defense Auditors, and Compliance Teams.

This document sets out the cryptographic, formal, and operational boundary of what the **Nyx Programming Language** guarantees by design, and what remains outside the scope of a general-purpose programming language.

---

## 1. What Nyx Formally Guarantees

### A. Mathematical Memory Safety by Default (All Nyx Code)
Nyx’s compile-time **Region Inference Engine** eliminates entire classes of critical vulnerabilities at compile time with **zero runtime garbage-collection jitter**:
- **Zero Spatial Memory Corruption:** No buffer overflows, out-of-bounds indexing, or stack-smashing (enforced via fat-pointer metadata `(ptr, len, cap)`).
- **Zero Temporal Memory Corruption:** No use-after-free, double-free, or uninitialized memory reads (enforced via lexical arena lifetimes).
- **Zero Data Races in Safe Code:** Concurrency boundaries are verified at compile time with structured channels and SPSC rings.

### B. Linear Capability Boundaries (Havilah `.hav` Dialect)
When using the Havilah capability dialect:
- System resources (`CAP_NET`, `CAP_FILE`, `CAP_MEM`, `CAP_SPAWN`, `CAP_SENSORY`) are **linear, non-forgeable tokens**.
- Capabilities cannot be duplicated or leaked outside their authorized calling membrane.

### C. Mathematical SMT Invariants (Jude `.jude` Dialect)
When using the Jude formal verification dialect:
- Annotated invariants (`@verify(...)`) are mathematically discharged against the Z3 SMT solver.
- Certified properties (e.g. `torque <= 180.0`, `array_index < BUFFER_MAX`) hold for **100% of reachable states**.

### D. Kernel-Level Isolation (When `nyx-sandbox` Is Invoked)
When executing via `nyx run --sandbox=policy.toml`:
- Isolation is enforced directly by host kernel primitives:
  - **Linux:** Landlock LSM + Seccomp-BPF + cgroups v2.
  - **macOS:** `sandbox_init` seatbelt profiles.
  - **Windows:** AppContainer + Restricted Token + Job Objects.

### E. Cryptographic Provenance (When `--emit-sbom` Is Invoked)
When compiling with `nyxc build --emit-sbom`:
- Binaries are emitted alongside CycloneDX 1.5 `.nyx.sbom` manifests signed with Ed25519 author keys, recording build flags, dependency hashes, and static capability footprints.

---

## 2. What Nyx Does NOT Guarantee

### 1. Developer Intent & Logic Correctness
Memory safety ensures the program executes cleanly; it does **not** guarantee that the business logic computes the intended result. A function that calculates an incorrect tax or cryptographic salt is memory-safe but logically flawed.

### 2. Prevention of Intentional Malicious Misuse
Any general-purpose language capable of sending network packets or writing files can be used to construct malicious software. Nyx cannot prevent an attacker from writing an HTTP beacon or exfiltration tool, just as C, Rust, or Python cannot. Nyx provides tools for **defenders** to analyze, sandbox, and constrain untrusted software.

### 3. Automatic Defense Against Supply-Chain Trojans
If an application imports a malicious third-party dependency from an untrusted author without auditing, that dependency will execute with whatever permissions the host grants it. Developers must use `--emit-sbom`, Jude certificates, and `nyx-sandbox` to restrict third-party risks.

### 4. Side-Channel Resistance Without Discipline
While Nyx eliminates memory corruption, cryptographic implementations require constant-time arithmetic to prevent timing and cache side-channels (provided in `std.crypto`).

### 5. Foreign Function Interface (FFI) Soundness
When calling raw external C/C++ libraries via `extern "C"`, the external C code is executed outside Nyx's safe memory model.

---

## 3. Recommended Security Posture

| Risk Level | Recommended Nyx Security Toolchain |
| :--- | :--- |
| **Standard Applications** | Native `nyxc` compilation (Default zero-GC memory safety). |
| **High-Integrity Infrastructure** | `nyxc build --emit-sbom` + signed Ed25519 provenance manifests. |
| **Safety-Critical Systems** | Jude `.jude` formal verification (`--verify --emit-certificates`). |
| **Untrusted Plugins & AI Agents** | `nyx-sandbox --policy=strict_isolation.toml`. |
