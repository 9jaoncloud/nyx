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

#define TAG_OrderDirection_Asc 0
#define TAG_OrderDirection_Desc 1
typedef struct {
    int tag;
    union {
    } data;
} OrderDirection;

#define TAG_DbEngine_SQLite 0
#define TAG_DbEngine_MySQL 1
#define TAG_DbEngine_MSSQL 2
#define TAG_DbEngine_PostgreSQL 3
typedef struct {
    int tag;
    union {
    } data;
} DbEngine;

// runtime struct Database

typedef struct {
    Stmt handle;
    DB db;
} Statement;

typedef struct {
    NyxMap values;
} Row;

typedef struct {
    int64_t version;
    rt_string_t name;
    rt_string_t sql;
} Migration;

typedef struct {
    int64_t applied_count;
} MigrationRunner;

typedef struct {
    DbEngine engine;
    rt_string_t host;
    int64_t port;
    rt_string_t user;
    rt_string_t pass;
    rt_string_t dbname;
} DbConfig;

typedef struct {
    rt_string_t column;
    rt_string_t operator;
    rt_string_t value;
    int is_or;
} WhereClause;

typedef struct {
    rt_string_t table_name;
    NyxVec select_cols;
    NyxVec where_clauses;
    rt_string_t order_col;
    OrderDirection order_dir;
    int64_t limit_count;
    int64_t offset_count;
} QueryBuilder;

NyxOption Row_get_string(Row*, rt_string_t);
NyxOption Row_get_int(Row*, rt_string_t);
NyxOption Row_get_float(Row*, rt_string_t);
NyxResult open(rt_string_t);
NyxResult open_memory();
NyxResult Database_exec(Database*, rt_string_t);
NyxResult Database_prepare(Database*, rt_string_t);
NyxResult Database_query(Database*, rt_string_t);
NyxResult Database_query_single(Database*, rt_string_t);
NyxResult Database_execute(Database*, rt_string_t);
void Database_close(Database*);
rt_string_t Database_last_error(Database*);
NyxResult Statement_step(Statement*);
NyxResult Statement_current_row(Statement*);
NyxResult Statement_bind_string(Statement*, int64_t, rt_string_t);
NyxResult Statement_bind_int(Statement*, int64_t, int64_t);
NyxResult Statement_bind_float(Statement*, int64_t, double);
NyxResult Statement_bind_null(Statement*, int64_t);
void Statement_close(Statement*);
MigrationRunner MigrationRunner_new();
int MigrationRunner_apply(MigrationRunner*, int64_t, rt_string_t, rt_string_t);
DbConfig DbConfig_mysql(rt_string_t, int64_t, rt_string_t, rt_string_t, rt_string_t);
DbConfig DbConfig_mssql(rt_string_t, int64_t, rt_string_t, rt_string_t, rt_string_t);
rt_string_t OrderDirection_to_sql(OrderDirection*);
QueryBuilder QueryBuilder_table(rt_string_t);
QueryBuilder QueryBuilder_select(QueryBuilder*, NyxVec);
QueryBuilder QueryBuilder_where_eq(QueryBuilder*, rt_string_t, rt_string_t);
QueryBuilder QueryBuilder_where_clause(QueryBuilder*, rt_string_t, rt_string_t, rt_string_t);
QueryBuilder QueryBuilder_or_where(QueryBuilder*, rt_string_t, rt_string_t, rt_string_t);
QueryBuilder QueryBuilder_order_by(QueryBuilder*, rt_string_t, OrderDirection);
QueryBuilder QueryBuilder_limit(QueryBuilder*, int64_t);
QueryBuilder QueryBuilder_offset(QueryBuilder*, int64_t);
QueryBuilder QueryBuilder_paginate(QueryBuilder*, int64_t, int64_t);
rt_string_t QueryBuilder_to_sql(QueryBuilder*);
rt_string_t QueryBuilder_count_sql(QueryBuilder*, rt_string_t);
rt_string_t QueryBuilder_to_delete_sql(QueryBuilder*);
NyxResult QueryBuilder_get(QueryBuilder*, Database*);
NyxResult QueryBuilder_first(QueryBuilder*, Database*);
// module db
extern int64_t sqlite3_open(int64_t* p0, int64_t** p1);
extern int64_t sqlite3_close(int64_t* p0);
extern int64_t sqlite3_exec(int64_t* p0, int64_t* p1, int64_t* p2, int64_t* p3);
extern int64_t* sqlite3_errmsg(int64_t* p0);
extern int64_t sqlite3_prepare_v2(int64_t* p0, int64_t* p1, int64_t p2, int64_t** p3, int64_t** p4);
extern int64_t sqlite3_step(int64_t* p0);
extern void sqlite3_finalize(int64_t* p0);
extern int64_t sqlite3_column_count(int64_t* p0);
extern int64_t* sqlite3_column_name(int64_t* p0, int64_t p1);
extern int64_t* sqlite3_column_text(int64_t* p0, int64_t p1);
extern int64_t sqlite3_column_int(int64_t* p0, int64_t p1);
extern double sqlite3_column_double(int64_t* p0, int64_t p1);
extern int64_t sqlite3_bind_text(int64_t* p0, int64_t p1, int64_t* p2, int64_t p3, int64_t* p4);
extern int64_t sqlite3_bind_int(int64_t* p0, int64_t p1, int64_t p2);
extern int64_t sqlite3_bind_double(int64_t* p0, int64_t p1, double p2);
extern int64_t sqlite3_bind_null(int64_t* p0, int64_t p1);
NyxOption Row_get_string(Row* self, rt_string_t col) {
return rt_map_get(&((*self).values), col);
}

NyxOption Row_get_int(Row* self, rt_string_t col) {
return ({
NyxOption __match_result_0;
NyxOption __match_val_0 = rt_map_get(&((*self).values), col);
if (__match_val_0.tag == OPTION_SOME) {
    void* v = __match_val_0.data.ptr_val;
__match_result_0 = rt_string_parse_int(v);
}
 else if (__match_val_0.tag == OPTION_NONE) {
__match_result_0 = rt_option_none();
}
__match_result_0;
});
}

NyxOption Row_get_float(Row* self, rt_string_t col) {
return ({
NyxOption __match_result_1;
NyxOption __match_val_1 = rt_map_get(&((*self).values), col);
if (__match_val_1.tag == OPTION_SOME) {
    void* v = __match_val_1.data.ptr_val;
__match_result_1 = rt_string_parse_float(v);
}
 else if (__match_val_1.tag == OPTION_NONE) {
__match_result_1 = rt_option_none();
}
__match_result_1;
});
}

NyxResult open(rt_string_t path) {
Arena* __arena = arena_create();
DB db = 0LL;
int64_t rc = sqlite3_open(((const uint8_t*)(path.data)), (&(db)));
arena_destroy(__arena);
if ((rc == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Database _stmp_ = (Database){.handle = db}; &_stmp_; }); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot open database: "), path); _stmp_.data; }); _tmp; });
}
}

NyxResult open_memory() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return open(rt_string_from(":memory:"));
}

NyxResult Database_exec(Database* self, rt_string_t sql) {
int64_t rc = sqlite3_exec((*self).handle, ((const uint8_t*)(sql.data)), 0LL, 0LL);
if ((rc == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
} else {
rt_string_t msg = uint8_t_to_string(sqlite3_errmsg((*self).handle));
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("SQL error: "), msg); _stmp_.data; }); _tmp; });
}
}

NyxResult Database_prepare(Database* self, rt_string_t sql) {
Stmt stmt = 0LL;
uint8_t* tail = 0LL;
int64_t rc = sqlite3_prepare_v2((*self).handle, ((const uint8_t*)(sql.data)), (-1LL), (&(stmt)), (&(tail)));
if ((rc == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Statement _stmp_ = (Statement){.handle = stmt, .db = (*self).handle}; &_stmp_; }); _tmp; });
} else {
rt_string_t msg = uint8_t_to_string(sqlite3_errmsg((*self).handle));
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Prepare error: "), msg); _stmp_.data; }); _tmp; });
}
}

NyxResult Database_query(Database* self, rt_string_t sql) {
Statement stmt = ({ NyxResult _qres = Database_prepare(self, sql); if (_qres.tag == RESULT_ERR) { return _qres; } *(Statement*)_qres.data.ptr_val; });
NyxVec rows = rt_vec_new(sizeof(int64_t));
int current = ({ NyxResult _qres = Statement_step(&(stmt)); if (_qres.tag == RESULT_ERR) { return _qres; } (int)_qres.data.int_val; });
while (current) {
rt_vec_push(&rows, ({ Row _tmp_2 = (int64_t)(({ NyxResult _qres = Statement_current_row(&(stmt)); if (_qres.tag == RESULT_ERR) { return _qres; } *(Row*)_qres.data.ptr_val; })); &_tmp_2; }));
current = ({ NyxResult _qres = Statement_step(&(stmt)); if (_qres.tag == RESULT_ERR) { return _qres; } (int)_qres.data.int_val; });
}
;
Statement_close(&(stmt));
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = rows; &_stmp_; }); _tmp; });
}

NyxResult Database_query_single(Database* self, rt_string_t sql) {
Statement stmt = ({ NyxResult _qres = Database_prepare(self, sql); if (_qres.tag == RESULT_ERR) { return _qres; } *(Statement*)_qres.data.ptr_val; });
return ({
NyxResult __match_result_3;
int __match_val_3 = ({ NyxResult _qres = Statement_step(&(stmt)); if (_qres.tag == RESULT_ERR) { return _qres; } (int)_qres.data.int_val; });
if (__match_val_3 == 1) {
Row row = ({ NyxResult _qres = Statement_current_row(&(stmt)); if (_qres.tag == RESULT_ERR) { return _qres; } *(Row*)_qres.data.ptr_val; });
Statement_close(&(stmt));
__match_result_3 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxOption _stmp_ = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ Row _stmp_ = row; &_stmp_; }); _tmp; }); &_stmp_; }); _tmp; });
}
 else if (__match_val_3 == 0) {
Statement_close(&(stmt));
__match_result_3 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Option _stmp_ = rt_option_none(); &_stmp_; }); _tmp; });
}
__match_result_3;
});
}

NyxResult Database_execute(Database* self, rt_string_t sql) {
Stmt stmt = 0LL;
uint8_t* tail = 0LL;
int64_t rc = sqlite3_prepare_v2((*self).handle, ((const uint8_t*)(sql.data)), (-1LL), (&(stmt)), (&(tail)));
({ if ((rc != 0LL)) {
rt_string_t msg = uint8_t_to_string(sqlite3_errmsg((*self).handle));
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Prepare error: "), msg); _stmp_.data; }); _tmp; });
}
});
sqlite3_step(stmt);
sqlite3_finalize(stmt);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
}

void Database_close(Database* self) {
sqlite3_close((*self).handle);
}

rt_string_t Database_last_error(Database* self) {
return uint8_t_to_string(sqlite3_errmsg((*self).handle));
}

NyxResult Statement_step(Statement* self) {
int64_t rc = sqlite3_step((*self).handle);
if ((rc == SQLITE_ROW)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (void*)(1); _tmp; });
} else if ((rc == SQLITE_DONE)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (void*)(0); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Step error"); _stmp_.data; }); _tmp; });
}
}

NyxResult Statement_current_row(Statement* self) {
int64_t count = sqlite3_column_count((*self).handle);
NyxMap values = rt_map_new();
for (int64_t i = 0LL; i < count; i++) {
rt_string_t name = uint8_t_to_string(sqlite3_column_name((*self).handle, i));
uint8_t* text = sqlite3_column_text((*self).handle, i);
rt_string_t value = ({ rt_string_t __if_res_6; if ((text != 0LL)) {
__if_res_6 = uint8_t_to_string(text);
} else {
__if_res_6 = rt_string_from("NULL");
}
__if_res_6; });
rt_map_insert(&(values), name, (int64_t)(({ rt_string_t* _mtmp = malloc(sizeof(rt_string_t)); *_mtmp = value; (int64_t)_mtmp; })));
}
;
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Row _stmp_ = (Row){.values = values}; &_stmp_; }); _tmp; });
}

NyxResult Statement_bind_string(Statement* self, int64_t idx, rt_string_t val) {
int64_t rc = sqlite3_bind_text((*self).handle, idx, ((const uint8_t*)(val.data)), (int64_t)(val.length), 0LL);
if ((rc == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Bind error"); _stmp_.data; }); _tmp; });
}
}

NyxResult Statement_bind_int(Statement* self, int64_t idx, int64_t val) {
int64_t rc = sqlite3_bind_int((*self).handle, idx, val);
if ((rc == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Bind error"); _stmp_.data; }); _tmp; });
}
}

NyxResult Statement_bind_float(Statement* self, int64_t idx, double val) {
int64_t rc = sqlite3_bind_double((*self).handle, idx, val);
if ((rc == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Bind error"); _stmp_.data; }); _tmp; });
}
}

NyxResult Statement_bind_null(Statement* self, int64_t idx) {
int64_t rc = sqlite3_bind_null((*self).handle, idx);
if ((rc == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Bind error"); _stmp_.data; }); _tmp; });
}
}

void Statement_close(Statement* self) {
sqlite3_finalize((*self).handle);
}

MigrationRunner MigrationRunner_new() {
return (MigrationRunner){.applied_count = 0LL};
}

int MigrationRunner_apply(MigrationRunner* self, int64_t version, rt_string_t name, rt_string_t sql) {
(self->applied_count = ((*self).applied_count + 1LL));
return 1;
}

DbConfig DbConfig_mysql(rt_string_t host, int64_t port, rt_string_t user, rt_string_t pass, rt_string_t dbname) {
return (DbConfig){.engine = (DbEngine){ .tag = TAG_DbEngine_MySQL }, .host = host, .port = port, .user = user, .pass = pass, .dbname = dbname};
}

DbConfig DbConfig_mssql(rt_string_t host, int64_t port, rt_string_t user, rt_string_t pass, rt_string_t dbname) {
return (DbConfig){.engine = (DbEngine){ .tag = TAG_DbEngine_MSSQL }, .host = host, .port = port, .user = user, .pass = pass, .dbname = dbname};
}

rt_string_t OrderDirection_to_sql(OrderDirection* self) {
return ({
rt_string_t __match_result_6;
void* __match_val_6 = self;
if ((*__match_val_6).tag == TAG_OrderDirection_Asc) {
__match_result_6 = rt_string_from("ASC");
}
 else if ((*__match_val_6).tag == TAG_OrderDirection_Desc) {
__match_result_6 = rt_string_from("DESC");
}
__match_result_6;
});
}

QueryBuilder QueryBuilder_table(rt_string_t name) {
return (QueryBuilder){.table_name = name, .select_cols = rt_vec_new(sizeof(int64_t)), .where_clauses = rt_vec_new(sizeof(int64_t)), .order_col = rt_string_from(""), .order_dir = (OrderDirection){ .tag = TAG_OrderDirection_Asc }, .limit_count = (-1LL), .offset_count = 0LL};
}

QueryBuilder QueryBuilder_select(QueryBuilder* self, NyxVec cols) {
(self->select_cols = cols);
return self;
}

QueryBuilder QueryBuilder_where_eq(QueryBuilder* self, rt_string_t col, rt_string_t val) {
rt_vec_push(&(*self).where_clauses, ({ WhereClause _tmp_7 = (int64_t)((WhereClause){.column = col, .operator = rt_string_from("="), .value = val, .is_or = 0}); &_tmp_7; }));
return self;
}

QueryBuilder QueryBuilder_where_clause(QueryBuilder* self, rt_string_t col, rt_string_t op, rt_string_t val) {
rt_vec_push(&(*self).where_clauses, ({ WhereClause _tmp_8 = (int64_t)((WhereClause){.column = col, .operator = op, .value = val, .is_or = 0}); &_tmp_8; }));
return self;
}

QueryBuilder QueryBuilder_or_where(QueryBuilder* self, rt_string_t col, rt_string_t op, rt_string_t val) {
rt_vec_push(&(*self).where_clauses, ({ WhereClause _tmp_9 = (int64_t)((WhereClause){.column = col, .operator = op, .value = val, .is_or = 1}); &_tmp_9; }));
return self;
}

QueryBuilder QueryBuilder_order_by(QueryBuilder* self, rt_string_t col, OrderDirection dir) {
(self->order_col = col);
(self->order_dir = dir);
return self;
}

QueryBuilder QueryBuilder_limit(QueryBuilder* self, int64_t count) {
(self->limit_count = count);
return self;
}

QueryBuilder QueryBuilder_offset(QueryBuilder* self, int64_t count) {
(self->offset_count = count);
return self;
}

QueryBuilder QueryBuilder_paginate(QueryBuilder* self, int64_t page, int64_t per_page) {
int64_t p = ({ int64_t __if_res_11; if ((page < 1LL)) {
__if_res_11 = 1LL;
} else {
__if_res_11 = page;
}
__if_res_11; });
int64_t offset = ((p - 1LL) * per_page);
(self->limit_count = per_page);
(self->offset_count = offset);
return self;
}

rt_string_t QueryBuilder_to_sql(QueryBuilder* self) {
rt_string_t cols_sql = ({ rt_string_t __if_res_12; if (((*self).select_cols.len == 0LL)) {
__if_res_12 = rt_string_from("*");
} else {
rt_string_t s = rt_string_from("");
int64_t i = 0LL;
while ((i < (*self).select_cols.len)) {
({ if ((i > 0LL)) {
s = rt_string_concat(s, rt_string_from(", "));
}
});
s = rt_string_concat(s, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)(*(int64_t*)rt_vec_get((*self).select_cols, i))); _s.ref_count = 1; _s; }));
i = (i + 1LL);
}
;
__if_res_12 = s;
}
__if_res_12; });
rt_string_t sql = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("SELECT "), cols_sql), rt_string_from(" FROM ")), (*self).table_name);
({ if (((*self).where_clauses.len > 0LL)) {
sql = rt_string_concat(sql, rt_string_from(" WHERE "));
int64_t i = 0LL;
while ((i < (*self).where_clauses.len)) {
int64_t w = (&((*(int64_t*)rt_vec_get((*self).where_clauses, i))));
({ if ((i > 0LL)) {
({ rt_string_t __if_res_16; if (w.is_or) {
__if_res_16 = (sql = rt_string_concat(sql, rt_string_from(" OR ")));
} else {
__if_res_16 = (sql = rt_string_concat(sql, rt_string_from(" AND ")));
}
__if_res_16; });
}
});
sql = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(sql, w.column), rt_string_from(" ")), w.operator), rt_string_from(" '")), w.value), rt_string_from("'"));
i = (i + 1LL);
}
;
}
});
({ if (((*self).order_col.length > 0LL)) {
sql = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(sql, rt_string_from(" ORDER BY ")), (*self).order_col), rt_string_from(" ")), OrderDirection_to_sql(&((*self).order_dir)));
}
});
({ if (((*self).limit_count >= 0LL)) {
sql = rt_string_concat(rt_string_concat(sql, rt_string_from(" LIMIT ")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)(*self).limit_count); _buf.ref_count = 1; _buf; }));
}
});
({ if (((*self).offset_count > 0LL)) {
sql = rt_string_concat(rt_string_concat(sql, rt_string_from(" OFFSET ")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)(*self).offset_count); _buf.ref_count = 1; _buf; }));
}
});
return sql;
}

rt_string_t QueryBuilder_count_sql(QueryBuilder* self, rt_string_t col) {
rt_string_t c = ({ rt_string_t __if_res_20; if ((col.length == 0LL)) {
__if_res_20 = rt_string_from("*");
} else {
__if_res_20 = col;
}
__if_res_20; });
rt_string_t sql = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("SELECT COUNT("), c), rt_string_from(") AS aggregate_count FROM ")), (*self).table_name);
({ if (((*self).where_clauses.len > 0LL)) {
sql = rt_string_concat(sql, rt_string_from(" WHERE "));
int64_t i = 0LL;
while ((i < (*self).where_clauses.len)) {
int64_t w = (&((*(int64_t*)rt_vec_get((*self).where_clauses, i))));
({ if ((i > 0LL)) {
({ rt_string_t __if_res_23; if (w.is_or) {
__if_res_23 = (sql = rt_string_concat(sql, rt_string_from(" OR ")));
} else {
__if_res_23 = (sql = rt_string_concat(sql, rt_string_from(" AND ")));
}
__if_res_23; });
}
});
sql = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(sql, w.column), rt_string_from(" ")), w.operator), rt_string_from(" '")), w.value), rt_string_from("'"));
i = (i + 1LL);
}
;
}
});
return sql;
}

rt_string_t QueryBuilder_to_delete_sql(QueryBuilder* self) {
rt_string_t sql = rt_string_concat(rt_string_from("DELETE FROM "), (*self).table_name);
({ if (((*self).where_clauses.len > 0LL)) {
sql = rt_string_concat(sql, rt_string_from(" WHERE "));
int64_t i = 0LL;
while ((i < (*self).where_clauses.len)) {
int64_t w = (&((*(int64_t*)rt_vec_get((*self).where_clauses, i))));
({ if ((i > 0LL)) {
({ rt_string_t __if_res_26; if (w.is_or) {
__if_res_26 = (sql = rt_string_concat(sql, rt_string_from(" OR ")));
} else {
__if_res_26 = (sql = rt_string_concat(sql, rt_string_from(" AND ")));
}
__if_res_26; });
}
});
sql = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(sql, w.column), rt_string_from(" ")), w.operator), rt_string_from(" '")), w.value), rt_string_from("'"));
i = (i + 1LL);
}
;
}
});
return sql;
}

NyxResult QueryBuilder_get(QueryBuilder* self, Database* db) {
return Database_query(db, to_sql(self));
}

NyxResult QueryBuilder_first(QueryBuilder* self, Database* db) {
QueryBuilder q = (QueryBuilder){.table_name = ({ rt_string_t _c = (*self).table_name; rt_string_retain(&_c); _c; }), .select_cols = rt_vec_clone((*self).select_cols), .where_clauses = rt_vec_clone((*self).where_clauses), .order_col = ({ rt_string_t _c = (*self).order_col; rt_string_retain(&_c); _c; }), .order_dir = (*self).order_dir, .limit_count = 1LL, .offset_count = (*self).offset_count};
NyxVec rows = ({ NyxResult _qres = Database_query(db, QueryBuilder_to_sql(&(q))); if (_qres.tag == RESULT_ERR) { return _qres; } *((NyxVec*)_qres.data.ptr_val); });
if ((rows.len > 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxOption _stmp_ = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (int64_t)((*(int64_t*)rt_vec_get(rows, 0LL))); _tmp; }); &_stmp_; }); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Option _stmp_ = rt_option_none(); &_stmp_; }); _tmp; });
}
}

