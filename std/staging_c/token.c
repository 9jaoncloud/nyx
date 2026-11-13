#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "runtime/rt_arena.h"
#include "runtime/rt_string.h"
#include "runtime/rt_io.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_option.h"
#include "runtime/rt_map.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_async.h"
#include "runtime/rt_json.h"
#include "runtime/rt_db.h"
#include "runtime/rt_http.h"
#include "runtime/rt_net.h"
#include "runtime/rt_crypto.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_time.h"
#include "runtime/rt_nursery.h"
#include "runtime/rt_collections.h"
#include "runtime/rt_math.h"
#include "runtime/rt_format.h"
#include "runtime/rt_thread.h"
#include "runtime/rt_atomic.h"
#include "runtime/nyx_generational.h"
#include "include/nyx_multidialect.h"
#ifndef NYX_CURRENT_CONTEXT_DEFINED
#define NYX_CURRENT_CONTEXT_DEFINED
/* Odin-inspired Native Execution Context */
typedef struct NyxContext {
    void* nursery;
    void* temp_nursery;
    uint64_t thread_id;
    void (*logger)(const char* msg);
    void* user_data;
} NyxContext;
#if defined(_MSC_VER)
__declspec(selectany) __declspec(thread) NyxContext nyx_current_context = {0};
#elif defined(__GNUC__) || defined(__clang__)
__attribute__((weak)) __thread NyxContext nyx_current_context = {0};
#else
static __thread NyxContext nyx_current_context = {0};
#endif
#endif
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#ifndef NYX_TUPLE_DEFS
#define NYX_TUPLE_DEFS
typedef struct { void* f0; void* f1; } NyxTuple2;
typedef struct { void* f0; void* f1; void* f2; } NyxTuple3;
typedef struct { void* f0; void* f1; void* f2; void* f3; } NyxTuple4;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; } NyxTuple5;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; } NyxTuple6;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; } NyxTuple7;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; void* f7; } NyxTuple8;
#endif

#define TAG_TokenKind_Ident 0
#define TAG_TokenKind_IntLit 1
#define TAG_TokenKind_FloatLit 2
#define TAG_TokenKind_StringLit 3
#define TAG_TokenKind_True 4
#define TAG_TokenKind_False 5
#define TAG_TokenKind_Fn 6
#define TAG_TokenKind_Let 7
#define TAG_TokenKind_Mut 8
#define TAG_TokenKind_If 9
#define TAG_TokenKind_Else 10
#define TAG_TokenKind_While 11
#define TAG_TokenKind_For 12
#define TAG_TokenKind_In 13
#define TAG_TokenKind_Return 14
#define TAG_TokenKind_Struct 15
#define TAG_TokenKind_Enum 16
#define TAG_TokenKind_Match 17
#define TAG_TokenKind_Trait 18
#define TAG_TokenKind_Impl 19
#define TAG_TokenKind_Import 20
#define TAG_TokenKind_Module 21
#define TAG_TokenKind_Pub 22
#define TAG_TokenKind_Async 23
#define TAG_TokenKind_Await 24
#define TAG_TokenKind_Yield 25
#define TAG_TokenKind_Extern 26
#define TAG_TokenKind_SelfKw 27
#define TAG_TokenKind_SelfType 28
#define TAG_TokenKind_Parallel 29
#define TAG_TokenKind_Null 30
#define TAG_TokenKind_LParen 31
#define TAG_TokenKind_RParen 32
#define TAG_TokenKind_LBrace 33
#define TAG_TokenKind_RBrace 34
#define TAG_TokenKind_LBracket 35
#define TAG_TokenKind_RBracket 36
#define TAG_TokenKind_Comma 37
#define TAG_TokenKind_Semicolon 38
#define TAG_TokenKind_Colon 39
#define TAG_TokenKind_Arrow 40
#define TAG_TokenKind_FatArrow 41
#define TAG_TokenKind_Equals 42
#define TAG_TokenKind_Plus 43
#define TAG_TokenKind_Minus 44
#define TAG_TokenKind_Star 45
#define TAG_TokenKind_Slash 46
#define TAG_TokenKind_Percent 47
#define TAG_TokenKind_EqEq 48
#define TAG_TokenKind_NotEq 49
#define TAG_TokenKind_Lt 50
#define TAG_TokenKind_LtEq 51
#define TAG_TokenKind_Gt 52
#define TAG_TokenKind_GtEq 53
#define TAG_TokenKind_And 54
#define TAG_TokenKind_Or 55
#define TAG_TokenKind_Not 56
#define TAG_TokenKind_Dot 57
#define TAG_TokenKind_DotDot 58
#define TAG_TokenKind_PathSep 59
#define TAG_TokenKind_Ampersand 60
#define TAG_TokenKind_Pipe 61
#define TAG_TokenKind_Question 62
#define TAG_TokenKind_Underscore 63
#define TAG_TokenKind_EOF 64
typedef struct {
    int tag;
    union {
        struct { rt_string_t f0; } Ident;
        struct { int64_t f0; } IntLit;
        struct { double f0; } FloatLit;
        struct { rt_string_t f0; } StringLit;
    } data;
} TokenKind;

typedef struct {
    TokenKind kind;
    int64_t line;
    int64_t col;
} Token;


#line 1 "token.nyx"
// module token
