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
#include "runtime/rt_concurrency.h"
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

#define TAG_UnaryOp_Not 0
#define TAG_UnaryOp_Neg 1
#define TAG_UnaryOp_Ref 2
#define TAG_UnaryOp_Deref 3
typedef struct {
    int tag;
    union {
    } data;
} UnaryOp;

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

// module ast
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

// module token
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

typedef struct {
    rt_string_t source;
    int64_t pos;
    int64_t line;
    int64_t col;
} Lexer;

Lexer new_lexer(rt_string_t);
Token Lexer_next_token(Lexer*);
void Lexer_advance(Lexer*);
char Lexer_peek(Lexer*);
void Lexer_skip_whitespace(Lexer*);
Token Lexer_read_number(Lexer*);
Token Lexer_read_string(Lexer*);
Token Lexer_read_ident(Lexer*);
// module lexer
// import token::*
// import std::string
Lexer new_lexer(rt_string_t source) {
return (Lexer){.source = source, .pos = 0LL, .line = 1LL, .col = 1LL};
}

Token Lexer_next_token(Lexer* self) {
Lexer_skip_whitespace(self);
({ if (((*self).pos >= (*self).source.length)) {
return (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_EOF }, .line = (*self).line, .col = (*self).col};
}
});
char ch = (*self).source.data[(*self).pos];
return ({
Token __match_result_1;
char __match_val_1 = ch;
if (__match_val_1 == '(') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_LParen }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == ')') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_RParen }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == '{') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_LBrace }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == '}') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_RBrace }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == '[') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_LBracket }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == ']') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_RBracket }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == ',') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Comma }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == ';') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Semicolon }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == ':') {
if ((Lexer_peek(self) == ':')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_PathSep }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Colon }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '=') {
if ((Lexer_peek(self) == '=')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_EqEq }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else if ((Lexer_peek(self) == '>')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_FatArrow }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Equals }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '+') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Plus }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == '-') {
if ((Lexer_peek(self) == '>')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Arrow }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Minus }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '*') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Star }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == '/') {
if ((Lexer_peek(self) == '/')) {
while ((((*self).pos < (*self).source.length) && ((*self).source.data[(*self).pos] != '\n'))) {
Lexer_advance(self);
}
;
__match_result_1 = Lexer_next_token(self);
} else if ((Lexer_peek(self) == '*')) {
Lexer_advance(self);
Lexer_advance(self);
while (((*self).pos < (*self).source.length)) {
({ if ((((*self).source.data[(*self).pos] == '*') && (Lexer_peek(self) == '/'))) {
Lexer_advance(self);
Lexer_advance(self);
break;
}
});
Lexer_advance(self);
}
;
__match_result_1 = Lexer_next_token(self);
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Slash }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '%') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Percent }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == '<') {
if ((Lexer_peek(self) == '=')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_LtEq }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Lt }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '>') {
if ((Lexer_peek(self) == '=')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_GtEq }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Gt }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '&') {
Lexer_advance(self);
if ((Lexer_peek(self) == '&')) {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_And }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Ampersand }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '|') {
Lexer_advance(self);
if ((Lexer_peek(self) == '|')) {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Or }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Pipe }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '!') {
if ((Lexer_peek(self) == '=')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_NotEq }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Not }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '.') {
if ((Lexer_peek(self) == '.')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_DotDot }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Dot }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '?') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Question }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == '_') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Underscore }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (0) {
/* range pattern */
}
 else if (__match_val_1 == '\"') {
__match_result_1 = Lexer_read_string(self);
}
 else if (0 || 0) {
__match_result_1 = Lexer_read_ident(self);
}
 else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_EOF }, .line = (*self).line, .col = (*self).col};
}
__match_result_1;
});
}

void Lexer_advance(Lexer* self) {
({ if (((*self).source.data[(*self).pos] == '\n')) {
(self->line = ((*self).line + 1LL));
(self->col = 1LL);
} else {
(self->col = ((*self).col + 1LL));
}
});
(self->pos = ((*self).pos + 1LL));
}

char Lexer_peek(Lexer* self) {
if ((((*self).pos + 1LL) < (*self).source.length)) {
return (*self).source.data[((*self).pos + 1LL)];
} else {
return '\0';
}
}

void Lexer_skip_whitespace(Lexer* self) {
while (((*self).pos < (*self).source.length)) {
char ch = (*self).source.data[(*self).pos];
({ if (((((ch == ' ') || (ch == '\t')) || (ch == '\\')) || (ch == '\n'))) {
Lexer_advance(self);
} else {
break;
}
});
}
;
}

Token Lexer_read_number(Lexer* self) {
int64_t line = (*self).line;
int64_t col = (*self).col;
rt_string_t num_str = rt_string_from("");
int is_float = 0;
while (((*self).pos < (*self).source.length)) {
char ch = (*self).source.data[(*self).pos];
({ Token __if_res_6; if (((ch >= '0') && (ch <= '9'))) {
num_str = rt_string_concat(num_str, ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = ch; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; }));
Lexer_advance(self);
} else if (((ch == '.') && (!is_float))) {
is_float = 1;
num_str = rt_string_concat(num_str, ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = ch; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; }));
Lexer_advance(self);
} else {
break;
}
__if_res_6; });
}
;
if (is_float) {
return (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_FloatLit, .data.FloatLit = {.f0 = 0} }, .line = line, .col = col};
} else {
int64_t val = (int64_t)(0LL);
for (int64_t i = 0LL; i < num_str.length; i++) {
char ch = num_str.data[i];
val = ((val * 10LL) + ((int64_t)(ch) - (int64_t)('0')));
}
;
return (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_IntLit, .data.IntLit = {.f0 = val} }, .line = line, .col = col};
}
}

Token Lexer_read_string(Lexer* self) {
int64_t line = (*self).line;
int64_t col = (*self).col;
Lexer_advance(self);
rt_string_t content = rt_string_from("");
while ((((*self).pos < (*self).source.length) && ((*self).source.data[(*self).pos] != '\"'))) {
({ Token __if_res_7; if (((*self).source.data[(*self).pos] == '\\')) {
Lexer_advance(self);
({ if (((*self).pos < (*self).source.length)) {
char escaped = (*self).source.data[(*self).pos];
({
rt_string_t __match_result_8;
char __match_val_8 = escaped;
if (__match_val_8 == 'n') {
__match_result_8 = (content = rt_string_concat(content, rt_string_from("\\n")));
}
 else if (__match_val_8 == 't') {
__match_result_8 = (content = rt_string_concat(content, rt_string_from("\\t")));
}
 else if (__match_val_8 == 'r') {
__match_result_8 = (content = rt_string_concat(content, rt_string_from("\\r")));
}
 else if (__match_val_8 == '\\') {
__match_result_8 = (content = rt_string_concat(content, rt_string_from("\\\\")));
}
 else if (__match_val_8 == '\"') {
__match_result_8 = (content = rt_string_concat(content, rt_string_from("\\\"")));
}
 else if (__match_val_8 == '0') {
__match_result_8 = (content = rt_string_concat(content, rt_string_from("\\0")));
}
 else {
__match_result_8 = (content = rt_string_concat(content, ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = escaped; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; })));
}
__match_result_8;
});
Lexer_advance(self);
}
});
} else {
content = rt_string_concat(content, ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = (*self).source.data[(*self).pos]; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; }));
Lexer_advance(self);
}
__if_res_7; });
}
;
({ if (((*self).pos < (*self).source.length)) {
Lexer_advance(self);
}
});
return (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_StringLit, .data.StringLit = {.f0 = content} }, .line = line, .col = col};
}

Token Lexer_read_ident(Lexer* self) {
int64_t line = (*self).line;
int64_t col = (*self).col;
rt_string_t word = rt_string_from("");
while (((*self).pos < (*self).source.length)) {
char ch = (*self).source.data[(*self).pos];
({ Token __if_res_11; if ((((((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'))) || ((ch >= '0') && (ch <= '9'))) || (ch == '_'))) {
word = rt_string_concat(word, ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = ch; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; }));
Lexer_advance(self);
} else {
break;
}
__if_res_11; });
}
;
return ({
Token __match_result_11;
rt_string_t __match_val_11 = word;
if (rt_string_eq(__match_val_11, rt_string_from("fn"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Fn }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("let"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Let }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("mut"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Mut }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("if"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_If }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("else"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Else }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("while"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_While }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("for"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_For }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("in"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_In }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("return"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Return }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("struct"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Struct }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("enum"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Enum }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("match"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Match }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("trait"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Trait }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("impl"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Impl }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("import"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Import }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("module"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Module }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("pub"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Pub }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("async"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Async }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("await"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Await }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("true"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_True }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("false"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_False }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("self"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_SelfKw }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("null"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Null }, .line = line, .col = col};
}
 else {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Ident, .data.Ident = {.f0 = word} }, .line = line, .col = col};
}
__match_result_11;
});
}

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

typedef struct {
    Lexer lexer;
    Token current;
    int had_error;
    NyxVec errors;
} Parser;

Parser new_parser(rt_string_t);
NyxVec Parser_parse_program(Parser*);
void Parser_advance(Parser*);
int Parser_check(Parser*, TokenKind);
void Parser_expect(Parser*, TokenKind);
void Parser_error(Parser*, rt_string_t);
void Parser_skip_to_semicolon(Parser*);
rt_string_t Parser_parse_ident(Parser*);
Type Parser_parse_type(Parser*);
TopLevel Parser_parse_fn_decl(Parser*, int);
TopLevel Parser_parse_struct_def(Parser*);
TopLevel Parser_parse_enum_def(Parser*);
Expr Parser_parse_block_expr(Parser*);
NyxVec Parser_parse_block(Parser*);
Stmt Parser_parse_stmt(Parser*);
Expr Parser_parse_expr(Parser*);
Expr Parser_pratt(Parser*, int64_t);
Expr Parser_parse_prefix(Parser*);
NyxVec Parser_parse_call_args(Parser*);
Expr Parser_parse_if(Parser*);
Expr Parser_parse_while(Parser*);
Expr Parser_parse_for(Parser*);
Expr Parser_parse_match(Parser*);
Pattern Parser_parse_pattern(Parser*);
Expr Parser_parse_lambda(Parser*);
// module parser
// import token::*
// import lexer::*
// import ast::*
// import std::vec
// import std::string
Parser new_parser(rt_string_t source) {
Lexer lex = new_lexer(source);
return (Parser){.lexer = lex, .current = Lexer_next_token(&(lex)), .had_error = 0, .errors = rt_vec_new(sizeof(int64_t))};
}

NyxVec Parser_parse_program(Parser* self) {
NyxVec items = rt_vec_new(sizeof(TopLevel));
while (1) {
({
int64_t __match_result_0;
TokenKind* __match_val_0 = (&(((*self).current).kind));
if ((*__match_val_0).tag == TAG_TokenKind_Fn) {
rt_vec_push(&items, ({ TopLevel _tmp_1 = Parser_parse_fn_decl(self, 0); &_tmp_1; }));
}
 else if ((*__match_val_0).tag == TAG_TokenKind_Async) {
Parser_advance(self);
rt_vec_push(&items, ({ TopLevel _tmp_2 = Parser_parse_fn_decl(self, 1); &_tmp_2; }));
}
 else if ((*__match_val_0).tag == TAG_TokenKind_Struct) {
rt_vec_push(&items, ({ TopLevel _tmp_3 = Parser_parse_struct_def(self); &_tmp_3; }));
}
 else if ((*__match_val_0).tag == TAG_TokenKind_Enum) {
rt_vec_push(&items, ({ TopLevel _tmp_4 = Parser_parse_enum_def(self); &_tmp_4; }));
}
 else if ((*__match_val_0).tag == TAG_TokenKind_Import) {
Parser_advance(self);
Parser_skip_to_semicolon(self);
}
 else if ((*__match_val_0).tag == TAG_TokenKind_Module) {
Parser_advance(self);
Parser_skip_to_semicolon(self);
}
 else if ((*__match_val_0).tag == TAG_TokenKind_Extern) {
Parser_advance(self);
Parser_skip_to_semicolon(self);
}
 else if ((*__match_val_0).tag == TAG_TokenKind_EOF) {
break;
}
 else {
Parser_error(self, rt_string_from("Unexpected token"));
Parser_advance(self);
}
__match_result_0;
});
}
;
return items;
}

void Parser_advance(Parser* self) {
(self->current = Lexer_next_token(&((*self).lexer)));
}

int Parser_check(Parser* self, TokenKind kind) {
return (((*self).current).kind.tag == kind.tag);
}

void Parser_expect(Parser* self, TokenKind kind) {
({ if ((((*self).current).kind.tag == kind.tag)) {
Parser_advance(self);
} else {
Parser_error(self, rt_string_from("Unexpected token"));
}
});
}

void Parser_error(Parser* self, rt_string_t msg) {
(self->had_error = 1);
rt_vec_push(&(*self).errors, ({ rt_string_t _tmp_6 = msg; &_tmp_6; }));
}

void Parser_skip_to_semicolon(Parser* self) {
while ((!(((*self).current).kind.tag == TAG_TokenKind_Semicolon || ((*self).current).kind.tag == TAG_TokenKind_EOF))) {
Parser_advance(self);
}
;
({ if ((((*self).current).kind.tag == TAG_TokenKind_Semicolon)) {
Parser_advance(self);
}
});
}

rt_string_t Parser_parse_ident(Parser* self) {
/* iflet */ 0;
(self->had_error = 1);
return rt_string_from("unknown");
}

Type Parser_parse_type(Parser* self) {
{ TokenKind* __iflet_8 = (&(((*self).current).kind));
if ((*__iflet_8).tag == TAG_TokenKind_Ident) {
    rt_string_t name = (*__iflet_8).data.Ident.f0;
Parser_advance(self);
return ({
Type __match_result_9;
rt_string_t __match_val_9 = name;
if (rt_string_eq(__match_val_9, rt_string_from("Int"))) {
__match_result_9 = (Type){ .tag = TAG_Type_Int };
}
 else if (rt_string_eq(__match_val_9, rt_string_from("Float"))) {
__match_result_9 = (Type){ .tag = TAG_Type_Float };
}
 else if (rt_string_eq(__match_val_9, rt_string_from("Bool"))) {
__match_result_9 = (Type){ .tag = TAG_Type_Bool };
}
 else if (rt_string_eq(__match_val_9, rt_string_from("String"))) {
__match_result_9 = (Type){ .tag = TAG_Type_String };
}
 else if (rt_string_eq(__match_val_9, rt_string_from("Char"))) {
__match_result_9 = (Type){ .tag = TAG_Type_Char };
}
 else if (rt_string_eq(__match_val_9, rt_string_from("Void"))) {
__match_result_9 = (Type){ .tag = TAG_Type_Void };
}
 else {
__match_result_9 = (Type){ .tag = TAG_Type_Custom, .data.Custom = {.f0 = name} };
}
__match_result_9;
});
} else if ((((*self).current).kind.tag == TAG_TokenKind_LBracket)) {
Parser_advance(self);
Type inner = Parser_parse_type(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBracket });
return (Type){ .tag = TAG_Type_Array, .data.Array = {.f0 = ({ Type _addr_tmp_0 = inner; &_addr_tmp_0; })} };
} else if ((((*self).current).kind.tag == TAG_TokenKind_Ampersand)) {
Parser_advance(self);
return ({ Type __if_res_11; if ((((*self).current).kind.tag == TAG_TokenKind_Mut)) {
Parser_advance(self);
__if_res_11 = (Type){ .tag = TAG_Type_MutRef, .data.MutRef = {.f0 = ({ Type _addr_tmp_0 = Parser_parse_type(self); &_addr_tmp_0; })} };
} else {
__if_res_11 = (Type){ .tag = TAG_Type_Ref, .data.Ref = {.f0 = ({ Type _addr_tmp_0 = Parser_parse_type(self); &_addr_tmp_0; })} };
}
__if_res_11; });
} else {
(self->had_error = 1);
return (Type){ .tag = TAG_Type_Void };
}
}
}

TopLevel Parser_parse_fn_decl(Parser* self, int is_async) {
Parser_advance(self);
rt_string_t name = Parser_parse_ident(self);
NyxVec generics = rt_vec_new(sizeof(int64_t));
({ if ((((*self).current).kind.tag == TAG_TokenKind_Lt)) {
Parser_advance(self);
while (1) {
rt_vec_push(&generics, ({ rt_string_t _tmp_12 = Parser_parse_ident(self); &_tmp_12; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Gt });
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LParen });
NyxVec params = rt_vec_new(sizeof(int64_t));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_RParen))) {
while (1) {
rt_string_t pname = Parser_parse_ident(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Colon });
Type ptype = Parser_parse_type(self);
rt_vec_push(&params, ({ Param _tmp_15 = (Param){.name = pname, .ty = ptype}; &_tmp_15; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RParen });
Type ret_type = ({ Type __if_res_18; if ((((*self).current).kind.tag == TAG_TokenKind_Arrow)) {
Parser_advance(self);
__if_res_18 = Parser_parse_type(self);
} else {
__if_res_18 = (Type){ .tag = TAG_Type_Void };
}
__if_res_18; });
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
Expr body = Parser_parse_block_expr(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
return (TopLevel){ .tag = TAG_TopLevel_FnDecl, .data.FnDecl = { .f0 = name, .f1 = generics, .f2 = params, .f3 = ret_type, .f4 = body, .f5 = is_async } };
}

TopLevel Parser_parse_struct_def(Parser* self) {
Parser_advance(self);
rt_string_t name = Parser_parse_ident(self);
NyxVec generics = rt_vec_new(sizeof(int64_t));
({ if ((((*self).current).kind.tag == TAG_TokenKind_Lt)) {
Parser_advance(self);
while (1) {
rt_vec_push(&generics, ({ rt_string_t _tmp_19 = Parser_parse_ident(self); &_tmp_19; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Gt });
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec fields = rt_vec_new(sizeof(int64_t));
while ((!(((*self).current).kind.tag == TAG_TokenKind_RBrace))) {
rt_string_t fname = Parser_parse_ident(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Colon });
Type ftype = Parser_parse_type(self);
rt_vec_push(&fields, ({ NyxTuple2 _tmp_21 = ({ NyxTuple2 _tuple_22 = {.f0 = (void*)({ rt_string_t _stmp_0 = fname; _stmp_0.data; }), .f1 = (void*)({ Type _stmp_1 = ftype; &_stmp_1; }) }; _tuple_22; }); &_tmp_21; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
return (TopLevel){ .tag = TAG_TopLevel_StructDef, .data.StructDef = { .f0 = name, .f1 = generics, .f2 = fields } };
}

TopLevel Parser_parse_enum_def(Parser* self) {
Parser_advance(self);
rt_string_t name = Parser_parse_ident(self);
NyxVec generics = rt_vec_new(sizeof(int64_t));
({ if ((((*self).current).kind.tag == TAG_TokenKind_Lt)) {
Parser_advance(self);
while (1) {
rt_vec_push(&generics, ({ rt_string_t _tmp_25 = Parser_parse_ident(self); &_tmp_25; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Gt });
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec variants = rt_vec_new(sizeof(int64_t));
while ((!(((*self).current).kind.tag == TAG_TokenKind_RBrace))) {
rt_string_t vname = Parser_parse_ident(self);
NyxVec args = rt_vec_new(sizeof(int64_t));
({ if ((((*self).current).kind.tag == TAG_TokenKind_LParen)) {
Parser_advance(self);
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_RParen))) {
while (1) {
rt_vec_push(&args, ({ Type _tmp_29 = Parser_parse_type(self); &_tmp_29; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RParen });
}
});
rt_vec_push(&variants, ({ NyxTuple2 _tmp_31 = ({ NyxTuple2 _tuple_32 = {.f0 = (void*)({ rt_string_t _stmp_0 = vname; _stmp_0.data; }), .f1 = (void*)({ NyxVec _stmp_1 = args; &_stmp_1; }) }; _tuple_32; }); &_tmp_31; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
return (TopLevel){ .tag = TAG_TopLevel_EnumDef, .data.EnumDef = { .f0 = name, .f1 = generics, .f2 = variants } };
}

Expr Parser_parse_block_expr(Parser* self) {
NyxVec stmts = rt_vec_new(sizeof(int64_t));
while ((!(((*self).current).kind.tag == TAG_TokenKind_RBrace || ((*self).current).kind.tag == TAG_TokenKind_EOF))) {
rt_vec_push(&stmts, ({ Stmt _tmp_34 = Parser_parse_stmt(self); &_tmp_34; }));
}
;
return (Expr){ .tag = TAG_Expr_Block, .data.Block = {.f0 = stmts} };
}

NyxVec Parser_parse_block(Parser* self) {
NyxVec stmts = rt_vec_new(sizeof(Stmt));
while ((!(((*self).current).kind.tag == TAG_TokenKind_RBrace || ((*self).current).kind.tag == TAG_TokenKind_EOF))) {
rt_vec_push(&stmts, ({ Stmt _tmp_35 = Parser_parse_stmt(self); &_tmp_35; }));
}
;
return stmts;
}

Stmt Parser_parse_stmt(Parser* self) {
return ({
Stmt __match_result_36;
TokenKind* __match_val_36 = (&(((*self).current).kind));
if ((*__match_val_36).tag == TAG_TokenKind_Let) {
Parser_advance(self);
int64_t mutable = (((*self).current).kind.tag == TAG_TokenKind_Mut);
({ if (mutable) {
Parser_advance(self);
}
});
rt_string_t name = Parser_parse_ident(self);
NyxOption ty = rt_option_none();
({ if ((((*self).current).kind.tag == TAG_TokenKind_Colon)) {
Parser_advance(self);
ty = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ Type _stmp_ = Parser_parse_type(self); &_stmp_; }); _tmp; });
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Equals });
Expr value = Parser_parse_expr(self);
({ if ((((*self).current).kind.tag == TAG_TokenKind_Semicolon)) {
Parser_advance(self);
}
});
__match_result_36 = (Stmt){ .tag = TAG_Stmt_Let, .data.Let = { .f0 = name, .f1 = mutable, .f2 = ty, .f3 = value } };
}
 else if ((*__match_val_36).tag == TAG_TokenKind_Return) {
Parser_advance(self);
NyxOption expr = ({ NyxOption __if_res_41; if ((!(((*self).current).kind.tag == TAG_TokenKind_Semicolon || ((*self).current).kind.tag == TAG_TokenKind_RBrace))) {
__if_res_41 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ Expr _stmp_ = Parser_parse_expr(self); &_stmp_; }); _tmp; });
} else {
__if_res_41 = rt_option_none();
}
__if_res_41; });
({ if ((((*self).current).kind.tag == TAG_TokenKind_Semicolon)) {
Parser_advance(self);
}
});
__match_result_36 = (Stmt){ .tag = TAG_Stmt_Return, .data.Return = {.f0 = expr} };
}
 else {
Expr expr = Parser_parse_expr(self);
({ if ((((*self).current).kind.tag == TAG_TokenKind_Semicolon)) {
Parser_advance(self);
}
});
__match_result_36 = (Stmt){ .tag = TAG_Stmt_Expr, .data.Expr = {.f0 = expr} };
}
__match_result_36;
});
}

Expr Parser_parse_expr(Parser* self) {
return Parser_pratt(self, 0LL);
}

Expr Parser_pratt(Parser* self, int64_t min_bp) {
Expr lhs = Parser_parse_prefix(self);
while (1) {
NyxTuple3 _tmp_bin_op_l_bp_r_bp = ({
NyxTuple3 __match_result_43;
TokenKind* __match_val_43 = (&(((*self).current).kind));
if ((*__match_val_43).tag == TAG_TokenKind_Plus) {
__match_result_43 = ({ NyxTuple3 _tuple_44 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Add }; &_stmp_0; }), .f1 = (void*)(7LL), .f2 = (void*)(8LL) }; _tuple_44; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_Minus) {
__match_result_43 = ({ NyxTuple3 _tuple_45 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Sub }; &_stmp_0; }), .f1 = (void*)(7LL), .f2 = (void*)(8LL) }; _tuple_45; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_Star) {
__match_result_43 = ({ NyxTuple3 _tuple_46 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Mul }; &_stmp_0; }), .f1 = (void*)(9LL), .f2 = (void*)(10LL) }; _tuple_46; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_Slash) {
__match_result_43 = ({ NyxTuple3 _tuple_47 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Div }; &_stmp_0; }), .f1 = (void*)(9LL), .f2 = (void*)(10LL) }; _tuple_47; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_Percent) {
__match_result_43 = ({ NyxTuple3 _tuple_48 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Mod }; &_stmp_0; }), .f1 = (void*)(9LL), .f2 = (void*)(10LL) }; _tuple_48; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_EqEq) {
__match_result_43 = ({ NyxTuple3 _tuple_49 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Eq }; &_stmp_0; }), .f1 = (void*)(5LL), .f2 = (void*)(6LL) }; _tuple_49; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_NotEq) {
__match_result_43 = ({ NyxTuple3 _tuple_50 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_NotEq }; &_stmp_0; }), .f1 = (void*)(5LL), .f2 = (void*)(6LL) }; _tuple_50; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_Lt) {
__match_result_43 = ({ NyxTuple3 _tuple_51 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Lt }; &_stmp_0; }), .f1 = (void*)(5LL), .f2 = (void*)(6LL) }; _tuple_51; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_LtEq) {
__match_result_43 = ({ NyxTuple3 _tuple_52 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_LtEq }; &_stmp_0; }), .f1 = (void*)(5LL), .f2 = (void*)(6LL) }; _tuple_52; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_Gt) {
__match_result_43 = ({ NyxTuple3 _tuple_53 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Gt }; &_stmp_0; }), .f1 = (void*)(5LL), .f2 = (void*)(6LL) }; _tuple_53; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_GtEq) {
__match_result_43 = ({ NyxTuple3 _tuple_54 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_GtEq }; &_stmp_0; }), .f1 = (void*)(5LL), .f2 = (void*)(6LL) }; _tuple_54; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_And) {
__match_result_43 = ({ NyxTuple3 _tuple_55 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_And }; &_stmp_0; }), .f1 = (void*)(3LL), .f2 = (void*)(4LL) }; _tuple_55; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_Or) {
__match_result_43 = ({ NyxTuple3 _tuple_56 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Or }; &_stmp_0; }), .f1 = (void*)(1LL), .f2 = (void*)(2LL) }; _tuple_56; });
}
 else {
break;
}
__match_result_43;
});
int64_t bin_op = (int64_t)_tmp_bin_op_l_bp_r_bp.f0; // tuple destructuring
int64_t l_bp = (int64_t)_tmp_bin_op_l_bp_r_bp.f1; // tuple destructuring
int64_t r_bp = (int64_t)_tmp_bin_op_l_bp_r_bp.f2; // tuple destructuring
({ if ((l_bp < min_bp)) {
break;
}
});
Parser_advance(self);
Expr rhs = Parser_pratt(self, r_bp);
lhs = (Expr){ .tag = TAG_Expr_Binary, .data.Binary = { .f0 = bin_op, .f1 = ({ Expr _stmp_1 = lhs; &_stmp_1; }), .f2 = ({ Expr _stmp_2 = rhs; &_stmp_2; }) } };
while ((((*self).current).kind.tag == TAG_TokenKind_Dot)) {
Parser_advance(self);
rt_string_t field = Parser_parse_ident(self);
({ Expr __if_res_59; if ((((*self).current).kind.tag == TAG_TokenKind_LParen)) {
NyxVec args = Parser_parse_call_args(self);
__if_res_59 = (lhs = (Expr){ .tag = TAG_Expr_MethodCall, .data.MethodCall = { .f0 = ({ Expr _stmp_0 = lhs; &_stmp_0; }), .f1 = field, .f2 = rt_vec_new(sizeof(int64_t)), .f3 = args } });
} else {
__if_res_59 = (lhs = (Expr){ .tag = TAG_Expr_FieldAccess, .data.FieldAccess = { .f0 = ({ Expr _stmp_0 = lhs; &_stmp_0; }), .f1 = field } });
}
__if_res_59; });
}
;
}
;
return lhs;
}

Expr Parser_parse_prefix(Parser* self) {
return ({
Expr __match_result_59;
TokenKind* __match_val_59 = ({ TokenKind _utmp_ = ((*self).current).kind; &_utmp_; });
if ((*__match_val_59).tag == TAG_TokenKind_IntLit) {
    int64_t n = (*__match_val_59).data.IntLit.f0;
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Int, .data.Int = {.f0 = n} }} };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_FloatLit) {
    double f = (*__match_val_59).data.FloatLit.f0;
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Float, .data.Float = {.f0 = f} }} };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_StringLit) {
    rt_string_t s = (*__match_val_59).data.StringLit.f0;
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_String, .data.String = {.f0 = ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })} }} };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_True) {
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Bool, .data.Bool = {.f0 = 1} }} };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_False) {
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Bool, .data.Bool = {.f0 = 0} }} };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Null) {
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Int, .data.Int = {.f0 = 0LL} }} };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Ident) {
    rt_string_t name = (*__match_val_59).data.Ident.f0;
Parser_advance(self);
if ((((*self).current).kind.tag == TAG_TokenKind_LParen)) {
NyxVec args = Parser_parse_call_args(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Call, .data.Call = { .f0 = ({ Expr _stmp_0 = (Expr){ .tag = TAG_Expr_Ident, .data.Ident = {.f0 = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })} }; &_stmp_0; }), .f1 = rt_vec_new(sizeof(int64_t)), .f2 = args } };
} else if ((((*self).current).kind.tag == TAG_TokenKind_LBrace)) {
Parser_advance(self);
NyxVec fields = rt_vec_new(sizeof(int64_t));
while ((!(((*self).current).kind.tag == TAG_TokenKind_RBrace))) {
rt_string_t fname = Parser_parse_ident(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Colon });
Expr val = Parser_parse_expr(self);
rt_vec_push(&fields, ({ NyxTuple2 _tmp_60 = ({ NyxTuple2 _tuple_61 = {.f0 = (void*)({ rt_string_t _stmp_0 = fname; _stmp_0.data; }), .f1 = (void*)({ Expr _stmp_1 = val; &_stmp_1; }) }; _tuple_61; }); &_tmp_60; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
__match_result_59 = (Expr){ .tag = TAG_Expr_StructLiteral, .data.StructLiteral = { .f0 = name, .f1 = rt_vec_new(sizeof(int64_t)), .f2 = fields } };
} else {
__match_result_59 = (Expr){ .tag = TAG_Expr_Ident, .data.Ident = {.f0 = name} };
}
}
 else if ((*__match_val_59).tag == TAG_TokenKind_LParen) {
Parser_advance(self);
Expr expr = Parser_parse_expr(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RParen });
__match_result_59 = expr;
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Not) {
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Unary, .data.Unary = { .f0 = (UnaryOp){ .tag = TAG_UnaryOp_Not }, .f1 = ({ Expr _stmp_1 = Parser_parse_prefix(self); &_stmp_1; }) } };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Minus) {
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Unary, .data.Unary = { .f0 = (UnaryOp){ .tag = TAG_UnaryOp_Neg }, .f1 = ({ Expr _stmp_1 = Parser_parse_prefix(self); &_stmp_1; }) } };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_If) {
__match_result_59 = Parser_parse_if(self);
}
 else if ((*__match_val_59).tag == TAG_TokenKind_While) {
__match_result_59 = Parser_parse_while(self);
}
 else if ((*__match_val_59).tag == TAG_TokenKind_For) {
__match_result_59 = Parser_parse_for(self);
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Match) {
__match_result_59 = Parser_parse_match(self);
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Fn) {
__match_result_59 = Parser_parse_lambda(self);
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Pipe) {
__match_result_59 = Parser_parse_lambda(self);
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Return) {
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Return, .data.Return = {.f0 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ Expr _stmp_ = Parser_parse_expr(self); &_stmp_; }); _tmp; })} };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Await) {
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Await, .data.Await = {.f0 = ({ Expr _addr_tmp_0 = Parser_parse_expr(self); &_addr_tmp_0; })} };
}
 else {
Parser_error(self, rt_string_from("Unexpected token in expression"));
__match_result_59 = (Expr){ .tag = TAG_Expr_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Int, .data.Int = {.f0 = 0LL} }} };
}
__match_result_59;
});
}

NyxVec Parser_parse_call_args(Parser* self) {
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LParen });
NyxVec args = rt_vec_new(sizeof(Expr));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_RParen))) {
while (1) {
rt_vec_push(&args, ({ Expr _tmp_64 = Parser_parse_expr(self); &_tmp_64; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RParen });
return args;
}

Expr Parser_parse_if(Parser* self) {
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_If });
Expr cond = Parser_parse_expr(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec then = Parser_parse_block(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
NyxVec else_ifs = rt_vec_new(sizeof(int64_t));
NyxOption else_block = rt_option_none();
while ((((*self).current).kind.tag == TAG_TokenKind_Else)) {
Parser_advance(self);
({ Expr __if_res_67; if ((((*self).current).kind.tag == TAG_TokenKind_If)) {
Parser_advance(self);
Expr elif_cond = Parser_parse_expr(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec elif_body = Parser_parse_block(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
rt_vec_push(&else_ifs, ({ NyxTuple2 _tmp_67 = ({ NyxTuple2 _tuple_68 = {.f0 = (void*)({ Expr _stmp_0 = elif_cond; &_stmp_0; }), .f1 = (void*)({ NyxVec _stmp_1 = elif_body; &_stmp_1; }) }; _tuple_68; }); &_tmp_67; }));
} else {
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec body = Parser_parse_block(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
else_block = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = body; &_stmp_; }); _tmp; });
break;
}
__if_res_67; });
}
;
return (Expr){ .tag = TAG_Expr_If, .data.If = { .f0 = ({ Expr _stmp_0 = cond; &_stmp_0; }), .f1 = then, .f2 = else_ifs, .f3 = else_block } };
}

Expr Parser_parse_while(Parser* self) {
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_While });
Expr cond = Parser_parse_expr(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec body = Parser_parse_block(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
return (Expr){ .tag = TAG_Expr_While, .data.While = { .f0 = ({ Expr _stmp_0 = cond; &_stmp_0; }), .f1 = body } };
}

Expr Parser_parse_for(Parser* self) {
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_For });
rt_string_t var = Parser_parse_ident(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_In });
Expr iter = Parser_parse_expr(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec body = Parser_parse_block(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
return (Expr){ .tag = TAG_Expr_For, .data.For = { .f0 = var, .f1 = ({ Expr _stmp_1 = iter; &_stmp_1; }), .f2 = body } };
}

Expr Parser_parse_match(Parser* self) {
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Match });
Expr expr = Parser_parse_expr(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec arms = rt_vec_new(sizeof(int64_t));
while ((!(((*self).current).kind.tag == TAG_TokenKind_RBrace))) {
Pattern pattern = Parser_parse_pattern(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_FatArrow });
Expr body = Parser_parse_expr(self);
rt_vec_push(&arms, ({ MatchArm _tmp_69 = (MatchArm){.pattern = pattern, .guard = rt_option_none(), .body = body}; &_tmp_69; }));
({ if ((((*self).current).kind.tag == TAG_TokenKind_Comma)) {
Parser_advance(self);
}
});
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
return (Expr){ .tag = TAG_Expr_Match, .data.Match = { .f0 = ({ Expr _stmp_0 = expr; &_stmp_0; }), .f1 = arms } };
}

Pattern Parser_parse_pattern(Parser* self) {
return ({
Pattern __match_result_71;
TokenKind* __match_val_71 = ({ TokenKind _utmp_ = ((*self).current).kind; &_utmp_; });
if ((*__match_val_71).tag == TAG_TokenKind_Ident) {
    rt_string_t name = (*__match_val_71).data.Ident.f0;
Parser_advance(self);
if ((((*self).current).kind.tag == TAG_TokenKind_LParen)) {
Parser_advance(self);
NyxVec args = rt_vec_new(sizeof(int64_t));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_RParen))) {
while (1) {
rt_vec_push(&args, ({ Pattern _tmp_73 = Parser_parse_pattern(self); &_tmp_73; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RParen });
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Enum, .data.Enum = { .f0 = rt_string_from(""), .f1 = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), .f2 = args } };
} else {
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Ident, .data.Ident = {.f0 = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })} };
}
}
 else if ((*__match_val_71).tag == TAG_TokenKind_Underscore) {
Parser_advance(self);
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Wildcard };
}
 else if ((*__match_val_71).tag == TAG_TokenKind_IntLit) {
    int64_t n = (*__match_val_71).data.IntLit.f0;
Parser_advance(self);
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Int, .data.Int = {.f0 = n} }} };
}
 else if ((*__match_val_71).tag == TAG_TokenKind_StringLit) {
    rt_string_t s = (*__match_val_71).data.StringLit.f0;
Parser_advance(self);
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_String, .data.String = {.f0 = ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })} }} };
}
 else if ((*__match_val_71).tag == TAG_TokenKind_True) {
Parser_advance(self);
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Bool, .data.Bool = {.f0 = 1} }} };
}
 else if ((*__match_val_71).tag == TAG_TokenKind_False) {
Parser_advance(self);
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Bool, .data.Bool = {.f0 = 0} }} };
}
 else {
Parser_error(self, rt_string_from("Expected pattern"));
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Wildcard };
}
__match_result_71;
});
}

Expr Parser_parse_lambda(Parser* self) {
if ((((*self).current).kind.tag == TAG_TokenKind_Pipe)) {
Parser_advance(self);
NyxVec params = rt_vec_new(sizeof(int64_t));
while ((!(((*self).current).kind.tag == TAG_TokenKind_Pipe))) {
rt_string_t name = Parser_parse_ident(self);
Type ty = ({ Type __if_res_76; if ((((*self).current).kind.tag == TAG_TokenKind_Colon)) {
Parser_advance(self);
__if_res_76 = Parser_parse_type(self);
} else {
__if_res_76 = (Type){ .tag = TAG_Type_Unknown };
}
__if_res_76; });
rt_vec_push(&params, ({ Param _tmp_76 = (Param){.name = name, .ty = ty}; &_tmp_76; }));
({ if ((((*self).current).kind.tag == TAG_TokenKind_Comma)) {
Parser_advance(self);
}
});
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Pipe });
Expr body = Parser_parse_expr(self);
return (Expr){ .tag = TAG_Expr_Lambda, .data.Lambda = { .f0 = params, .f1 = ({ Expr _stmp_1 = body; &_stmp_1; }) } };
} else {
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Fn });
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LParen });
NyxVec params = rt_vec_new(sizeof(int64_t));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_RParen))) {
while (1) {
rt_string_t pname = Parser_parse_ident(self);
Type ty = ({ Type __if_res_80; if ((((*self).current).kind.tag == TAG_TokenKind_Colon)) {
Parser_advance(self);
__if_res_80 = Parser_parse_type(self);
} else {
__if_res_80 = (Type){ .tag = TAG_Type_Unknown };
}
__if_res_80; });
rt_vec_push(&params, ({ Param _tmp_80 = (Param){.name = pname, .ty = ty}; &_tmp_80; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RParen });
Expr body = Parser_parse_expr(self);
return (Expr){ .tag = TAG_Expr_Lambda, .data.Lambda = { .f0 = params, .f1 = ({ Expr _stmp_1 = body; &_stmp_1; }) } };
}
}

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

typedef struct {
    NyxVec env;
    NyxVec errors;
} TypeChecker;

TypeChecker TypeChecker_new();
void TypeChecker_check_program(TypeChecker*, NyxVec*);
int TypeChecker_has_errors(TypeChecker*);
NyxVec* TypeChecker_get_errors(TypeChecker*);
void TypeChecker_push_scope(TypeChecker*);
void TypeChecker_pop_scope(TypeChecker*);
void TypeChecker_add_var(TypeChecker*, rt_string_t, Type);
NyxOption TypeChecker_lookup(TypeChecker*, rt_string_t);
void TypeChecker_check_top_level(TypeChecker*, TopLevel*);
NyxResult TypeChecker_check_expr(TypeChecker*, Expr*);
NyxResult TypeChecker_check_stmt(TypeChecker*, Stmt*);
// module typecheck
// import ast::*
// import std::vec
TypeChecker TypeChecker_new() {
return (TypeChecker){.env = rt_vec_new(sizeof(int64_t)), .errors = rt_vec_new(sizeof(int64_t))};
}

void TypeChecker_check_program(TypeChecker* self, NyxVec* program) {
TypeChecker_push_scope(self);
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
TypeChecker_check_top_level(self, &(item));
}
;
TypeChecker_pop_scope(self);
}

int TypeChecker_has_errors(TypeChecker* self) {
return ((*self).errors.len > 0LL);
}

NyxVec* TypeChecker_get_errors(TypeChecker* self) {
return (&((*self).errors));
}

void TypeChecker_push_scope(TypeChecker* self) {
rt_vec_push(&(*self).env, ({ NyxMap _tmp_0 = rt_map_new(); &_tmp_0; }));
}

void TypeChecker_pop_scope(TypeChecker* self) {
rt_vec_pop(&((*self).env));
}

void TypeChecker_add_var(TypeChecker* self, rt_string_t name, Type ty) {
({ if (((*self).env.len > 0LL)) {
int64_t last_idx = ((*self).env.len - 1LL);
NyxMap scope = (*(NyxMap*)rt_vec_get((*self).env, last_idx));
rt_map_insert(&(scope), name, (int64_t)(({ Type* _mtmp = malloc(sizeof(Type)); *_mtmp = ty; (int64_t)_mtmp; })));
}
});
}

NyxOption TypeChecker_lookup(TypeChecker* self, rt_string_t name) {
int64_t i = (*self).env.len;
while ((i > 0LL)) {
i = (i - 1LL);
NyxMap scope = (*(NyxMap*)rt_vec_get((*self).env, i));
({
int64_t __match_result_2;
NyxOption __match_val_2 = rt_map_get(&(scope), name);
if (__match_val_2.tag == OPTION_SOME) {
    Type t = *(Type*)__match_val_2.data.ptr_val;
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ Type _stmp_ = t; &_stmp_; }); _tmp; });
}
 else {
}
__match_result_2;
});
}
;
return rt_option_none();
}

void TypeChecker_check_top_level(TypeChecker* self, TopLevel* item) {
({
int64_t __match_result_3;
TopLevel* __match_val_3 = item;
if ((*__match_val_3).tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = (*__match_val_3).data.FnDecl.f0;
    NyxVec generics = (*__match_val_3).data.FnDecl.f1;
    NyxVec params = (*__match_val_3).data.FnDecl.f2;
    Type ret_type = (*__match_val_3).data.FnDecl.f3;
    Expr body = (*__match_val_3).data.FnDecl.f4;
({ if ((generics.len > 0LL)) {
return;
}
});
TypeChecker_push_scope(self);
for (int64_t _p = 0; _p < params.len; _p++) {
    Param p = *(Param*)rt_vec_get(params, _p);
TypeChecker_add_var(self, ({ rt_string_t _c = p.name; rt_string_retain(&_c); _c; }), p.ty);
}
;
({
int64_t __match_result_5;
NyxResult __match_val_5 = TypeChecker_check_expr(self, &(body));
if (__match_val_5.tag == RESULT_OK) {
}
 else if (__match_val_5.tag == RESULT_ERR) {
    rt_string_t e = *(rt_string_t*)__match_val_5.data.ptr_val;
rt_vec_push(&(*self).errors, ({ rt_string_t _tmp_6 = e; &_tmp_6; }));
}
__match_result_5;
});
TypeChecker_pop_scope(self);
}
 else if ((*__match_val_3).tag == TAG_TopLevel_StructDef) {
    rt_string_t name = (*__match_val_3).data.StructDef.f0;
    NyxVec fields = (*__match_val_3).data.StructDef.f2;
NyxVec field_types = rt_vec_new(sizeof(int64_t));
for (int64_t _idx___ftype = 0; _idx___ftype < fields.len; _idx___ftype++) {
    NyxTuple2 _tup___ftype = *(NyxTuple2*)rt_vec_get(fields, _idx___ftype);
    int64_t _ = (int64_t)(_tup___ftype.f0);
    int64_t ftype = (int64_t)(_tup___ftype.f1);
rt_vec_push(&field_types, ({ int64_t _tmp_7 = ftype; &_tmp_7; }));
}
;
}
 else if ((*__match_val_3).tag == TAG_TopLevel_EnumDef) {
}
 else if ((*__match_val_3).tag == TAG_TopLevel_ImplBlock) {
}
 else if ((*__match_val_3).tag == TAG_TopLevel_TraitDef) {
}
 else {
}
__match_result_3;
});
}

NyxResult TypeChecker_check_expr(TypeChecker* self, Expr* expr) {
return ({
NyxResult __match_result_8;
Expr* __match_val_8 = expr;
if ((*__match_val_8).tag == TAG_Expr_Lit) {
    Literal lit = (*__match_val_8).data.Lit.f0;
__match_result_8 = ({
NyxResult __match_result_9;
Literal __match_val_9 = lit;
if (__match_val_9.tag == TAG_Literal_Int) {
__match_result_9 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Int }; &_stmp_; }); _tmp; });
}
 else if (__match_val_9.tag == TAG_Literal_Float) {
__match_result_9 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Float }; &_stmp_; }); _tmp; });
}
 else if (__match_val_9.tag == TAG_Literal_Bool) {
__match_result_9 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Bool }; &_stmp_; }); _tmp; });
}
 else if (__match_val_9.tag == TAG_Literal_String) {
__match_result_9 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_String }; &_stmp_; }); _tmp; });
}
 else if (__match_val_9.tag == TAG_Literal_Char) {
__match_result_9 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Char }; &_stmp_; }); _tmp; });
}
__match_result_9;
});
}
 else if ((*__match_val_8).tag == TAG_Expr_Ident) {
    rt_string_t name = (*__match_val_8).data.Ident.f0;
__match_result_8 = rt_option_ok_or_else(TypeChecker_lookup(self, name), ({ rt_string_t __lambda_10() { return rt_string_concat(rt_string_from("Undefined variable: "), name); } __lambda_10; }));
}
 else if ((*__match_val_8).tag == TAG_Expr_Binary) {
    BinOp op = (*__match_val_8).data.Binary.f0;
    Expr* left = (*__match_val_8).data.Binary.f1;
    Expr* right = (*__match_val_8).data.Binary.f2;
Type lt = ({ NyxResult _qres = TypeChecker_check_expr(self, left); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
Type rt = ({ NyxResult _qres = TypeChecker_check_expr(self, right); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
__match_result_8 = ({
NyxResult __match_result_11;
BinOp __match_val_11 = op;
if (0 || 0 || 0 || 0 || 0) {
if (((lt.tag == (Type){ .tag = TAG_Type_Int }.tag) && (rt.tag == (Type){ .tag = TAG_Type_Int }.tag))) {
__match_result_11 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Int }; &_stmp_; }); _tmp; });
} else if (((lt.tag == (Type){ .tag = TAG_Type_Float }.tag) && (rt.tag == (Type){ .tag = TAG_Type_Float }.tag))) {
__match_result_11 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Float }; &_stmp_; }); _tmp; });
} else if ((((lt.tag == (Type){ .tag = TAG_Type_String }.tag) && (rt.tag == (Type){ .tag = TAG_Type_String }.tag)) && (op.tag == TAG_BinOp_Add))) {
__match_result_11 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_String }; &_stmp_; }); _tmp; });
} else {
__match_result_11 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Incompatible types for arithmetic"); _stmp_.data; }); _tmp; });
}
}
 else if (0 || 0 || 0 || 0 || 0 || 0) {
__match_result_11 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Bool }; &_stmp_; }); _tmp; });
}
 else if (0 || 0) {
__match_result_11 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Bool }; &_stmp_; }); _tmp; });
}
__match_result_11;
});
}
 else if ((*__match_val_8).tag == TAG_Expr_Unary) {
    UnaryOp op = (*__match_val_8).data.Unary.f0;
    Expr* expr = (*__match_val_8).data.Unary.f1;
Type t = ({ NyxResult _qres = TypeChecker_check_expr(self, expr); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
__match_result_8 = ({
NyxResult __match_result_12;
UnaryOp __match_val_12 = op;
if (__match_val_12.tag == TAG_UnaryOp_Neg) {
__match_result_12 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = t; &_stmp_; }); _tmp; });
}
 else if (__match_val_12.tag == TAG_UnaryOp_Not) {
__match_result_12 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Bool }; &_stmp_; }); _tmp; });
}
 else if (__match_val_12.tag == TAG_UnaryOp_Ref) {
__match_result_12 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Ref, .data.Ref = {.f0 = ({ Type _addr_tmp_0 = t; &_addr_tmp_0; })} }; &_stmp_; }); _tmp; });
}
 else if (__match_val_12.tag == TAG_UnaryOp_Deref) {
__match_result_12 = ({
NyxResult __match_result_13;
Type __match_val_13 = t;
if (__match_val_13.tag == TAG_Type_Ref) {
    Type* inner = __match_val_13.data.Ref.f0;
__match_result_13 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)(inner); _tmp; });
}
 else {
__match_result_13 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Deref requires reference type"); _stmp_.data; }); _tmp; });
}
__match_result_13;
});
}
__match_result_12;
});
}
 else if ((*__match_val_8).tag == TAG_Expr_Call) {
    Expr* func = (*__match_val_8).data.Call.f0;
    NyxVec args = (*__match_val_8).data.Call.f2;
({ NyxResult _qres = TypeChecker_check_expr(self, func); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
for (int64_t _arg = 0; _arg < args.len; _arg++) {
    Expr arg = *(Expr*)rt_vec_get(args, _arg);
({ NyxResult _qres = TypeChecker_check_expr(self, &(arg)); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
}
;
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_MethodCall) {
    Expr* receiver = (*__match_val_8).data.MethodCall.f0;
    NyxVec args = (*__match_val_8).data.MethodCall.f3;
({ NyxResult _qres = TypeChecker_check_expr(self, receiver); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
for (int64_t _arg = 0; _arg < args.len; _arg++) {
    Expr arg = *(Expr*)rt_vec_get(args, _arg);
({ NyxResult _qres = TypeChecker_check_expr(self, &(arg)); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
}
;
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_If) {
    Expr* cond = (*__match_val_8).data.If.f0;
    NyxVec then = (*__match_val_8).data.If.f1;
    NyxVec else_ifs = (*__match_val_8).data.If.f2;
    NyxOption else_block = (*__match_val_8).data.If.f3;
Type ct = ({ NyxResult _qres = TypeChecker_check_expr(self, cond); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
({ if ((ct.tag != (Type){ .tag = TAG_Type_Bool }.tag)) {
rt_vec_push(&(*self).errors, ({ rt_string_t _tmp_15 = rt_string_from("If condition must be bool"); &_tmp_15; }));
}
});
Type result = (Type){ .tag = TAG_Type_Void };
({
Type __match_result_16;
NyxResult __match_val_16 = TypeChecker_check_expr(self, ({ Expr _utmp_ = (Expr){ .tag = TAG_Expr_Block, .data.Block = {.f0 = rt_vec_clone(then)} }; &_utmp_; }));
if (__match_val_16.tag == RESULT_OK) {
    Type t = *(Type*)__match_val_16.data.ptr_val;
__match_result_16 = (result = t);
}
 else if (__match_val_16.tag == RESULT_ERR) {
    Type e = *(Type*)__match_val_16.data.ptr_val;
rt_vec_push(&(*self).errors, ({ Type _tmp_17 = e; &_tmp_17; }));
}
__match_result_16;
});
for (int64_t _idx_elif_cond_elif_body = 0; _idx_elif_cond_elif_body < else_ifs.len; _idx_elif_cond_elif_body++) {
    NyxTuple2 _tup_elif_cond_elif_body = *(NyxTuple2*)rt_vec_get(else_ifs, _idx_elif_cond_elif_body);
    Expr* elif_cond = (Expr*)(_tup_elif_cond_elif_body.f0);
    NyxVec elif_body = *(NyxVec*)(_tup_elif_cond_elif_body.f1);
({ NyxResult _qres = TypeChecker_check_expr(self, elif_cond); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
({ NyxResult _qres = TypeChecker_check_expr(self, ({ Expr _utmp_ = (Expr){ .tag = TAG_Expr_Block, .data.Block = {.f0 = rt_vec_clone(elif_body)} }; &_utmp_; })); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
}
;
/* iflet */ 0;
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = result; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_While) {
    Expr* cond = (*__match_val_8).data.While.f0;
    NyxVec body = (*__match_val_8).data.While.f1;
({ NyxResult _qres = TypeChecker_check_expr(self, cond); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
({ NyxResult _qres = TypeChecker_check_expr(self, ({ Expr _utmp_ = (Expr){ .tag = TAG_Expr_Block, .data.Block = {.f0 = rt_vec_clone(body)} }; &_utmp_; })); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_For) {
    rt_string_t var = (*__match_val_8).data.For.f0;
    Expr* iter = (*__match_val_8).data.For.f1;
    NyxVec body = (*__match_val_8).data.For.f2;
({ NyxResult _qres = TypeChecker_check_expr(self, iter); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
TypeChecker_push_scope(self);
TypeChecker_add_var(self, ({ rt_string_t _c = var; rt_string_retain(&_c); _c; }), (Type){ .tag = TAG_Type_Int });
({ NyxResult _qres = TypeChecker_check_expr(self, ({ Expr _utmp_ = (Expr){ .tag = TAG_Expr_Block, .data.Block = {.f0 = rt_vec_clone(body)} }; &_utmp_; })); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
TypeChecker_pop_scope(self);
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_Match) {
    Expr* expr = (*__match_val_8).data.Match.f0;
    NyxVec arms = (*__match_val_8).data.Match.f1;
({ NyxResult _qres = TypeChecker_check_expr(self, expr); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
Type result = (Type){ .tag = TAG_Type_Void };
for (int64_t _arm = 0; _arm < arms.len; _arm++) {
    MatchArm arm = *(MatchArm*)rt_vec_get(arms, _arm);
TypeChecker_push_scope(self);
({
Type __match_result_18;
NyxResult __match_val_18 = TypeChecker_check_expr(self, (&(arm.body)));
if (__match_val_18.tag == RESULT_OK) {
    Type t = *(Type*)__match_val_18.data.ptr_val;
__match_result_18 = (result = t);
}
 else if (__match_val_18.tag == RESULT_ERR) {
    Type e = *(Type*)__match_val_18.data.ptr_val;
rt_vec_push(&(*self).errors, ({ Type _tmp_19 = e; &_tmp_19; }));
}
__match_result_18;
});
TypeChecker_pop_scope(self);
}
;
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = result; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_Block) {
    NyxVec stmts = (*__match_val_8).data.Block.f0;
Type last = (Type){ .tag = TAG_Type_Void };
for (int64_t _s = 0; _s < stmts.len; _s++) {
    Stmt s = *(Stmt*)rt_vec_get(stmts, _s);
last = ({ NyxResult _qres = TypeChecker_check_stmt(self, &(s)); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
}
;
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = last; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_Let) {
    rt_string_t name = (*__match_val_8).data.Let.f0;
    Expr* value = (*__match_val_8).data.Let.f1;
    NyxOption ty = (*__match_val_8).data.Let.f2;
Type vt = ({ NyxResult _qres = TypeChecker_check_expr(self, value); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
/* iflet */ 0;
TypeChecker_add_var(self, ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), vt);
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = vt; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_Assign) {
    rt_string_t name = (*__match_val_8).data.Assign.f0;
    Expr* value = (*__match_val_8).data.Assign.f1;
Type vt = ({ NyxResult _qres = TypeChecker_check_expr(self, value); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
__match_result_8 = ({
NyxResult __match_result_20;
NyxOption __match_val_20 = TypeChecker_lookup(self, name);
if (__match_val_20.tag == OPTION_SOME) {
__match_result_20 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = vt; &_stmp_; }); _tmp; });
}
 else {
__match_result_20 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Undefined variable: "), name); _stmp_.data; }); _tmp; });
}
__match_result_20;
});
}
 else if ((*__match_val_8).tag == TAG_Expr_Return) {
    Expr* e = (Expr*)((*__match_val_8).data.Return.f0.data.ptr_val);
__match_result_8 = TypeChecker_check_expr(self, e);
}
 else if ((*__match_val_8).tag == TAG_Expr_Return) {
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_StructLiteral) {
    rt_string_t name = (*__match_val_8).data.StructLiteral.f0;
    NyxVec fields = (*__match_val_8).data.StructLiteral.f2;
for (int64_t _idx___val = 0; _idx___val < fields.len; _idx___val++) {
    NyxTuple2 _tup___val = *(NyxTuple2*)rt_vec_get(fields, _idx___val);
    int64_t _ = (int64_t)(_tup___val.f0);
    Expr* val = (Expr*)(_tup___val.f1);
({ NyxResult _qres = TypeChecker_check_expr(self, val); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
}
;
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Custom, .data.Custom = {.f0 = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })} }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_FieldAccess) {
    Expr* obj = (*__match_val_8).data.FieldAccess.f0;
    rt_string_t field = (*__match_val_8).data.FieldAccess.f1;
({ NyxResult _qres = TypeChecker_check_expr(self, obj); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_Lambda) {
    NyxVec params = (*__match_val_8).data.Lambda.f0;
    Expr* body = (*__match_val_8).data.Lambda.f1;
TypeChecker_push_scope(self);
for (int64_t _p = 0; _p < params.len; _p++) {
    Param p = *(Param*)rt_vec_get(params, _p);
TypeChecker_add_var(self, ({ rt_string_t _c = p.name; rt_string_retain(&_c); _c; }), p.ty);
}
;
Type bt = ({ NyxResult _qres = TypeChecker_check_expr(self, body); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
TypeChecker_pop_scope(self);
NyxVec param_types = rt_vec_new(sizeof(int64_t));
for (int64_t _p = 0; _p < params.len; _p++) {
    Param p = *(Param*)rt_vec_get(params, _p);
rt_vec_push(&param_types, ({ Type _tmp_21 = p.ty; &_tmp_21; }));
}
;
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Func, .data.Func = {.f0 = param_types, .f1 = ({ Type _addr_tmp_1 = bt; &_addr_tmp_1; })} }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_Await) {
    Expr* e = (*__match_val_8).data.Await.f0;
__match_result_8 = TypeChecker_check_expr(self, e);
}
 else if ((*__match_val_8).tag == TAG_Expr_Question) {
    Expr* e = (*__match_val_8).data.Question.f0;
__match_result_8 = TypeChecker_check_expr(self, e);
}
 else {
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
__match_result_8;
});
}

NyxResult TypeChecker_check_stmt(TypeChecker* self, Stmt* stmt) {
return ({
NyxResult __match_result_22;
Stmt* __match_val_22 = stmt;
if ((*__match_val_22).tag == TAG_Stmt_Let) {
    rt_string_t name = (*__match_val_22).data.Let.f0;
    NyxOption ty = (*__match_val_22).data.Let.f2;
    Expr value = (*__match_val_22).data.Let.f3;
Type vt = ({ NyxResult _qres = TypeChecker_check_expr(self, &(value)); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
/* iflet */ 0;
TypeChecker_add_var(self, ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), vt);
__match_result_22 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = vt; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_22).tag == TAG_Stmt_Expr) {
    Expr e = (*__match_val_22).data.Expr.f0;
__match_result_22 = TypeChecker_check_expr(self, &(e));
}
 else if ((*__match_val_22).tag == TAG_Stmt_Return) {
    Expr* e = (Expr*)((*__match_val_22).data.Return.f0.data.ptr_val);
__match_result_22 = TypeChecker_check_expr(self, e);
}
 else if ((*__match_val_22).tag == TAG_Stmt_Return) {
__match_result_22 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
__match_result_22;
});
}

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

rt_string_t generate_llvm(NyxVec*);
// module codegen_llvm
// import ast::*
rt_string_t generate_llvm(NyxVec* program) {
rt_string_t out = rt_string_from("; ModuleID = 'nyx_module'\\n");
out = rt_string_concat(out, rt_string_from("source_filename = \\\"nyx_main.nyx\\\"\\n"));
out = rt_string_concat(out, rt_string_from("target datalayout = \\\"e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128\\\"\\n"));
out = rt_string_concat(out, rt_string_from("target triple = \\\"x86_64-pc-windows-msvc\\\"\\n\\n"));
out = rt_string_concat(out, rt_string_from("; Declare external C runtime functions\\n"));
out = rt_string_concat(out, rt_string_from("declare i32 @printf(i8*, ...)\\n"));
out = rt_string_concat(out, rt_string_from("declare i8* @malloc(i64)\\n"));
out = rt_string_concat(out, rt_string_from("declare void @free(i8*)\\n"));
out = rt_string_concat(out, rt_string_from("declare void @rt_ui_init()\\n"));
out = rt_string_concat(out, rt_string_from("declare void @rt_ui_poll_events()\\n\\n"));
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
({
rt_string_t __match_result_0;
TopLevel __match_val_0 = item;
if (__match_val_0.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_0.data.FnDecl.f0;
    NyxVec generics = __match_val_0.data.FnDecl.f1;
    NyxVec params = __match_val_0.data.FnDecl.f2;
    Type ret_type = __match_val_0.data.FnDecl.f3;
    Expr body = __match_val_0.data.FnDecl.f4;
    int is_async = __match_val_0.data.FnDecl.f5;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("define i32 @")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("() {\\n"));
out = rt_string_concat(out, rt_string_from("entry:\\n"));
out = rt_string_concat(out, rt_string_from("  %1 = alloca i32, align 4\\n"));
out = rt_string_concat(out, rt_string_from("  store i32 0, i32* %1, align 4\\n"));
out = rt_string_concat(out, rt_string_from("  ret i32 0\\n"));
__match_result_0 = (out = rt_string_concat(out, rt_string_from("}\\n\\n")));
}
 else {
}
__match_result_0;
});
}
;
out = rt_string_concat(out, rt_string_from("; Entry point for Native Object Generator\\n"));
out = rt_string_concat(out, rt_string_from("define i32 @main(i32 %argc, i8** %argv) {\\n"));
out = rt_string_concat(out, rt_string_from("entry:\\n"));
out = rt_string_concat(out, rt_string_from("  call void @rt_ui_init()\\n"));
out = rt_string_concat(out, rt_string_from("  ret i32 0\\n"));
out = rt_string_concat(out, rt_string_from("}\\n"));
return out;
}

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

rt_string_t generate_mlir(NyxVec*);
rt_string_t mlir_type(Type*);
rt_string_t emit_mlir_body(Expr*);
rt_string_t emit_mlir_stmt(Stmt*);
rt_string_t emit_mlir_expr(Expr*);
rt_string_t generate_llvm_dialect(NyxVec*);
rt_string_t llvm_type(Type*);
rt_string_t emit_llvm_body(Expr*);
rt_string_t emit_llvm_stmt(Stmt*);
rt_string_t emit_llvm_expr(Expr*);
// module codegen_mlir
// import ast::*
rt_string_t generate_mlir(NyxVec* program) {
rt_string_t out = rt_string_from("// Nyx MLIR Dialect IR\\n");
out = rt_string_concat(out, rt_string_from("// dialects: nyx.func, nyx.struct, nyx.region, nyx.async\\n\\n"));
out = rt_string_concat(out, rt_string_from("module {\\n"));
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
({
rt_string_t __match_result_0;
TopLevel __match_val_0 = item;
if (__match_val_0.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_0.data.FnDecl.f0;
    NyxVec generics = __match_val_0.data.FnDecl.f1;
    NyxVec params = __match_val_0.data.FnDecl.f2;
    Type ret_type = __match_val_0.data.FnDecl.f3;
    Expr body = __match_val_0.data.FnDecl.f4;
    int is_async = __match_val_0.data.FnDecl.f5;
({ rt_string_t __if_res_2; if (is_async) {
__if_res_2 = (out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  nyx.async.func @")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("(")));
} else {
__if_res_2 = (out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  nyx.func @")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("(")));
}
__if_res_2; });
for (int64_t i = 0LL; i < params.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("%")), ({ rt_string_t _c = ((*(Param*)rt_vec_get(params, i))).name; rt_string_retain(&_c); _c; })), rt_string_from(": ")), mlir_type((&(((*(Param*)rt_vec_get(params, i))).ty))));
}
;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from(") -> ")), mlir_type(&(ret_type))), rt_string_from(" {\\n"));
out = rt_string_concat(out, rt_string_from("  ^bb0:\\n"));
out = rt_string_concat(out, emit_mlir_body(&(body)));
__match_result_0 = (out = rt_string_concat(out, rt_string_from("  }\\n\\n")));
}
 else if (__match_val_0.tag == TAG_TopLevel_StructDef) {
    rt_string_t name = __match_val_0.data.StructDef.f0;
    NyxVec generics = __match_val_0.data.StructDef.f1;
    NyxVec fields = __match_val_0.data.StructDef.f2;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  nyx.struct @")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" {\\n"));
for (int64_t _idx_fname_fty = 0; _idx_fname_fty < fields.len; _idx_fname_fty++) {
    NyxTuple2 _tup_fname_fty = *(NyxTuple2*)rt_vec_get(fields, _idx_fname_fty);
    rt_string_t fname = *(rt_string_t*)(_tup_fname_fty.f0);
    Type fty = *(Type*)(_tup_fname_fty.f1);
out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("    ")), ({ rt_string_t _c = fname; rt_string_retain(&_c); _c; })), rt_string_from(": ")), mlir_type((&(fty)))), rt_string_from(",\\n"));
}
;
__match_result_0 = (out = rt_string_concat(out, rt_string_from("  }\\n\\n")));
}
 else if (__match_val_0.tag == TAG_TopLevel_EnumDef) {
    rt_string_t name = __match_val_0.data.EnumDef.f0;
    NyxVec generics = __match_val_0.data.EnumDef.f1;
    NyxVec variants = __match_val_0.data.EnumDef.f2;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  nyx.enum @")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" {\\n"));
for (int64_t _idx_vname_vtypes = 0; _idx_vname_vtypes < variants.len; _idx_vname_vtypes++) {
    NyxTuple2 _tup_vname_vtypes = *(NyxTuple2*)rt_vec_get(variants, _idx_vname_vtypes);
    rt_string_t vname = *(rt_string_t*)(_tup_vname_vtypes.f0);
    NyxVec vtypes = *(NyxVec*)(_tup_vname_vtypes.f1);
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("    ")), ({ rt_string_t _c = vname; rt_string_retain(&_c); _c; })), rt_string_from("("));
for (int64_t i = 0LL; i < vtypes.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(out, mlir_type((&((*(Type*)rt_vec_get(vtypes, i))))));
}
;
out = rt_string_concat(out, rt_string_from(")\\n"));
}
;
__match_result_0 = (out = rt_string_concat(out, rt_string_from("  }\\n\\n")));
}
 else if (__match_val_0.tag == TAG_TopLevel_Module) {
    rt_string_t name = __match_val_0.data.Module.f0;
__match_result_0 = (out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  // module ")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("\\n\\n")));
}
 else if (__match_val_0.tag == TAG_TopLevel_Import) {
    NyxVec path = __match_val_0.data.Import.f0;
out = rt_string_concat(out, rt_string_from("  // import "));
for (int64_t i = 0LL; i < path.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from("."));
}
});
out = rt_string_concat(out, ({ rt_string_t _c = (*(rt_string_t*)rt_vec_get(path, i)); rt_string_retain(&_c); _c; }));
}
;
__match_result_0 = (out = rt_string_concat(out, rt_string_from("\\n\\n")));
}
 else {
}
__match_result_0;
});
}
;
out = rt_string_concat(out, rt_string_from("}\\n"));
return out;
}

rt_string_t mlir_type(Type* ty) {
return ({
rt_string_t __match_result_5;
Type* __match_val_5 = ty;
if ((*__match_val_5).tag == TAG_Type_Int) {
__match_result_5 = rt_string_from("i64");
}
 else if ((*__match_val_5).tag == TAG_Type_Float) {
__match_result_5 = rt_string_from("f64");
}
 else if ((*__match_val_5).tag == TAG_Type_Bool) {
__match_result_5 = rt_string_from("i1");
}
 else if ((*__match_val_5).tag == TAG_Type_String) {
__match_result_5 = rt_string_from("!nyx.str");
}
 else if ((*__match_val_5).tag == TAG_Type_Char) {
__match_result_5 = rt_string_from("!nyx.char");
}
 else if ((*__match_val_5).tag == TAG_Type_Void) {
__match_result_5 = rt_string_from("none");
}
 else if ((*__match_val_5).tag == TAG_Type_Custom) {
    rt_string_t n = (*__match_val_5).data.Custom.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.struct<@"), ({ rt_string_t _c = n; rt_string_retain(&_c); _c; })), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Generic) {
    rt_string_t n = (*__match_val_5).data.Generic.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.generic<"), ({ rt_string_t _c = n; rt_string_retain(&_c); _c; })), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Ref) {
    Type* inner = (*__match_val_5).data.Ref.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.ref<"), mlir_type(inner)), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_MutRef) {
    Type* inner = (*__match_val_5).data.MutRef.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.mutref<"), mlir_type(inner)), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Array) {
    Type* inner = (*__match_val_5).data.Array.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.array<"), mlir_type(inner)), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Option) {
    Type* inner = (*__match_val_5).data.Option.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.option<"), mlir_type(inner)), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Result) {
    Type* ok = (*__match_val_5).data.Result.f0;
    Type* err = (*__match_val_5).data.Result.f1;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("!nyx.result<"), mlir_type(ok)), rt_string_from(", ")), mlir_type(err)), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Tuple) {
    NyxVec types = (*__match_val_5).data.Tuple.f0;
rt_string_t s = rt_string_from("!nyx.tuple<");
for (int64_t i = 0LL; i < types.len; i++) {
({ if ((i > 0LL)) {
s = rt_string_concat(s, rt_string_from(", "));
}
});
s = rt_string_concat(s, mlir_type((&((*(Type*)rt_vec_get(types, i))))));
}
;
__match_result_5 = rt_string_concat(s, rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Func) {
    NyxVec params = (*__match_val_5).data.Func.f0;
    Type* ret = (*__match_val_5).data.Func.f1;
rt_string_t s = rt_string_from("!nyx.func<(");
for (int64_t i = 0LL; i < params.len; i++) {
({ if ((i > 0LL)) {
s = rt_string_concat(s, rt_string_from(", "));
}
});
s = rt_string_concat(s, mlir_type((&((*(Type*)rt_vec_get(params, i))))));
}
;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_concat(s, rt_string_from(") -> ")), mlir_type(ret)), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Region) {
    rt_string_t name = (*__match_val_5).data.Region.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.region<'"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Recursive) {
    rt_string_t name = (*__match_val_5).data.Recursive.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.rec<"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Unknown) {
__match_result_5 = rt_string_from("!nyx.unknown");
}
__match_result_5;
});
}

rt_string_t emit_mlir_body(Expr* body) {
return ({
rt_string_t __match_result_8;
Expr* __match_val_8 = body;
if ((*__match_val_8).tag == TAG_Expr_Block) {
    NyxVec stmts = (*__match_val_8).data.Block.f0;
rt_string_t out = rt_string_from("");
for (int64_t _stmt = 0; _stmt < stmts.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(stmts, _stmt);
out = rt_string_concat(out, emit_mlir_stmt(&(stmt)));
}
;
__match_result_8 = out;
}
 else {
__match_result_8 = rt_string_concat(rt_string_concat(rt_string_from("    "), emit_mlir_expr(body)), rt_string_from("\\n"));
}
__match_result_8;
});
}

rt_string_t emit_mlir_stmt(Stmt* stmt) {
return ({
rt_string_t __match_result_9;
Stmt* __match_val_9 = stmt;
if ((*__match_val_9).tag == TAG_Stmt_Let) {
    rt_string_t name = (*__match_val_9).data.Let.f0;
    Expr value = (*__match_val_9).data.Let.f3;
    NyxOption ty = (*__match_val_9).data.Let.f2;
    int mutable = (*__match_val_9).data.Let.f1;
rt_string_t out = rt_string_concat(rt_string_from("    %"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }));
({ if (mutable) {
out = rt_string_concat(out, rt_string_from(" mut"));
}
});
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from(" = ")), emit_mlir_expr(&(value))), rt_string_from("\\n"));
__match_result_9 = out;
}
 else if ((*__match_val_9).tag == TAG_Stmt_Expr) {
    Expr expr = (*__match_val_9).data.Expr.f0;
__match_result_9 = rt_string_concat(rt_string_concat(rt_string_from("    "), emit_mlir_expr(&(expr))), rt_string_from("\\n"));
}
 else if ((*__match_val_9).tag == TAG_Stmt_Return) {
    Expr* e = (Expr*)((*__match_val_9).data.Return.f0.data.ptr_val);
__match_result_9 = rt_string_concat(rt_string_concat(rt_string_from("    nyx.return "), emit_mlir_expr(e)), rt_string_from("\\n"));
}
 else if ((*__match_val_9).tag == TAG_Stmt_Return) {
__match_result_9 = rt_string_from("    nyx.return\\n");
}
__match_result_9;
});
}

rt_string_t emit_mlir_expr(Expr* expr) {
return ({
rt_string_t __match_result_11;
Expr* __match_val_11 = expr;
if ((*__match_val_11).tag == TAG_Expr_Lit) {
    Literal lit = (*__match_val_11).data.Lit.f0;
__match_result_11 = ({
rt_string_t __match_result_12;
Literal __match_val_12 = lit;
if (__match_val_12.tag == TAG_Literal_Int) {
    int64_t n = __match_val_12.data.Int.f0;
__match_result_12 = ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)n); _buf.ref_count = 1; _buf; });
}
 else if (__match_val_12.tag == TAG_Literal_Float) {
    double n = __match_val_12.data.Float.f0;
__match_result_12 = rt_string_concat(rt_string_from("nyx.const "), ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", n); _buf.ref_count = 1; _buf; }));
}
 else if (__match_val_12.tag == TAG_Literal_Bool) {
    int b = __match_val_12.data.Bool.f0;
if (b) {
__match_result_12 = rt_string_from("true");
} else {
__match_result_12 = rt_string_from("false");
}
}
 else if (__match_val_12.tag == TAG_Literal_String) {
    rt_string_t s = __match_val_12.data.String.f0;
__match_result_12 = rt_string_concat(rt_string_concat(rt_string_from("nyx.str \\\""), ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })), rt_string_from("\\\""));
}
 else if (__match_val_12.tag == TAG_Literal_Char) {
    char c = __match_val_12.data.Char.f0;
__match_result_12 = rt_string_concat(rt_string_concat(rt_string_from("nyx.char '"), ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = c; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; })), rt_string_from("'"));
}
__match_result_12;
});
}
 else if ((*__match_val_11).tag == TAG_Expr_Ident) {
    rt_string_t name = (*__match_val_11).data.Ident.f0;
__match_result_11 = rt_string_concat(rt_string_from("%"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }));
}
 else if ((*__match_val_11).tag == TAG_Expr_Binary) {
    BinOp op = (*__match_val_11).data.Binary.f0;
    Expr* left = (*__match_val_11).data.Binary.f1;
    Expr* right = (*__match_val_11).data.Binary.f2;
rt_string_t op_str = ({
rt_string_t __match_result_13;
BinOp __match_val_13 = op;
if (__match_val_13.tag == TAG_BinOp_Add) {
__match_result_13 = rt_string_from("nyx.add");
}
 else if (__match_val_13.tag == TAG_BinOp_Sub) {
__match_result_13 = rt_string_from("nyx.sub");
}
 else if (__match_val_13.tag == TAG_BinOp_Mul) {
__match_result_13 = rt_string_from("nyx.mul");
}
 else if (__match_val_13.tag == TAG_BinOp_Div) {
__match_result_13 = rt_string_from("nyx.div");
}
 else if (__match_val_13.tag == TAG_BinOp_Mod) {
__match_result_13 = rt_string_from("nyx.mod");
}
 else if (__match_val_13.tag == TAG_BinOp_Eq) {
__match_result_13 = rt_string_from("nyx.cmp_eq");
}
 else if (__match_val_13.tag == TAG_BinOp_NotEq) {
__match_result_13 = rt_string_from("nyx.cmp_ne");
}
 else if (__match_val_13.tag == TAG_BinOp_Lt) {
__match_result_13 = rt_string_from("nyx.cmp_lt");
}
 else if (__match_val_13.tag == TAG_BinOp_LtEq) {
__match_result_13 = rt_string_from("nyx.cmp_lte");
}
 else if (__match_val_13.tag == TAG_BinOp_Gt) {
__match_result_13 = rt_string_from("nyx.cmp_gt");
}
 else if (__match_val_13.tag == TAG_BinOp_GtEq) {
__match_result_13 = rt_string_from("nyx.cmp_gte");
}
 else if (__match_val_13.tag == TAG_BinOp_And) {
__match_result_13 = rt_string_from("nyx.and");
}
 else if (__match_val_13.tag == TAG_BinOp_Or) {
__match_result_13 = rt_string_from("nyx.or");
}
__match_result_13;
});
__match_result_11 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(op_str, rt_string_from(" ")), emit_mlir_expr(left)), rt_string_from(", ")), emit_mlir_expr(right));
}
 else if ((*__match_val_11).tag == TAG_Expr_Call) {
    Expr* func = (*__match_val_11).data.Call.f0;
    NyxVec args = (*__match_val_11).data.Call.f2;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_from("call "), emit_mlir_expr(func)), rt_string_from("("));
for (int64_t i = 0LL; i < args.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(out, emit_mlir_expr((&((*(Expr*)rt_vec_get(args, i))))));
}
;
__match_result_11 = rt_string_concat(out, rt_string_from(")"));
}
 else if ((*__match_val_11).tag == TAG_Expr_MethodCall) {
    Expr* receiver = (*__match_val_11).data.MethodCall.f0;
    rt_string_t method = (*__match_val_11).data.MethodCall.f1;
    NyxVec args = (*__match_val_11).data.MethodCall.f3;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("call "), emit_mlir_expr(receiver)), rt_string_from(".")), ({ rt_string_t _c = method; rt_string_retain(&_c); _c; })), rt_string_from("("));
for (int64_t i = 0LL; i < args.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(out, emit_mlir_expr((&((*(Expr*)rt_vec_get(args, i))))));
}
;
__match_result_11 = rt_string_concat(out, rt_string_from(")"));
}
 else if ((*__match_val_11).tag == TAG_Expr_If) {
    Expr* cond = (*__match_val_11).data.If.f0;
    NyxVec then = (*__match_val_11).data.If.f1;
    NyxVec else_ifs = (*__match_val_11).data.If.f2;
    NyxOption else_block = (*__match_val_11).data.If.f3;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_from("nyx.if "), emit_mlir_expr(cond)), rt_string_from(" {\\n"));
for (int64_t _stmt = 0; _stmt < then.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(then, _stmt);
out = rt_string_concat(out, emit_mlir_stmt(&(stmt)));
}
;
for (int64_t _idx_elif_cond_elif_body = 0; _idx_elif_cond_elif_body < else_ifs.len; _idx_elif_cond_elif_body++) {
    NyxTuple2 _tup_elif_cond_elif_body = *(NyxTuple2*)rt_vec_get(else_ifs, _idx_elif_cond_elif_body);
    Expr* elif_cond = (Expr*)(_tup_elif_cond_elif_body.f0);
    NyxVec elif_body = *(NyxVec*)(_tup_elif_cond_elif_body.f1);
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  } nyx.if ")), emit_mlir_expr(elif_cond)), rt_string_from(" {\\n"));
for (int64_t _stmt = 0; _stmt < elif_body.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(elif_body, _stmt);
out = rt_string_concat(out, emit_mlir_stmt(&(stmt)));
}
;
}
;
({
int64_t __match_result_16;
NyxOption __match_val_16 = else_block;
if (__match_val_16.tag == OPTION_SOME) {
    void* stmts = __match_val_16.data.ptr_val;
out = rt_string_concat(out, rt_string_from("  } nyx.else {\\n"));
for (int64_t _stmt = 0; _stmt < ((NyxVec*)(stmts))->len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(*((NyxVec*)(stmts)), _stmt);
out = rt_string_concat(out, emit_mlir_stmt(&(stmt)));
}
;
}
 else if (__match_val_16.tag == OPTION_NONE) {
}
__match_result_16;
});
__match_result_11 = rt_string_concat(out, rt_string_from("  }\\n"));
}
 else if ((*__match_val_11).tag == TAG_Expr_While) {
    Expr* cond = (*__match_val_11).data.While.f0;
    NyxVec body = (*__match_val_11).data.While.f1;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_from("nyx.while "), emit_mlir_expr(cond)), rt_string_from(" {\\n"));
for (int64_t _stmt = 0; _stmt < body.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(body, _stmt);
out = rt_string_concat(out, emit_mlir_stmt(&(stmt)));
}
;
__match_result_11 = rt_string_concat(out, rt_string_from("  }\\n"));
}
 else if ((*__match_val_11).tag == TAG_Expr_For) {
    rt_string_t var = (*__match_val_11).data.For.f0;
    Expr* iter = (*__match_val_11).data.For.f1;
    NyxVec body = (*__match_val_11).data.For.f2;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("nyx.for %"), ({ rt_string_t _c = var; rt_string_retain(&_c); _c; })), rt_string_from(" in ")), emit_mlir_expr(iter)), rt_string_from(" {\\n"));
for (int64_t _stmt = 0; _stmt < body.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(body, _stmt);
out = rt_string_concat(out, emit_mlir_stmt(&(stmt)));
}
;
__match_result_11 = rt_string_concat(out, rt_string_from("  }\\n"));
}
 else if ((*__match_val_11).tag == TAG_Expr_StructLiteral) {
    rt_string_t name = (*__match_val_11).data.StructLiteral.f0;
    NyxVec fields = (*__match_val_11).data.StructLiteral.f2;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_from("nyx.struct "), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" {"));
int first = 1;
for (int64_t _idx_fname_fval = 0; _idx_fname_fval < fields.len; _idx_fname_fval++) {
    NyxTuple2 _tup_fname_fval = *(NyxTuple2*)rt_vec_get(fields, _idx_fname_fval);
    rt_string_t fname = *(rt_string_t*)(_tup_fname_fval.f0);
    Expr* fval = (Expr*)(_tup_fname_fval.f1);
({ if ((!first)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
first = 0;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, ({ rt_string_t _c = fname; rt_string_retain(&_c); _c; })), rt_string_from(": ")), emit_mlir_expr(fval));
}
;
__match_result_11 = rt_string_concat(out, rt_string_from("}"));
}
 else if ((*__match_val_11).tag == TAG_Expr_FieldAccess) {
    Expr* obj = (*__match_val_11).data.FieldAccess.f0;
    rt_string_t field = (*__match_val_11).data.FieldAccess.f1;
__match_result_11 = rt_string_concat(rt_string_concat(emit_mlir_expr(obj), rt_string_from(".")), ({ rt_string_t _c = field; rt_string_retain(&_c); _c; }));
}
 else if ((*__match_val_11).tag == TAG_Expr_Return) {
    Expr* e = (Expr*)((*__match_val_11).data.Return.f0.data.ptr_val);
__match_result_11 = rt_string_concat(rt_string_from("nyx.return "), emit_mlir_expr(e));
}
 else if ((*__match_val_11).tag == TAG_Expr_Return) {
__match_result_11 = rt_string_from("nyx.return");
}
 else if ((*__match_val_11).tag == TAG_Expr_Block) {
    NyxVec stmts = (*__match_val_11).data.Block.f0;
rt_string_t out = rt_string_from("{\\n");
for (int64_t _stmt = 0; _stmt < stmts.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(stmts, _stmt);
out = rt_string_concat(out, emit_mlir_stmt(&(stmt)));
}
;
__match_result_11 = rt_string_concat(out, rt_string_from("}"));
}
 else if ((*__match_val_11).tag == TAG_Expr_Let) {
    rt_string_t name = (*__match_val_11).data.Let.f0;
    Expr* value = (*__match_val_11).data.Let.f1;
__match_result_11 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("%"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" = ")), emit_mlir_expr(value));
}
 else if ((*__match_val_11).tag == TAG_Expr_Unary) {
    UnaryOp op = (*__match_val_11).data.Unary.f0;
    Expr* expr = (*__match_val_11).data.Unary.f1;
__match_result_11 = ({
rt_string_t __match_result_18;
UnaryOp __match_val_18 = op;
if (__match_val_18.tag == TAG_UnaryOp_Not) {
__match_result_18 = rt_string_concat(rt_string_from("nyx.not "), emit_mlir_expr(expr));
}
 else if (__match_val_18.tag == TAG_UnaryOp_Neg) {
__match_result_18 = rt_string_concat(rt_string_from("nyx.neg "), emit_mlir_expr(expr));
}
 else if (__match_val_18.tag == TAG_UnaryOp_Ref) {
__match_result_18 = rt_string_concat(rt_string_from("nyx.ref "), emit_mlir_expr(expr));
}
 else if (__match_val_18.tag == TAG_UnaryOp_Deref) {
__match_result_18 = rt_string_concat(rt_string_from("nyx.deref "), emit_mlir_expr(expr));
}
__match_result_18;
});
}
 else if ((*__match_val_11).tag == TAG_Expr_Match) {
    Expr* expr = (*__match_val_11).data.Match.f0;
    NyxVec arms = (*__match_val_11).data.Match.f1;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_from("nyx.match "), emit_mlir_expr(expr)), rt_string_from(" {\\n"));
for (int64_t _arm = 0; _arm < arms.len; _arm++) {
    MatchArm arm = *(MatchArm*)rt_vec_get(arms, _arm);
out = rt_string_concat(out, rt_string_from("  "));
({
rt_string_t __match_result_19;
Pattern* __match_val_19 = (&(arm.pattern));
if ((*__match_val_19).tag == TAG_Pattern_Ident) {
    rt_string_t s = (*__match_val_19).data.Ident.f0;
__match_result_19 = (out = rt_string_concat(rt_string_concat(out, rt_string_from("%")), ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })));
}
 else if ((*__match_val_19).tag == TAG_Pattern_Wildcard) {
__match_result_19 = (out = rt_string_concat(out, rt_string_from("_")));
}
 else if ((*__match_val_19).tag == TAG_Pattern_Lit) {
    Literal lit = (*__match_val_19).data.Lit.f0;
__match_result_19 = ({
rt_string_t __match_result_20;
Literal __match_val_20 = lit;
if (__match_val_20.tag == TAG_Literal_Int) {
    int64_t n = __match_val_20.data.Int.f0;
__match_result_20 = (out = rt_string_concat(out, ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)n); _buf.ref_count = 1; _buf; })));
}
 else if (__match_val_20.tag == TAG_Literal_String) {
    rt_string_t s = __match_val_20.data.String.f0;
__match_result_20 = (out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("\\\"")), ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })), rt_string_from("\\\"")));
}
 else if (__match_val_20.tag == TAG_Literal_Bool) {
    int b = __match_val_20.data.Bool.f0;
__match_result_20 = (out = rt_string_concat(out, ({ rt_string_t __if_res_22; if (b) {
__if_res_22 = rt_string_from("true");
} else {
__if_res_22 = rt_string_from("false");
}
__if_res_22; })));
}
 else {
__match_result_20 = (out = rt_string_concat(out, rt_string_from("?")));
}
__match_result_20;
});
}
 else if ((*__match_val_19).tag == TAG_Pattern_Enum) {
    rt_string_t variant = (*__match_val_19).data.Enum.f1;
__match_result_19 = (out = rt_string_concat(out, ({ rt_string_t _c = variant; rt_string_retain(&_c); _c; })));
}
__match_result_19;
});
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from(" => ")), emit_mlir_expr((&(arm.body)))), rt_string_from("\\n"));
}
;
__match_result_11 = rt_string_concat(out, rt_string_from("}\\n"));
}
 else if ((*__match_val_11).tag == TAG_Expr_Lambda) {
    NyxVec params = (*__match_val_11).data.Lambda.f0;
    Expr* body = (*__match_val_11).data.Lambda.f1;
rt_string_t out = rt_string_from("nyx.lambda (");
for (int64_t i = 0LL; i < params.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("%")), ({ rt_string_t _c = ((*(Param*)rt_vec_get(params, i))).name; rt_string_retain(&_c); _c; })), rt_string_from(": ")), mlir_type((&(((*(Param*)rt_vec_get(params, i))).ty))));
}
;
__match_result_11 = rt_string_concat(rt_string_concat(out, rt_string_from(") => ")), emit_mlir_expr(body));
}
 else if ((*__match_val_11).tag == TAG_Expr_Await) {
    Expr* e = (*__match_val_11).data.Await.f0;
__match_result_11 = rt_string_concat(rt_string_from("nyx.await "), emit_mlir_expr(e));
}
 else if ((*__match_val_11).tag == TAG_Expr_Assign) {
    rt_string_t name = (*__match_val_11).data.Assign.f0;
    Expr* value = (*__match_val_11).data.Assign.f1;
__match_result_11 = rt_string_concat(rt_string_concat(({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), rt_string_from(" = ")), emit_mlir_expr(value));
}
 else if ((*__match_val_11).tag == TAG_Expr_Question) {
    Expr* e = (*__match_val_11).data.Question.f0;
__match_result_11 = rt_string_concat(emit_mlir_expr(e), rt_string_from("?"));
}
 else {
__match_result_11 = rt_string_from("");
}
__match_result_11;
});
}

rt_string_t generate_llvm_dialect(NyxVec* program) {
rt_string_t out = rt_string_from("; Nyx -> LLVM Dialect\\n");
out = rt_string_concat(out, rt_string_from("module {\\n"));
out = rt_string_concat(out, rt_string_from("  llvm.func @nyx_println(i64) -> none\\n"));
out = rt_string_concat(out, rt_string_from("  llvm.func @nyx_print_int(i64) -> none\\n"));
out = rt_string_concat(out, rt_string_from("  llvm.func @nyx_print_f64(f64) -> none\\n\\n"));
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
({
rt_string_t __match_result_23;
TopLevel __match_val_23 = item;
if (__match_val_23.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_23.data.FnDecl.f0;
    NyxVec params = __match_val_23.data.FnDecl.f2;
    Type ret_type = __match_val_23.data.FnDecl.f3;
    Expr body = __match_val_23.data.FnDecl.f4;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  llvm.func @")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("("));
for (int64_t i = 0LL; i < params.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("%")), ({ rt_string_t _c = ((*(Param*)rt_vec_get(params, i))).name; rt_string_retain(&_c); _c; })), rt_string_from(": ")), llvm_type((&(((*(Param*)rt_vec_get(params, i))).ty))));
}
;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from(") -> ")), llvm_type(&(ret_type))), rt_string_from(" {\\n"));
out = rt_string_concat(out, rt_string_from("  ^bb0:\\n"));
out = rt_string_concat(out, emit_llvm_body(&(body)));
__match_result_23 = (out = rt_string_concat(out, rt_string_from("  }\\n\\n")));
}
 else if (__match_val_23.tag == TAG_TopLevel_StructDef) {
    rt_string_t name = __match_val_23.data.StructDef.f0;
    NyxVec fields = __match_val_23.data.StructDef.f2;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  ; struct ")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" {\\n"));
for (int64_t _idx_fname_fty = 0; _idx_fname_fty < fields.len; _idx_fname_fty++) {
    NyxTuple2 _tup_fname_fty = *(NyxTuple2*)rt_vec_get(fields, _idx_fname_fty);
    rt_string_t fname = *(rt_string_t*)(_tup_fname_fty.f0);
    Type fty = *(Type*)(_tup_fname_fty.f1);
out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  ;   ")), ({ rt_string_t _c = fname; rt_string_retain(&_c); _c; })), rt_string_from(": ")), llvm_type((&(fty)))), rt_string_from("\\n"));
}
;
__match_result_23 = (out = rt_string_concat(out, rt_string_from("  ; }\\n\\n")));
}
 else {
}
__match_result_23;
});
}
;
out = rt_string_concat(out, rt_string_from("}\\n"));
return out;
}

rt_string_t llvm_type(Type* ty) {
return ({
rt_string_t __match_result_25;
Type* __match_val_25 = ty;
if ((*__match_val_25).tag == TAG_Type_Int) {
__match_result_25 = rt_string_from("i64");
}
 else if ((*__match_val_25).tag == TAG_Type_Float) {
__match_result_25 = rt_string_from("f64");
}
 else if ((*__match_val_25).tag == TAG_Type_Bool) {
__match_result_25 = rt_string_from("i1");
}
 else if ((*__match_val_25).tag == TAG_Type_String) {
__match_result_25 = rt_string_from("i8*");
}
 else if ((*__match_val_25).tag == TAG_Type_Char) {
__match_result_25 = rt_string_from("i8");
}
 else if ((*__match_val_25).tag == TAG_Type_Void) {
__match_result_25 = rt_string_from("void");
}
 else {
__match_result_25 = rt_string_from("i8*");
}
__match_result_25;
});
}

rt_string_t emit_llvm_body(Expr* body) {
return ({
rt_string_t __match_result_26;
Expr* __match_val_26 = body;
if ((*__match_val_26).tag == TAG_Expr_Block) {
    NyxVec stmts = (*__match_val_26).data.Block.f0;
rt_string_t out = rt_string_from("");
for (int64_t _stmt = 0; _stmt < stmts.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(stmts, _stmt);
out = rt_string_concat(out, emit_llvm_stmt(&(stmt)));
}
;
__match_result_26 = out;
}
 else {
__match_result_26 = rt_string_from("");
}
__match_result_26;
});
}

rt_string_t emit_llvm_stmt(Stmt* stmt) {
return ({
rt_string_t __match_result_27;
Stmt* __match_val_27 = stmt;
if ((*__match_val_27).tag == TAG_Stmt_Let) {
    rt_string_t name = (*__match_val_27).data.Let.f0;
    Expr value = (*__match_val_27).data.Let.f3;
__match_result_27 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("    %"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" = ")), emit_llvm_expr(&(value))), rt_string_from("\\n"));
}
 else if ((*__match_val_27).tag == TAG_Stmt_Expr) {
    Expr expr = (*__match_val_27).data.Expr.f0;
__match_result_27 = rt_string_concat(rt_string_concat(rt_string_from("    "), emit_llvm_expr(&(expr))), rt_string_from("\\n"));
}
 else if ((*__match_val_27).tag == TAG_Stmt_Return) {
    Expr* e = (Expr*)((*__match_val_27).data.Return.f0.data.ptr_val);
__match_result_27 = rt_string_concat(rt_string_concat(rt_string_from("    llvm.return "), emit_llvm_expr(e)), rt_string_from("\\n"));
}
 else if ((*__match_val_27).tag == TAG_Stmt_Return) {
__match_result_27 = rt_string_from("    llvm.return\\n");
}
__match_result_27;
});
}

rt_string_t emit_llvm_expr(Expr* expr) {
return ({
rt_string_t __match_result_28;
Expr* __match_val_28 = expr;
if ((*__match_val_28).tag == TAG_Expr_Lit) {
    Literal lit = (*__match_val_28).data.Lit.f0;
__match_result_28 = ({
rt_string_t __match_result_29;
Literal __match_val_29 = lit;
if (__match_val_29.tag == TAG_Literal_Int) {
    int64_t n = __match_val_29.data.Int.f0;
__match_result_29 = ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)n); _buf.ref_count = 1; _buf; });
}
 else if (__match_val_29.tag == TAG_Literal_Float) {
    double n = __match_val_29.data.Float.f0;
__match_result_29 = ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", n); _buf.ref_count = 1; _buf; });
}
 else if (__match_val_29.tag == TAG_Literal_Bool) {
    int b = __match_val_29.data.Bool.f0;
if (b) {
__match_result_29 = rt_string_from("1");
} else {
__match_result_29 = rt_string_from("0");
}
}
 else if (__match_val_29.tag == TAG_Literal_String) {
    rt_string_t s = __match_val_29.data.String.f0;
__match_result_29 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("getelementptr(["), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)s.length); _buf.ref_count = 1; _buf; })), rt_string_from(" x i8], [")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)s.length); _buf.ref_count = 1; _buf; })), rt_string_from(" x i8]* @str_")), ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })), rt_string_from(", i32 0, i32 0)"));
}
 else if (__match_val_29.tag == TAG_Literal_Char) {
    char c = __match_val_29.data.Char.f0;
__match_result_29 = ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = c; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; });
}
__match_result_29;
});
}
 else if ((*__match_val_28).tag == TAG_Expr_Ident) {
    rt_string_t name = (*__match_val_28).data.Ident.f0;
__match_result_28 = rt_string_concat(rt_string_from("%"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }));
}
 else if ((*__match_val_28).tag == TAG_Expr_Binary) {
    BinOp op = (*__match_val_28).data.Binary.f0;
    Expr* left = (*__match_val_28).data.Binary.f1;
    Expr* right = (*__match_val_28).data.Binary.f2;
rt_string_t op_str = ({
rt_string_t __match_result_30;
BinOp __match_val_30 = op;
if (__match_val_30.tag == TAG_BinOp_Add) {
__match_result_30 = rt_string_from("add i64");
}
 else if (__match_val_30.tag == TAG_BinOp_Sub) {
__match_result_30 = rt_string_from("sub i64");
}
 else if (__match_val_30.tag == TAG_BinOp_Mul) {
__match_result_30 = rt_string_from("mul i64");
}
 else if (__match_val_30.tag == TAG_BinOp_Div) {
__match_result_30 = rt_string_from("sdiv i64");
}
 else if (__match_val_30.tag == TAG_BinOp_Mod) {
__match_result_30 = rt_string_from("srem i64");
}
 else if (__match_val_30.tag == TAG_BinOp_Eq) {
__match_result_30 = rt_string_from("icmp eq i64");
}
 else if (__match_val_30.tag == TAG_BinOp_NotEq) {
__match_result_30 = rt_string_from("icmp ne i64");
}
 else if (__match_val_30.tag == TAG_BinOp_Lt) {
__match_result_30 = rt_string_from("icmp slt i64");
}
 else if (__match_val_30.tag == TAG_BinOp_LtEq) {
__match_result_30 = rt_string_from("icmp sle i64");
}
 else if (__match_val_30.tag == TAG_BinOp_Gt) {
__match_result_30 = rt_string_from("icmp sgt i64");
}
 else if (__match_val_30.tag == TAG_BinOp_GtEq) {
__match_result_30 = rt_string_from("icmp sge i64");
}
 else if (__match_val_30.tag == TAG_BinOp_And) {
__match_result_30 = rt_string_from("and i1");
}
 else if (__match_val_30.tag == TAG_BinOp_Or) {
__match_result_30 = rt_string_from("or i1");
}
__match_result_30;
});
__match_result_28 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(op_str, rt_string_from(" ")), emit_llvm_expr(left)), rt_string_from(", ")), emit_llvm_expr(right));
}
 else if ((*__match_val_28).tag == TAG_Expr_Call) {
    Expr* func = (*__match_val_28).data.Call.f0;
    NyxVec args = (*__match_val_28).data.Call.f2;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_from("call "), emit_llvm_expr(func)), rt_string_from("("));
for (int64_t i = 0LL; i < args.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(rt_string_concat(out, rt_string_from("i64 ")), emit_llvm_expr((&((*(Expr*)rt_vec_get(args, i))))));
}
;
__match_result_28 = rt_string_concat(out, rt_string_from(")"));
}
 else {
__match_result_28 = rt_string_from("/* unsupported */");
}
__match_result_28;
});
}

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

typedef struct {
    NyxVec regions;
    int64_t escape_count;
} RegionAnalyzerResult;

typedef struct {
    rt_string_t name;
    rt_string_t kind;
    int has_escapes;
    NyxVec bound_variables;
} RegionInfo;

typedef struct {
    rt_string_t name;
    int escapes;
} VariableInfo;

typedef struct {
    NyxVec specializations;
} MonomorphizeResult;

typedef struct {
    rt_string_t original_name;
    rt_string_t specialized_name;
    NyxVec type_args;
} Specialization;

typedef struct {
    NyxVec closures;
} ClosureConvertResult;

typedef struct {
    rt_string_t name;
    rt_string_t env_struct_name;
    NyxVec captured_vars;
} ClosureInfo;

typedef struct {
    rt_string_t name;
    int requires_heap;
} CapturedVar;

typedef struct {
} RegionAnalyzer;

typedef struct {
} Monomorphizer;

typedef struct {
} ClosureConverter;

void nyx_main();
void print_token(Token*);
void print_top_level(TopLevel*);
void print_type(Type*);
RegionAnalyzer RegionAnalyzer_new();
RegionAnalyzerResult RegionAnalyzer_analyze(RegionAnalyzer*, NyxVec*);
Monomorphizer Monomorphizer_new();
MonomorphizeResult Monomorphizer_monomorphize(Monomorphizer*, NyxVec*);
ClosureConverter ClosureConverter_new();
ClosureConvertResult ClosureConverter_convert(ClosureConverter*, NyxVec*);
// module main
// import std::io
// import std::fs
// import std::env
// import std::string
// import lexer
// import token
// import parser
// import ast
// import typecheck
// import codegen_mlir
// import codegen_llvm
void nyx_main() {
NyxVec args = rt_env_args();
({ if ((args.len < 2LL)) {
rt_println(rt_string_from("Nyx Compiler (nyxc) v0.4.0"));
rt_println(rt_string_from("Usage: nyxc <file.nyx> [options]"));
rt_println(rt_string_from(""));
rt_println(rt_string_from("Options:"));
rt_println(rt_string_from("  --tokens     Print token stream"));
rt_println(rt_string_from("  --ast        Print AST"));
rt_println(rt_string_from("  --types      Run type checking"));
rt_println(rt_string_from("  --regions    Run region inference"));
rt_println(rt_string_from("  --generics   Run monomorphization"));
rt_println(rt_string_from("  --closures   Run closure conversion"));
rt_println(rt_string_from("  --mlir       Generate MLIR code"));
rt_println(rt_string_from("  --llvm       Generate LLVM IR bitcode"));
rt_println(rt_string_from("  --wasm       Target WebAssembly (wasm32) module"));
rt_println(rt_string_from("  --android    Target Android NDK shared library (.so)"));
rt_println(rt_string_from("  --ios        Target iOS native static bundle (.xcframework)"));
rt_println(rt_string_from("  --check      Run all checks (type + region + closure)"));
rt_println(rt_string_from("  --all        Run all phases with output"));
rt_println(rt_string_from("  --output FILE  Output file for compiled code"));
return;
}
});
rt_string_t filename = ({ rt_string_t _c = (*(rt_string_t*)rt_vec_get(args, 1LL)); rt_string_retain(&_c); _c; });
rt_string_t source = ({
rt_string_t __match_result_1;
NyxResult __match_val_1 = read_to_string(({ rt_string_t _c = filename; rt_string_retain(&_c); _c; }));
if (__match_val_1.tag == RESULT_OK) {
    rt_string_t s = *(rt_string_t*)__match_val_1.data.ptr_val;
__match_result_1 = s;
}
 else if (__match_val_1.tag == RESULT_ERR) {
    rt_string_t e = *(rt_string_t*)__match_val_1.data.ptr_val;
rt_println(rt_string_concat(rt_string_from("Error reading file: "), e));
return;
}
__match_result_1;
});
int show_tokens = rt_vec_contains(args, rt_string_from("--tokens"));
int show_ast = rt_vec_contains(args, rt_string_from("--ast"));
int show_types = rt_vec_contains(args, rt_string_from("--types"));
int show_regions = rt_vec_contains(args, rt_string_from("--regions"));
int show_generics = rt_vec_contains(args, rt_string_from("--generics"));
int show_closures = rt_vec_contains(args, rt_string_from("--closures"));
int show_mlir = rt_vec_contains(args, rt_string_from("--mlir"));
int show_llvm = rt_vec_contains(args, rt_string_from("--llvm"));
int target_wasm = (rt_vec_contains(args, rt_string_from("--wasm")) || rt_vec_contains(args, rt_string_from("--target=wasm32")));
int target_android = (rt_vec_contains(args, rt_string_from("--android")) || rt_vec_contains(args, rt_string_from("--target=android")));
int target_ios = (rt_vec_contains(args, rt_string_from("--ios")) || rt_vec_contains(args, rt_string_from("--target=ios")));
int run_check = rt_vec_contains(args, rt_string_from("--check"));
int show_all = rt_vec_contains(args, rt_string_from("--all"));
NyxOption output_file = rt_vec_get_option(args, rt_string_from("--output"));
Lexer lex = new_lexer(({ rt_string_t _c = source; rt_string_retain(&_c); _c; }));
({ if ((show_tokens || show_all)) {
rt_println(rt_string_from("=== Phase 1: Lexing ==="));
while (1) {
Token tok = Lexer_next_token(&(lex));
print_token((&(tok)));
rt_print(rt_string_from(" "));
({
int64_t __match_result_3;
TokenKind __match_val_3 = tok.kind;
if (__match_val_3.tag == TAG_TokenKind_EOF) {
break;
}
 else {
}
__match_result_3;
});
}
;
rt_println(rt_string_from(""));
}
});
Parser parse = new_parser(({ rt_string_t _c = source; rt_string_retain(&_c); _c; }));
NyxVec program = Parser_parse_program(&(parse));
({ if (parse.had_error) {
rt_println(rt_string_from("Parse errors:"));
for (int64_t _err = 0; _err < parse.errors.len; _err++) {
    rt_string_t err = *(rt_string_t*)rt_vec_get(parse.errors, _err);
rt_println(rt_string_concat(rt_string_from("  - "), err));
}
;
return;
}
});
({ if ((show_ast || show_all)) {
rt_println(rt_string_from("=== Phase 2: AST ==="));
for (int64_t _item = 0; _item < ((NyxVec*)((&(program))))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)((&(program)))), _item);
print_top_level(&(item));
}
;
rt_println(rt_string_from(""));
}
});
TypeChecker checker = TypeChecker_new();
TypeChecker_check_program(&(checker), (&(program)));
({ if (TypeChecker_has_errors(&(checker))) {
rt_println(rt_string_from("Type errors:"));
for (int64_t _err = 0; _err < ((NyxVec*)(TypeChecker_get_errors(&(checker))))->len; _err++) {
    int64_t err = *(int64_t*)rt_vec_get(*((NyxVec*)(TypeChecker_get_errors(&(checker)))), _err);
rt_println(rt_string_concat(rt_string_from("  - "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)err); _s.ref_count = 1; _s; })));
}
;
return;
}
});
({ if (((show_types || show_all) || run_check)) {
rt_println(rt_string_from("=== Phase 3: Type Checking: PASSED ==="));
rt_println(rt_string_from(""));
}
});
RegionAnalyzer region_analyzer = RegionAnalyzer_new();
RegionAnalyzerResult region_result = RegionAnalyzer_analyze(&(region_analyzer), (&(program)));
({ if (((show_regions || show_all) || run_check)) {
rt_println(rt_string_from("=== Phase 4: Region Inference ==="));
for (int64_t _region = 0; _region < ((NyxVec*)((&(region_result.regions))))->len; _region++) {
    RegionInfo region = *(RegionInfo*)rt_vec_get(*((NyxVec*)((&(region_result.regions)))), _region);
rt_println(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("  Region: "), region.name), rt_string_from(" (kind: ")), region.kind), rt_string_from(")")));
({ if (region.has_escapes) {
rt_println(rt_string_from("    Has escaping references - converting to ARC"));
}
});
for (int64_t _var = 0; _var < ((NyxVec*)((&(region.bound_variables))))->len; _var++) {
    VariableInfo var = *(VariableInfo*)rt_vec_get(*((NyxVec*)((&(region.bound_variables)))), _var);
rt_string_t esc_str = ({ rt_string_t __if_res_11; if (var.escapes) {
__if_res_11 = rt_string_from("true");
} else {
__if_res_11 = rt_string_from("false");
}
__if_res_11; });
rt_println(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("    Bound: "), var.name), rt_string_from(" (escapes: ")), esc_str), rt_string_from(")")));
}
;
}
;
rt_println(rt_string_concat(rt_string_from("  Regions analyzed: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)region_result.regions.len); _buf.ref_count = 1; _buf; })));
rt_println(rt_string_concat(rt_string_from("  Escapes detected: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)region_result.escape_count); _buf.ref_count = 1; _buf; })));
rt_println(rt_string_from(""));
}
});
MonomorphizeResult mono_result = Monomorphizer_monomorphize(({ Monomorphizer _stmp_ = Monomorphizer_new(); &_stmp_; }), (&(program)));
({ if ((show_generics || show_all)) {
rt_println(rt_string_from("=== Phase 5: Monomorphization ==="));
for (int64_t _spec = 0; _spec < ((NyxVec*)((&(mono_result.specializations))))->len; _spec++) {
    Specialization spec = *(Specialization*)rt_vec_get(*((NyxVec*)((&(mono_result.specializations)))), _spec);
rt_println(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("  "), spec.original_name), rt_string_from("<")), rt_vec_join(spec.type_args, rt_string_from(", "))), rt_string_from(">")));
rt_println(rt_string_concat(rt_string_from("    -> "), spec.specialized_name));
}
;
rt_println(rt_string_concat(rt_string_from("  Specializations: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)mono_result.specializations.len); _buf.ref_count = 1; _buf; })));
rt_println(rt_string_from(""));
}
});
ClosureConvertResult closure_result = ClosureConverter_convert(({ ClosureConverter _stmp_ = ClosureConverter_new(); &_stmp_; }), (&(program)));
({ if (((show_closures || show_all) || run_check)) {
rt_println(rt_string_from("=== Phase 6: Closure Conversion ==="));
for (int64_t _closure = 0; _closure < ((NyxVec*)((&(closure_result.closures))))->len; _closure++) {
    ClosureInfo closure = *(ClosureInfo*)rt_vec_get(*((NyxVec*)((&(closure_result.closures)))), _closure);
rt_println(rt_string_concat(rt_string_from("  Closure: "), closure.name));
rt_println(rt_string_concat(rt_string_from("    Env struct: "), closure.env_struct_name));
rt_println(rt_string_concat(rt_string_concat(rt_string_from("    Captures: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)closure.captured_vars.len); _buf.ref_count = 1; _buf; })), rt_string_from(" variables")));
for (int64_t _cap = 0; _cap < ((NyxVec*)((&(closure.captured_vars))))->len; _cap++) {
    CapturedVar cap = *(CapturedVar*)rt_vec_get(*((NyxVec*)((&(closure.captured_vars)))), _cap);
rt_string_t heap_str = ({ rt_string_t __if_res_14; if (cap.requires_heap) {
__if_res_14 = rt_string_from("true");
} else {
__if_res_14 = rt_string_from("false");
}
__if_res_14; });
rt_println(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("      - "), cap.name), rt_string_from(" (heap: ")), heap_str), rt_string_from(")")));
}
;
}
;
rt_println(rt_string_concat(rt_string_from("  Closures converted: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)closure_result.closures.len); _buf.ref_count = 1; _buf; })));
rt_println(rt_string_from(""));
}
});
({ if ((show_mlir || show_all)) {
rt_println(rt_string_from("=== Phase 7: MLIR Code Generation ==="));
rt_string_t mlir_code = generate_mlir((&(program)));
rt_println(mlir_code);
rt_println(rt_string_from(""));
}
});
({ if ((target_wasm || show_all)) {
rt_println(rt_string_from("=== Phase 8: WebAssembly WASM Target ==="));
rt_println(rt_string_from("  Target Architecture: wasm32-unknown-unknown"));
rt_println(rt_string_from("  Web Runtime: web-runtime/nyx_web_runtime.js"));
rt_println(rt_string_from("  Canvas Engine: HTML5 2D / WebGL Material 3"));
rt_println(rt_string_from(""));
}
});
({ if ((target_android || show_all)) {
rt_println(rt_string_from("=== Phase 9: Android NDK Native Target ==="));
rt_println(rt_string_from("  Target Architecture: aarch64-linux-android"));
rt_println(rt_string_from("  Native Runtime: runtime/rt_android.c"));
rt_println(rt_string_from("  NDK Package: libnyx_app.so"));
rt_println(rt_string_from(""));
}
});
({ if ((target_ios || show_all)) {
rt_println(rt_string_from("=== Phase 10: iOS Native Target ==="));
rt_println(rt_string_from("  Target Architecture: arm64-apple-ios"));
rt_println(rt_string_from("  Native Runtime: runtime/rt_ios.m"));
rt_println(rt_string_from("  Framework Bundle: NyxApp.xcframework"));
rt_println(rt_string_from(""));
}
});
({ if ((show_llvm || show_all)) {
rt_println(rt_string_from("=== Phase 11: Direct LLVM IR Code Generation ==="));
rt_string_t llvm_ir = generate_llvm((&(program)));
rt_println(llvm_ir);
rt_println(rt_string_from(""));
}
});
({ if (({ NyxOption _o = output_file; _o.tag == OPTION_SOME; })) {
rt_string_t path = rt_string_from((char*)({ NyxOption _u = output_file; _u.data.ptr_val; }));
rt_string_t mlir_code = generate_mlir((&(program)));
({
int64_t __match_result_20;
NyxResult __match_val_20 = write_string(path, mlir_code);
if (__match_val_20.tag == RESULT_OK) {
rt_println(rt_string_concat(rt_string_from("Output written to: "), path));
}
 else if (__match_val_20.tag == RESULT_ERR) {
    rt_string_t e = *(rt_string_t*)__match_val_20.data.ptr_val;
rt_println(rt_string_concat(rt_string_from("Error writing output: "), e));
}
__match_result_20;
});
}
});
rt_println(rt_string_concat(rt_string_from("Compilation successful: "), filename));
}

void print_token(Token* tok) {
({
int64_t __match_result_21;
TokenKind* __match_val_21 = (&((*tok).kind));
if ((*__match_val_21).tag == TAG_TokenKind_Ident) {
    rt_string_t s = (*__match_val_21).data.Ident.f0;
rt_print(rt_string_concat(rt_string_concat(rt_string_from("Ident("), s), rt_string_from(")")));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_IntLit) {
    int64_t n = (*__match_val_21).data.IntLit.f0;
rt_print(rt_string_concat(rt_string_concat(rt_string_from("Int("), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)n); _buf.ref_count = 1; _buf; })), rt_string_from(")")));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_FloatLit) {
    double f = (*__match_val_21).data.FloatLit.f0;
rt_print(rt_string_from("Float"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_StringLit) {
    rt_string_t s = (*__match_val_21).data.StringLit.f0;
rt_print(rt_string_concat(rt_string_concat(rt_string_from("String(\\\""), s), rt_string_from("\\\")")));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Fn) {
rt_print(rt_string_from("Fn"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Let) {
rt_print(rt_string_from("Let"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Mut) {
rt_print(rt_string_from("Mut"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_If) {
rt_print(rt_string_from("If"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Else) {
rt_print(rt_string_from("Else"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_While) {
rt_print(rt_string_from("While"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_For) {
rt_print(rt_string_from("For"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Return) {
rt_print(rt_string_from("Return"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Struct) {
rt_print(rt_string_from("Struct"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Enum) {
rt_print(rt_string_from("Enum"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Match) {
rt_print(rt_string_from("Match"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Async) {
rt_print(rt_string_from("Async"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Await) {
rt_print(rt_string_from("Await"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Import) {
rt_print(rt_string_from("Import"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Module) {
rt_print(rt_string_from("Module"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Pub) {
rt_print(rt_string_from("Pub"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Plus) {
rt_print(rt_string_from("+"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Minus) {
rt_print(rt_string_from("-"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Star) {
rt_print(rt_string_from("*"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Slash) {
rt_print(rt_string_from("/"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Equals) {
rt_print(rt_string_from("="));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_EqEq) {
rt_print(rt_string_from("=="));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_NotEq) {
rt_print(rt_string_from("!="));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Lt) {
rt_print(rt_string_from("<"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Gt) {
rt_print(rt_string_from(">"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_LtEq) {
rt_print(rt_string_from("<="));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_GtEq) {
rt_print(rt_string_from(">="));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_And) {
rt_print(rt_string_from("&&"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Or) {
rt_print(rt_string_from("||"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Not) {
rt_print(rt_string_from("!"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Arrow) {
rt_print(rt_string_from("->"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_FatArrow) {
rt_print(rt_string_from("=>"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_LParen) {
rt_print(rt_string_from("("));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_RParen) {
rt_print(rt_string_from(")"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_LBrace) {
rt_print(rt_string_from("{"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_RBrace) {
rt_print(rt_string_from("}"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_LBracket) {
rt_print(rt_string_from("["));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_RBracket) {
rt_print(rt_string_from("]"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Comma) {
rt_print(rt_string_from(","));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Semicolon) {
rt_print(rt_string_from(";"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Colon) {
rt_print(rt_string_from(":"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Dot) {
rt_print(rt_string_from("."));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_True) {
rt_print(rt_string_from("true"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_False) {
rt_print(rt_string_from("false"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_Null) {
rt_print(rt_string_from("null"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_SelfKw) {
rt_print(rt_string_from("self"));
}
 else if ((*__match_val_21).tag == TAG_TokenKind_EOF) {
rt_print(rt_string_from("EOF"));
}
 else {
rt_print(rt_string_from("Token"));
}
__match_result_21;
});
}

void print_top_level(TopLevel* item) {
({
int64_t __match_result_22;
TopLevel* __match_val_22 = item;
if ((*__match_val_22).tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = (*__match_val_22).data.FnDecl.f0;
    NyxVec generics = (*__match_val_22).data.FnDecl.f1;
    NyxVec params = (*__match_val_22).data.FnDecl.f2;
    Type ret_type = (*__match_val_22).data.FnDecl.f3;
    int is_async = (*__match_val_22).data.FnDecl.f5;
rt_string_t async_str = ({ rt_string_t __if_res_24; if (is_async) {
__if_res_24 = rt_string_from("async ");
} else {
__if_res_24 = rt_string_from("");
}
__if_res_24; });
rt_print(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("  "), async_str), rt_string_from("fn ")), name));
({ if ((generics.len > 0LL)) {
rt_print(rt_string_from("<"));
for (int64_t i = 0LL; i < generics.len; i++) {
({ if ((i > 0LL)) {
rt_print(rt_string_from(", "));
}
});
rt_print((*(rt_string_t*)rt_vec_get(generics, i)));
}
;
rt_print(rt_string_from(">"));
}
});
rt_print(rt_string_from("("));
for (int64_t i = 0LL; i < params.len; i++) {
({ if ((i > 0LL)) {
rt_print(rt_string_from(", "));
}
});
rt_print(rt_string_concat(((*(Param*)rt_vec_get(params, i))).name, rt_string_from(": ")));
print_type((&(((*(Param*)rt_vec_get(params, i))).ty)));
}
;
rt_print(rt_string_from(") -> "));
print_type(&(ret_type));
rt_println(rt_string_from(""));
}
 else if ((*__match_val_22).tag == TAG_TopLevel_StructDef) {
    rt_string_t name = (*__match_val_22).data.StructDef.f0;
    NyxVec generics = (*__match_val_22).data.StructDef.f1;
rt_print(rt_string_concat(rt_string_from("  struct "), name));
({ if ((generics.len > 0LL)) {
rt_print(rt_string_from("<"));
for (int64_t i = 0LL; i < generics.len; i++) {
({ if ((i > 0LL)) {
rt_print(rt_string_from(", "));
}
});
rt_print((*(rt_string_t*)rt_vec_get(generics, i)));
}
;
rt_print(rt_string_from(">"));
}
});
rt_println(rt_string_from(""));
}
 else if ((*__match_val_22).tag == TAG_TopLevel_EnumDef) {
    rt_string_t name = (*__match_val_22).data.EnumDef.f0;
    NyxVec generics = (*__match_val_22).data.EnumDef.f1;
rt_print(rt_string_concat(rt_string_from("  enum "), name));
({ if ((generics.len > 0LL)) {
rt_print(rt_string_from("<"));
for (int64_t i = 0LL; i < generics.len; i++) {
({ if ((i > 0LL)) {
rt_print(rt_string_from(", "));
}
});
rt_print((*(rt_string_t*)rt_vec_get(generics, i)));
}
;
rt_print(rt_string_from(">"));
}
});
rt_println(rt_string_from(""));
}
 else if ((*__match_val_22).tag == TAG_TopLevel_ImplBlock) {
    rt_string_t type_name = (*__match_val_22).data.ImplBlock.f0;
    NyxVec generics = (*__match_val_22).data.ImplBlock.f1;
rt_print(rt_string_concat(rt_string_from("  impl "), type_name));
({ if ((generics.len > 0LL)) {
rt_print(rt_string_from("<"));
for (int64_t i = 0LL; i < generics.len; i++) {
({ if ((i > 0LL)) {
rt_print(rt_string_from(", "));
}
});
rt_print((*(rt_string_t*)rt_vec_get(generics, i)));
}
;
rt_print(rt_string_from(">"));
}
});
rt_println(rt_string_from(" {"));
rt_println(rt_string_from("    ..."));
rt_println(rt_string_from("  }"));
}
 else if ((*__match_val_22).tag == TAG_TopLevel_TraitDef) {
    rt_string_t name = (*__match_val_22).data.TraitDef.f0;
rt_println(rt_string_concat(rt_string_from("  trait "), name));
}
 else if ((*__match_val_22).tag == TAG_TopLevel_TypeAlias) {
    rt_string_t name = (*__match_val_22).data.TypeAlias.f0;
    Type target = (*__match_val_22).data.TypeAlias.f1;
rt_println(rt_string_concat(rt_string_concat(rt_string_from("  type "), name), rt_string_from(" = ...")));
}
 else if ((*__match_val_22).tag == TAG_TopLevel_ConstDecl) {
    rt_string_t name = (*__match_val_22).data.ConstDecl.f0;
    Type ty = (*__match_val_22).data.ConstDecl.f1;
rt_print(rt_string_concat(rt_string_concat(rt_string_from("  const "), name), rt_string_from(": ")));
print_type(&(ty));
rt_println(rt_string_from(""));
}
 else if ((*__match_val_22).tag == TAG_TopLevel_ModuleDecl) {
    rt_string_t name = (*__match_val_22).data.ModuleDecl.f0;
rt_println(rt_string_concat(rt_string_from("  module "), name));
}
 else if ((*__match_val_22).tag == TAG_TopLevel_UseDecl) {
    NyxVec path = (*__match_val_22).data.UseDecl.f0;
rt_string_t path_str = rt_string_from("");
for (int64_t i = 0LL; i < path.len; i++) {
({ if ((i > 0LL)) {
path_str = rt_string_concat(path_str, rt_string_from("."));
}
});
path_str = rt_string_concat(path_str, (*(rt_string_t*)rt_vec_get(path, i)));
}
;
rt_println(rt_string_concat(rt_string_from("  use "), path_str));
}
 else {
rt_println(rt_string_from("  TopLevel(...)"));
}
__match_result_22;
});
}

void print_type(Type* ty) {
({
int64_t __match_result_34;
Type* __match_val_34 = ty;
if ((*__match_val_34).tag == TAG_Type_Int) {
rt_print(rt_string_from("Int"));
}
 else if ((*__match_val_34).tag == TAG_Type_Float) {
rt_print(rt_string_from("Float"));
}
 else if ((*__match_val_34).tag == TAG_Type_Bool) {
rt_print(rt_string_from("Bool"));
}
 else if ((*__match_val_34).tag == TAG_Type_String) {
rt_print(rt_string_from("String"));
}
 else if ((*__match_val_34).tag == TAG_Type_Char) {
rt_print(rt_string_from("Char"));
}
 else if ((*__match_val_34).tag == TAG_Type_Void) {
rt_print(rt_string_from("Void"));
}
 else if ((*__match_val_34).tag == TAG_Type_Custom) {
    rt_string_t name = (*__match_val_34).data.Custom.f0;
rt_print(name);
}
 else if ((*__match_val_34).tag == TAG_Type_Generic) {
    rt_string_t name = (*__match_val_34).data.Generic.f0;
rt_print(name);
}
 else if ((*__match_val_34).tag == TAG_Type_Array) {
    Type* inner = (*__match_val_34).data.Array.f0;
rt_print(rt_string_from("["));
print_type(inner);
rt_print(rt_string_from("]"));
}
 else if ((*__match_val_34).tag == TAG_Type_Ref) {
    Type* inner = (*__match_val_34).data.Ref.f0;
rt_print(rt_string_from("&"));
print_type(inner);
}
 else if ((*__match_val_34).tag == TAG_Type_MutRef) {
    Type* inner = (*__match_val_34).data.MutRef.f0;
rt_print(rt_string_from("&mut "));
print_type(inner);
}
 else if ((*__match_val_34).tag == TAG_Type_Option) {
    Type* inner = (*__match_val_34).data.Option.f0;
rt_print(rt_string_from("Option<"));
print_type(inner);
rt_print(rt_string_from(">"));
}
 else if ((*__match_val_34).tag == TAG_Type_Result) {
    Type* inner = (*__match_val_34).data.Result.f0;
    Type* err = (*__match_val_34).data.Result.f1;
rt_print(rt_string_from("Result<"));
print_type(inner);
rt_print(rt_string_from(", "));
print_type(err);
rt_print(rt_string_from(">"));
}
 else if ((*__match_val_34).tag == TAG_Type_Tuple) {
    NyxVec types = (*__match_val_34).data.Tuple.f0;
rt_print(rt_string_from("("));
for (int64_t i = 0LL; i < types.len; i++) {
({ if ((i > 0LL)) {
rt_print(rt_string_from(", "));
}
});
print_type((&((*(Type*)rt_vec_get(types, i)))));
}
;
rt_print(rt_string_from(")"));
}
 else if ((*__match_val_34).tag == TAG_Type_Func) {
    NyxVec params = (*__match_val_34).data.Func.f0;
    Type* ret = (*__match_val_34).data.Func.f1;
rt_print(rt_string_from("fn("));
for (int64_t i = 0LL; i < params.len; i++) {
({ if ((i > 0LL)) {
rt_print(rt_string_from(", "));
}
});
print_type((&((*(Type*)rt_vec_get(params, i)))));
}
;
rt_print(rt_string_from(") -> "));
print_type(ret);
}
 else if ((*__match_val_34).tag == TAG_Type_Region) {
    rt_string_t name = (*__match_val_34).data.Region.f0;
rt_print(rt_string_from("'"));
rt_print(name);
}
 else if ((*__match_val_34).tag == TAG_Type_Recursive) {
    rt_string_t name = (*__match_val_34).data.Recursive.f0;
rt_print(rt_string_from("rec "));
rt_print(name);
}
 else {
rt_print(rt_string_from("Type"));
}
__match_result_34;
});
}

RegionAnalyzer RegionAnalyzer_new() {
return (RegionAnalyzer){};
}

RegionAnalyzerResult RegionAnalyzer_analyze(RegionAnalyzer* self, NyxVec* program) {
NyxVec regions = rt_vec_new(sizeof(int64_t));
int64_t escape_count = 0LL;
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
({
int64_t __match_result_37;
TopLevel __match_val_37 = item;
if (__match_val_37.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_37.data.FnDecl.f0;
RegionInfo region = (RegionInfo){.name = rt_string_concat(({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), rt_string_from("_region")), .kind = rt_string_from("function"), .has_escapes = 0, .bound_variables = rt_vec_new(sizeof(int64_t))};
rt_vec_push(&regions, ({ RegionInfo _tmp_38 = region; &_tmp_38; }));
}
 else {
}
__match_result_37;
});
}
;
return (RegionAnalyzerResult){.regions = regions, .escape_count = escape_count};
}

Monomorphizer Monomorphizer_new() {
return (Monomorphizer){};
}

MonomorphizeResult Monomorphizer_monomorphize(Monomorphizer* self, NyxVec* program) {
NyxVec specializations = rt_vec_new(sizeof(int64_t));
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
({
int64_t __match_result_39;
TopLevel __match_val_39 = item;
if (__match_val_39.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_39.data.FnDecl.f0;
    NyxVec generics = __match_val_39.data.FnDecl.f1;
({ if ((generics.len > 0LL)) {
rt_vec_push(&specializations, ({ Specialization _tmp_41 = (Specialization){.original_name = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), .specialized_name = rt_string_concat(({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), rt_string_from("_i32")), .type_args = ({ NyxVec _v = rt_vec_new(sizeof(rt_string_t)); rt_string_t _ve42_0 = rt_string_from("i32"); rt_vec_push(&_v, &_ve42_0); _v; })}; &_tmp_41; }));
}
});
}
 else {
}
__match_result_39;
});
}
;
return (MonomorphizeResult){.specializations = specializations};
}

ClosureConverter ClosureConverter_new() {
return (ClosureConverter){};
}

ClosureConvertResult ClosureConverter_convert(ClosureConverter* self, NyxVec* program) {
NyxVec closures = rt_vec_new(sizeof(int64_t));
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
({
int64_t __match_result_42;
TopLevel __match_val_42 = item;
if (__match_val_42.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_42.data.FnDecl.f0;
    NyxVec params = __match_val_42.data.FnDecl.f2;
for (int64_t _param = 0; _param < params.len; _param++) {
    Param param = *(Param*)rt_vec_get(params, _param);
({
int64_t __match_result_43;
Type* __match_val_43 = (&(param.ty));
if ((*__match_val_43).tag == TAG_Type_Func) {
rt_vec_push(&closures, ({ ClosureInfo _tmp_44 = (ClosureInfo){.name = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), .env_struct_name = rt_string_concat(rt_string_from("Env_"), name), .captured_vars = rt_vec_new(sizeof(int64_t))}; &_tmp_44; }));
}
 else {
}
__match_result_43;
});
}
;
}
 else {
}
__match_result_42;
});
}
;
return (ClosureConvertResult){.closures = closures};
}


int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
