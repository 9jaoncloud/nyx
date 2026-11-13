#ifndef RT_DB_H
#define RT_DB_H

#include "rt_string.h"
#include "rt_option.h"
#include "rt_vec.h"
#include "rt_arena.h"
#include <stdint.h>
#include <stdbool.h>

#define NYX_DB_MAX_COLUMNS 16
#define NYX_DB_MAX_ROWS 256
#define NYX_DB_MAX_MIGRATIONS 64

typedef enum {
    DB_DRIVER_SQLITE = 0,
    DB_DRIVER_MYSQL = 1,
    DB_DRIVER_MSSQL = 2,
    DB_DRIVER_POSTGRES = 3
} DbDriverType;

typedef enum {
    DB_TYPE_INT = 1,
    DB_TYPE_FLOAT = 2,
    DB_TYPE_STRING = 3,
    DB_TYPE_NULL = 4
} DbColumnType;

typedef struct {
    char name[32];
    DbColumnType type;
} DbColumnMeta;

typedef struct {
    DbColumnType type;
    union {
        int64_t int_val;
        double float_val;
        char str_val[128];
    } value;
} DbCell;

typedef struct Row {
    DbCell cells[NYX_DB_MAX_COLUMNS];
    int column_count;
} Row;

typedef struct Table {
    char name[64];
    DbColumnMeta columns[NYX_DB_MAX_COLUMNS];
    int column_count;
    Row rows[NYX_DB_MAX_ROWS];
    int row_count;
} Table;

typedef struct Database {
    char uri[256];
    char host[128];
    int port;
    char user[64];
    char password[64];
    char database_name[64];
    DbDriverType driver;
    Table tables[16];
    int table_count;
    int applied_migrations[NYX_DB_MAX_MIGRATIONS];
    int migration_count;
    bool in_transaction;
    bool is_connected;
} Database;

typedef struct DbStmt {
    char sql[512];
    char translated_sql[512];
    DbDriverType target_driver;
    DbCell bound_params[NYX_DB_MAX_COLUMNS];
    int param_count;
    Row result_rows[NYX_DB_MAX_ROWS];
    int result_count;
    int current_cursor;
} DbStmt;

/* ── Multi-Engine Database Connection API ───────────────────────────────── */
NyxResult open_db(rt_string_t path_or_uri);
NyxResult open_db_mysql(rt_string_t host, int port, rt_string_t user, rt_string_t pass, rt_string_t dbname);
NyxResult open_db_mssql(rt_string_t host, int port, rt_string_t user, rt_string_t pass, rt_string_t dbname);
NyxResult open_db_sqlite(rt_string_t file_path);
void close_db(Database* db);

/* Schema & Migrations */
bool nyx_db_apply_migration(Database* db, int version, const char* name, const char* ddl);
int  nyx_db_get_schema_version(Database* db);

/* Statements & Parameterized Queries */
NyxResult prepare(Database* db, rt_string_t sql);
NyxResult prepare_slice(Database* db, NyxFatSlice sql_slice);
NyxResult bind_int(DbStmt* stmt, int64_t idx, int64_t val);
NyxResult bind_string(DbStmt* stmt, int64_t idx, rt_string_t val);
NyxResult bind_float(DbStmt* stmt, int64_t idx, double val);

NyxResult execute_stmt(Database* db, DbStmt* stmt);
NyxResult step(DbStmt* stmt);
Row*      current_row_ptr(DbStmt* stmt);
void      close_stmt(DbStmt* stmt);

/* Direct Query & Exec */
NyxResult exec(Database* db, rt_string_t sql);
NyxResult exec_slice(Database* db, NyxFatSlice sql_slice);
NyxResult execute(Database* db, rt_string_t sql);
NyxResult query(Database* db, rt_string_t sql);

/* Row Accessors */
int64_t     row_get_int(Row* row, int col_idx);
double      row_get_float(Row* row, int col_idx);
const char* row_get_string(Row* row, int col_idx);
NyxOption   get_int(Row* row, rt_string_t col);
NyxOption   get_string(Row* row, rt_string_t col);

/* Dialect Translation Helper */
void nyx_db_translate_dialect(DbDriverType driver, const char* in_sql, char* out_sql, size_t max_out);

#endif // RT_DB_H
