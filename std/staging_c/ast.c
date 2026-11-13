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

#define TAG_BinOp_Add 0
#define TAG_BinOp_Sub 1
#define TAG_BinOp_Mul 2
#define TAG_BinOp_Div 3
#define TAG_BinOp_Mod 4
#define TAG_BinOp_Eq 5
#define TAG_BinOp_NotEq 6
#define TAG_BinOp_Lt 7
#define TAG_BinOp_LtEq 8
#define TAG_BinOp_Gt 9
#define TAG_BinOp_GtEq 10
#define TAG_BinOp_And 11
#define TAG_BinOp_Or 12
typedef struct {
    int tag;
    union {
    } data;
} BinOp;

#define TAG_Type_Int 0
#define TAG_Type_Float 1
#define TAG_Type_Bool 2
#define TAG_Type_String 3
#define TAG_Type_Char 4
#define TAG_Type_Void 5
#define TAG_Type_Custom 6
#define TAG_Type_Generic 7
#define TAG_Type_Ref 8
#define TAG_Type_MutRef 9
#define TAG_Type_Array 10
#define TAG_Type_Option 11
#define TAG_Type_Result 12
#define TAG_Type_Tuple 13
#define TAG_Type_Func 14
#define TAG_Type_Region 15
#define TAG_Type_Recursive 16
#define TAG_Type_Unknown 17
typedef struct Type Type;
struct Type {
    int tag;
    union {
        struct { rt_string_t f0; } Custom;
        struct { rt_string_t f0; } Generic;
        struct { Type* f0; } Ref;
        struct { Type* f0; } MutRef;
        struct { Type* f0; } Array;
        struct { Type* f0; } Option;
        struct { Type* f0; Type* f1; } Result;
        struct { NyxVec f0; } Tuple;
        struct { NyxVec f0; Type* f1; } Func;
        struct { rt_string_t f0; } Region;
        struct { rt_string_t f0; } Recursive;
    } data;
};

#define TAG_Literal_Int 0
#define TAG_Literal_Float 1
#define TAG_Literal_Bool 2
#define TAG_Literal_String 3
#define TAG_Literal_Char 4
typedef struct {
    int tag;
    union {
        struct { int64_t f0; } Int;
        struct { double f0; } Float;
        struct { int f0; } Bool;
        struct { rt_string_t f0; } String;
        struct { char f0; } Char;
    } data;
} Literal;

#define TAG_Pattern_Ident 0
#define TAG_Pattern_Wildcard 1
#define TAG_Pattern_Lit 2
#define TAG_Pattern_Enum 3
typedef struct {
    int tag;
    union {
        struct { rt_string_t f0; } Ident;
        struct { Literal f0; } Lit;
        struct { rt_string_t f0; rt_string_t f1; NyxVec f2; } Enum;
    } data;
} Pattern;

#define TAG_UnaryOp_Not 0
#define TAG_UnaryOp_Neg 1
#define TAG_UnaryOp_Ref 2
#define TAG_UnaryOp_Deref 3
typedef struct {
    int tag;
    union {
    } data;
} UnaryOp;

#define TAG_Expr_Lit 0
#define TAG_Expr_Ident 1
#define TAG_Expr_Binary 2
#define TAG_Expr_Call 3
#define TAG_Expr_MethodCall 4
#define TAG_Expr_FieldAccess 5
#define TAG_Expr_StructLiteral 6
#define TAG_Expr_Unary 7
#define TAG_Expr_If 8
#define TAG_Expr_While 9
#define TAG_Expr_For 10
#define TAG_Expr_Match 11
#define TAG_Expr_Lambda 12
#define TAG_Expr_Return 13
#define TAG_Expr_Await 14
#define TAG_Expr_Block 15
#define TAG_Expr_Let 16
#define TAG_Expr_Assign 17
#define TAG_Expr_Question 18
typedef struct Expr Expr;
struct Expr {
    int tag;
    union {
        struct { Literal f0; } Lit;
        struct { rt_string_t f0; } Ident;
        struct { BinOp f0; Expr* f1; Expr* f2; } Binary;
        struct { Expr* f0; NyxVec f1; NyxVec f2; } Call;
        struct { Expr* f0; rt_string_t f1; NyxVec f2; NyxVec f3; } MethodCall;
        struct { Expr* f0; rt_string_t f1; } FieldAccess;
        struct { rt_string_t f0; NyxVec f1; NyxVec f2; } StructLiteral;
        struct { UnaryOp f0; Expr* f1; } Unary;
        struct { Expr* f0; NyxVec f1; NyxVec f2; NyxOption f3; } If;
        struct { Expr* f0; NyxVec f1; } While;
        struct { rt_string_t f0; Expr* f1; NyxVec f2; } For;
        struct { Expr* f0; NyxVec f1; } Match;
        struct { NyxVec f0; Expr* f1; } Lambda;
        struct { NyxOption f0; } Return;
        struct { Expr* f0; } Await;
        struct { NyxVec f0; } Block;
        struct { rt_string_t f0; Expr* f1; NyxOption f2; int f3; } Let;
        struct { rt_string_t f0; Expr* f1; } Assign;
        struct { Expr* f0; } Question;
    } data;
};

#define TAG_Stmt_Let 0
#define TAG_Stmt_Expr 1
#define TAG_Stmt_Return 2
typedef struct {
    int tag;
    union {
        struct { rt_string_t f0; int f1; NyxOption f2; Expr f3; } Let;
        struct { Expr f0; } Expr;
        struct { NyxOption f0; } Return;
    } data;
} Stmt;

#define TAG_TopLevel_FnDecl 0
#define TAG_TopLevel_StructDef 1
#define TAG_TopLevel_EnumDef 2
#define TAG_TopLevel_ImplBlock 3
#define TAG_TopLevel_TraitDef 4
#define TAG_TopLevel_TypeAlias 5
#define TAG_TopLevel_ConstDecl 6
#define TAG_TopLevel_ModuleDecl 7
#define TAG_TopLevel_UseDecl 8
#define TAG_TopLevel_ExternBlock 9
#define TAG_TopLevel_Module 10
#define TAG_TopLevel_Import 11
typedef struct {
    int tag;
    union {
        struct { rt_string_t f0; NyxVec f1; NyxVec f2; Type f3; Expr f4; int f5; } FnDecl;
        struct { rt_string_t f0; NyxVec f1; NyxVec f2; } StructDef;
        struct { rt_string_t f0; NyxVec f1; NyxVec f2; } EnumDef;
        struct { rt_string_t f0; NyxVec f1; NyxVec f2; } ImplBlock;
        struct { rt_string_t f0; NyxVec f1; NyxVec f2; } TraitDef;
        struct { rt_string_t f0; Type f1; } TypeAlias;
        struct { rt_string_t f0; Type f1; Expr f2; } ConstDecl;
        struct { rt_string_t f0; } ModuleDecl;
        struct { NyxVec f0; } UseDecl;
        struct { rt_string_t f0; NyxVec f1; } ExternBlock;
        struct { rt_string_t f0; } Module;
        struct { NyxVec f0; } Import;
    } data;
} TopLevel;

typedef struct {
    Pattern pattern;
    NyxOption guard;
    Expr body;
} MatchArm;

typedef struct {
    rt_string_t name;
    Type ty;
} Param;

typedef struct {
    rt_string_t name;
    NyxVec generics;
    NyxVec fields;
} StructDef;


#line 1 "ast.nyx"
// module ast
