# Nyx Async Runtime Design

**Status:** Draft — Phase 1 (single-threaded event loop)
**Last updated:** July 2025

---

## Problem Statement

The Nyx spec promises `async fn`, `await`, structured concurrency, and M:N work-stealing. The current implementation is a 42-line sequential runner (`rt_async.c`) that ignores `num_threads` and executes tasks in order. `async fn` is parsed but the flag is discarded during codegen. `await` compiles to a no-op. This document designs the minimum viable async runtime that makes `async fn` / `await` actually work, with a path to the full M:N model.

## Design Decisions

### 1. Compilation Model: Closure Iterators (Nim-style)

**Why not Rust-style enum state machines?**
- Rust's model requires `Pin<Self>` for self-referential futures — complex lifetime interactions
- Rust's `Future` trait + `Waker` + `Context` is powerful but heavyweight for a C-targeting language
- The borrow checker in async contexts is a major source of complexity for Rust users

**Why not Go-style stackful goroutines?**
- Requires stack growth, copying, and a sophisticated scheduler from day one
- 2KB initial stacks need careful tuning per platform
- M:N scheduling is the hardest part — we want to defer it

**Why closure iterators (Nim-style)?**
- Compiles to straightforward C: `switch(state)` + local variables in a heap-allocated env struct
- No self-referential structs — the env is a plain C struct
- No `Pin`, no lifetimes in the async system
- Sequential code with yield points — simple mental model
- Proven in production (Nim, also similar to C++ coroutine frame approach)

### 2. Runtime: Single-Threaded Event Loop (Phase 1)

Phase 1 is a single-threaded cooperative event loop. No threads, no work-stealing. This is the minimum to make `async fn` / `await` work for I/O-bound code.

Phase 2 (later) adds a thread pool and work-stealing for CPU-bound parallelism.

### 3. No Function Coloring (Phase 1)

In Phase 1, `await` is the only yield point. Blocking and async functions can interoperate freely. The event loop runs on the calling thread. This avoids the "colored function" problem where async infects the entire call stack.

---

## Phase 1: Single-Threaded Event Loop

### What `async fn` Compiles To

```nyx
async fn fetch_two_pages() -> String {
    let page1 = await http_get("a")
    let page2 = await http_get("b")
    return page1 + "\n" + page2
}
```

Compiles to C:

```c
// The async function becomes a closure iterator
typedef struct FetchTwoPagesEnv {
    int state;                  // 0 = initial, 1 = waiting page1, 2 = waiting page2
    NyxFuture page1_fut;        // In-flight future
    NyxFuture page2_fut;        // In-flight future
    rt_string_t page1;          // Local variable across await point
    rt_string_t page2;          // Local variable across await point
} FetchTwoPagesEnv;

NyxFuture fetch_two_pages_iter(FetchTwoPagesEnv* env) {
    switch (env->state) {
    case 0:
        // First call: start page1 fetch
        env->page1_fut = http_get("a");
        env->state = 1;
        return env->page1_fut;  // Yield: caller will poll this
    case 1:
        // Resumed: page1 is ready
        env->page1 = nyx_future_get_string(&env->page1_fut);
        env->page2_fut = http_get("b");
        env->state = 2;
        return env->page2_fut;  // Yield: caller will poll this
    case 2:
        // Resumed: page2 is ready
        env->page2 = nyx_future_get_string(&env->page2_fut);
        rt_string_t result = rt_string_concat(env->page1, rt_string_from("\n"));
        result = rt_string_concat(result, env->page2);
        return nyx_future_ok(result);
    }
    return nyx_future_err("unreachable");
}
```

### Key Data Structures

```c
// ---- Future ----
typedef enum { NYX_FUTURE_PENDING, NYX_FUTURE_READY, NYX_FUTURE_ERROR } NyxFutureState;

typedef struct NyxFuture NyxFuture;
typedef void (*NyxFutureResumeFn)(NyxFuture*);

struct NyxFuture {
    NyxFutureState state;
    union {
        void* value;        // Typed via generic (string, int, etc.)
        NyxError error;
    };
    void* env;              // Closure iterator environment
    NyxFutureResumeFn resume; // Resume function (the iterator)
};

// ---- Event Loop ----
typedef struct NyxEventLoop {
    NyxFuture** tasks;      // Array of pending futures
    int task_count;
    int task_capacity;
    // I/O backend (platform-specific)
    #ifdef _WIN32
    HANDLE iocp;
    #else
    int epoll_fd;
    #endif
} NyxEventLoop;

// ---- Task ----
typedef struct NyxTask {
    NyxFuture future;
    int priority;
    int ready;              // 1 if ready to poll
} NyxTask;
```

### Event Loop Operations

```c
// Create/destroy
NyxEventLoop* nyx_event_loop_new(void);
void nyx_event_loop_free(NyxEventLoop* loop);

// Submit a future to the event loop
void nyx_event_loop_submit(NyxEventLoop* loop, NyxFuture future);

// Run until all futures complete
void nyx_event_loop_run(NyxEventLoop* loop);

// Internal: poll I/O, resume ready futures
void nyx_event_loop_tick(NyxEventLoop* loop);

// I/O registration (for async I/O)
int nyx_io_read_async(int fd, void* buf, size_t len, NyxFuture* completion);
int nyx_io_write_async(int fd, const void* buf, size_t len, NyxFuture* completion);
```

### How `await` Works

```
┌──────────────────────────────────────────────────────────┐
│  await expr                                              │
│                                                          │
│  1. Evaluate expr → produces a Future                    │
│  2. If future.state == READY:                            │
│     → Extract value, continue execution                  │
│  3. If future.state == PENDING:                          │
│     → Store current state in env                         │
│     → Return future to event loop                        │
│     → Event loop will poll I/O and resume when ready     │
│                                                          │
│  This is cooperative: the function suspends at yield     │
│  points and resumes when the awaited future completes.   │
└──────────────────────────────────────────────────────────┘
```

### `spawn` in Phase 1

In Phase 1, `spawn` adds a future to the event loop's task queue. All tasks run on one thread, interleaved at yield points:

```nyx
spawn http_server.listen(8080)   // Adds to event loop
spawn db.connect(conn_str)       // Adds to event loop
event_loop.run()                 // Runs all tasks cooperatively
```

### `parallel` in Phase 1

`parallel` does not exist in Phase 1. Documented as "coming in Phase 2."

---

## Phase 2: M:N Threading (Future)

Phase 2 adds true parallelism. This is a separate design effort.

### Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    NYX ASYNC PHASE 2                     │
│                                                          │
│  ┌─────────────┐   ┌─────────────┐   ┌─────────────┐   │
│  │  Thread 1   │   │  Thread 2   │   │  Thread N   │   │
│  │  (OS thread)│   │  (OS thread)│   │  (OS thread)│   │
│  └──────┬──────┘   └──────┬──────┘   └──────┬──────┘   │
│         │                 │                 │           │
│  ┌──────▼──────┐   ┌──────▼──────┐   ┌──────▼──────┐   │
│  │  Queue 1    │   │  Queue 2    │   │  Queue N    │   │
│  │  (local)    │   │  (local)    │   │  (local)    │   │
│  └──────┬──────┘   └──────┬──────┘   └──────┬──────┘   │
│         │                 │                 │           │
│         └────────────┬────┘─────────────────┘           │
│                      │                                  │
│              ┌───────▼───────┐                          │
│              │  Work Stealing │                          │
│              │  (when idle)   │                          │
│              └───────────────┘                          │
│                                                          │
│  GMP Model:                                              │
│  G = NyxTask (green task)                                │
│  M = OS Thread (pthreads / Win32 threads)                │
│  P = Processor (thread-local queue + stealing)           │
└─────────────────────────────────────────────────────────┘
```

### Key Components (Phase 2)

1. **Work-stealing deque** per thread ( Chase-Lev or similar)
2. **Thread pool** (configurable M threads)
3. **Task migration** (steal from busy queues)
4. **IOCP/epoll integration** per thread
5. **Structured concurrency** (nursery/scope pattern)

### Structured Concurrency

```nyx
async fn handle_client(conn: Connection) {
    spawn_nursery {
        | nursery |
        nursery.spawn read_loop(conn)
        nursery.spawn write_loop(conn)
        // Both tasks guaranteed to complete before handle_client returns
    }
}
```

---

## Migration Path: Phase 1 → Phase 2

Phase 1 code should work unchanged in Phase 2:

| Phase 1 construct | Phase 2 behavior |
|---|---|
| `spawn task` | Task goes to local queue, may be stolen |
| `await fut` | Same semantics, may resume on different thread |
| `event_loop.run()` | Replaced by thread pool scheduler |
| Single-threaded | Multi-threaded with work-stealing |

The only breaking change: Phase 2 may execute concurrent tasks on different threads, so shared mutable state needs synchronization. This is already required by the language spec (ownership rules).

---

## Implementation Plan

### Step 1: Fix `async fn` / `await` codegen (now)

- `async fn` flag: don't discard it — generate closure iterator
- `await` expression: generate state machine code
- `Future[T]` type: add to type system

### Step 2: Minimal runtime (rt_async.c rewrite)

- `NyxFuture` struct
- `NyxEventLoop` with task queue
- Platform-specific I/O (epoll/kqueue/IOCP)
- `nyx_event_loop_run()` cooperative scheduler

### Step 3: `spawn` keyword

- Parse `spawn expr` as a statement
- Codegen: submit future to event loop
- Typecheck: validate spawned expression returns a Future

### Step 4: Structured concurrency (nursery)

- Parse `spawn_nursery { |n| ... }` pattern
- Implement task group with scope-based join
- Error propagation from child tasks

### Step 5: Thread pool (Phase 2)

- M:N scheduler with work-stealing
- `parallel` keyword for data-parallel loops
- Thread-safe channels and synchronization

---

## References

- [Nim async/await design](https://nim-lang.org/docs/asyncfile.html)
- [Rust Future trait](https://doc.rust-lang.org/std/future/trait.Future.html)
- [Go scheduler design](https://go.dev/blog/scheduler)
- [Zig async (pre-0.13)](https://ziglang.org/documentation/master/#async-await)
- [HHVM async HHVM](https://hhvm.com/) — region-based JIT with async
- [Region-Based Compilation (UIUC)](https://courses.engr.illinois.edu/cs422/sp2018/lectures/13-regions.pdf)
