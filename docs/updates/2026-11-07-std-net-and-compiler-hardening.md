# Update 2026-11-07 — `std/net` hardening + compiler/runtime bug fixes

**Classification:** internal engineering log (offline, not deployed)
**Installed compiler at time of writing:** `nyx 0.54.0` (C-emitting bootstrap)
**Verification:** `tests/unit` 13/13 · `tests/test_continuum_memory.nyx` · `tests/probe_net*.nyx` all green

---

## Summary
`std/net.nyx` was made a reliable base (HTTP parsing, EOF reads, real `peer_addr`, a real RFC 6455
WebSocket client). Doing so exposed four latent bugs — two in the C-emitting compiler, two in the
runtime — that were silently corrupting networking code. All fixed.

---

## Issue 1 — `Result`/`Option` struct payloads dangled across `return` (hard crash)
- **Symptom:** a program that *compiled* crashed with `0xC0000005` (access violation) on first
  dereference of a `Result.Ok(struct)` payload (e.g. `tests/probe_net_use.nyx`).
- **Root cause:** `NyxResult`/`NyxOption` store the payload in an 8‑byte union (`data.ptr_val`).
  For a value‑struct payload the emitter wrote `(void*)({ T _stmp_ = …; &_stmp_; })` — the address
  of a **stack temporary inside the producing function**. After `return` (e.g. `TcpListener_bind`)
  it dangled; the receiving `match` read garbage.
- **Fix:** new `emit_boxed_payload()` heap-allocates the pointee
  (`T* _bx_ = malloc(sizeof(T)); *_bx_ = …; _bx_;`); routed all 5 value‑struct payload emitters
  through it.
- **Files:** `compiler/archive/v0.53.0/src/codegen_c.rs`, `.../v0.54.0/src/codegen_c.rs`.
- **Verify:** `probe_net_use` prints `127.0.0.1:0` and `Connection failed…` with no crash.

## Issue 2 — static `.get(...)` on a user struct lowered to `rt_map_get`
- **Symptom:** `NetHttpRequest.get(url)` emitted `rt_map_get(&(NetHttpRequest), …)` → C error
  (`NetHttpRequest undeclared`).
- **Root cause:** the "user method precedence" block deliberately **skips type‑name receivers**, so
  static associated calls fell through to the `"get"` Map intrinsic arm.
- **Fix:** in the `"get"` arm, if the receiver is a known struct/enum with a registered
  `<Type>_get`, emit `<Type>_get(args)` and return.
- **Files:** both `codegen_c.rs` trees.

## Issue 3 — `Vec<u8>` sized as `sizeof(int64_t)` (byte buffers truncated)
- **Symptom:** received TCP bytes read back as `"H"` (truncated at first NUL).
- **Root cause:** the `Vec.new()`/`vec(..)` elem‑size match had **no arm for integer widths**, so
  `u8` fell to `sizeof(int64_t)`.
- **Fix:** added `uint8_t/int8_t/…/int64_t/bool/size_t` arms → `sizeof(<type>)`.
- **Files:** both `codegen_c.rs` trees (`new` arm + `vec(..)` arm).
- **Note:** `v.push(72_u8)` still infers `int64_t`; prefer `let b: Vec<u8> = s.bytes()` (annotated).

## Issue 4 — `rt_string_substr` returned a non‑NUL‑terminated VIEW
- **Symptom:** URL‑parsed `connect()` always failed — host `127.0.0.1` became
  `127.0.0.1:9002/path` inside `getaddrinfo`.
- **Root cause:** `rt_string_substr` (used by `slice`/`split`) returned `{data = s.data+start,
  length}` with no NUL at `length`, violating the runtime's `data` == C‑string contract
  (`rt_string_as_str`). Raw C APIs read past the slice.
- **Fix:** return a **NUL‑terminated heap copy**. Added `rt_string_from_len(buf, len)` for raw
  buffers.
- **Files:** `runtime/rt_string.h` (header‑only; no `.a` rebuild needed).

## Issue 5 — `std/net.nyx` feature gaps (fixed)
- `read_to_string()` was a single `recv` → now loops to EOF (String concat, no `Vec<u8>` push).
- `http_send()` hard‑coded `200`/empty headers → now parses the status line + headers + URL.
- `peer_addr()` returned literal `"peer"` → now real `ip:port` via new `nyx_peer_addr` (`getpeername`).
- `WebSocket` had no handshake and sent **unmasked** frames → now real RFC 6455 handshake
  (`runtime/rt_ws.h`: SHA‑1/Base64/key), masked client frames (`nyx_ws_send`), unmasked receive
  (`nyx_ws_recv`).
- **Files:** `std/net.nyx`, `runtime/rt_net.c/.h`, `runtime/rt_ws.h` (new), `lib/libnyxrt.a`
  (rt_net.o member replaced; backup `lib/libnyxrt.a.bak-before-net-gaps`).

## Issue 6 — version drift: installed binary was building from `v0.53.0`
- **Symptom:** CHANGELOG says `0.54.0` is current, but `nyx --version` printed `0.53.0` and the
  `v0.54.0` tree silently lacked all prior fixes (building it regressed `std/net`).
- **Root cause:** `compiler/archive/v0.54.0/src/` was a **stale pre‑fix snapshot** (`codegen_c.rs`,
  `driver.rs`), and `main.rs` hard‑coded `version = "0.53.0"`.
- **Fix:** synced `v0.53.0 → v0.54.0` for `codegen_c.rs` + `driver.rs`; bumped the clap version to
  `0.54.0`; rebuilt and installed `nyx.exe` from `v0.54.0`.
- **Files:** `compiler/archive/v0.54.0/src/{codegen_c,driver,main}.rs`.

---

## Traps for the next engineer/AI
- **`Copy-Item` preserves mtime → `cargo build` silently no‑ops.** `touch` copied `.rs` files first.
- **Nyx parser: multi‑line conditions break** (`Expected LBrace, got Some(And)`). Keep `&&`/`||`
  conditions on **one line**.
- **`.get` / `default` / `switch` are reserved** method names (runtime intrinsics).
- Runtime **headers** are `static inline` (edit → no rebuild); `rt_*.c` changes need
  `gcc -Wall -O2 -c rt_net.c -o rt_net.o && ar r ../lib/libnyxrt.a rt_net.o`.

## Acceptance matrix
| Check | Result |
|---|---|
| `nyx --version` | `nyx 0.54.0` |
| `tests/unit/*.nyx` | 13/13 |
| `tests/probe_net.nyx` / `probe_net_use.nyx` | compile + run |
| `tests/probe_net_e2e.nyx` | TCP full message + UDP + `DONE` |
| `tests/probe_net_gaps.nyx` | HTTP `200`/6 headers/body · `peer=127.0.0.1:9002` · WS `payload=ping-ws` |
| `tests/test_continuum_memory.nyx` | pass |

---

## Native compiler (`nyxc`) rebuild — investigated; partial fix, NOT safe to install yet
**Bottom line:** the C-emitting compiler (`nyx.exe`) is updated and `std/net` works. The self-host
rebuild now *transpiles* but still has a runtime regression on one construct, so the known-good
`nyxc` (2026-10-27) is kept installed.

### What was fixed in this pass
- **Targeted payload boxing.** `emit_boxed_payload` now heap-boxes **only** the `std/net` structs
  (`TcpListener`, `TcpStream`, `TlsStream`, `UdpSocket`, `UdpPacket`, `WebSocket`,
  `WebSocketMessage`, `NetHttpRequest`, `NetHttpResponse`); every other struct payload keeps the
  previous stack-temp form. This **removed one whole class of nyxc crash** — the rebuilt nyxc now
  compiles `t_v2` (struct + struct-literal-returning fn) and `probe_net`.
- **Root-cause instrumented.** The generated `int main` had **no `setvbuf`**, so buffered stdout made
  crashes look like "no output"; forcing `_IONBF` (and using `eprintln`) localised the crash to the
  **typechecker** (`check_program`), not the lexer/parser.

### Remaining regression (repro)
Minimal: `tests/t_a.nyx` (kept in the issue log) —
```nyx
struct Vec3 { x: f32, y: f32, z: f32 }
fn main() { let v1 = Vec3 { x: 1.0, y: 2.0, z: 3.0 }; }
```
- Rebuilt nyxc: **0xC0000005**. The 2026-10-27 nyxc: **`Compilation successful`**.
- Trigger = a **struct literal with ≥2–3 fields bound via `let`** (1-field `let` works; struct literal
  in a `return` works; `let` of a scalar works). Also `let x: i64 = 1` emits a garbage type-error
  number (`- 2255349725792`) — related value/typing mishandling.
- **Bisected and ruled out:** `rt_string_substr` copy, the `Vec<u8>`/integer elem-size fix (Fix 18),
  and the heap-box path for these types. So the cause is among the earlier compiler fixes from the
  prior sessions (or the `driver.rs` self-type sync) — **not** this session's net work.
- **Action:** did NOT install the crashing build — restored `nyxc.exe` + `main.c` from
  `nyxc/_backup_20261107/`. Verified `nyxc.exe tests/vector_math_test.nyx --types` →
  `Compilation successful`.

### Verified final state
| Check | Result |
|---|---|
| `nyx --version` | `nyx 0.54.0` |
| `tests/unit/*.nyx` | 13/13 |
| `tests/probe_net` / `probe_net_use` / `probe_net_e2e` | compile + run (`net ok`, `127.0.0.1:0`, `DONE`) |
| `nyxc.exe` (2026-10-27) `tests/vector_math_test.nyx --types` | pass |

### Next step (focused follow-up)
Bisect the **prior-session** codegen fixes (one at a time, rebuild `nyx.exe`, re-transpile
`nyxc/main.nyx`, test `tests/t_a.nyx`) — suspect the struct-field / `AssignIndex` / Vec-elem-type
arms or the `driver.rs` `c_owner_type_str` self-type change. Do **not** install a rebuilt `nyxc`
until `t_a.nyx` and `vector_math_test.nyx` both compile.
