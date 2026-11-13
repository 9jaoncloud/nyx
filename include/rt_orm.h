/*
 * rt_orm.h — Nyx Runtime Type-Safe ORM, Query Builder & Auto-Migrations
 */

#ifndef RT_ORM_H
#define RT_ORM_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SQL_DIALECT_SQLITE,
    SQL_DIALECT_MYSQL,
    SQL_DIALECT_POSTGRES,
    SQL_DIALECT_MSSQL
} SqlDialect;

typedef struct {
    char table_name[64];
    char select_fields[256];
    char where_clauses[512];
    char order_by_clause[128];
    char join_clause[256];
    int32_t limit_val;
    int32_t offset_val;
    SqlDialect dialect;
} NyxQueryBuilder;

/* Query Builder API */
NyxQueryBuilder nyx_query_from(const char* table, SqlDialect dialect);
NyxQueryBuilder* nyx_query_select(NyxQueryBuilder* qb, const char* fields);
NyxQueryBuilder* nyx_query_where(NyxQueryBuilder* qb, const char* column, const char* op, const char* val);
NyxQueryBuilder* nyx_query_where_num(NyxQueryBuilder* qb, const char* column, const char* op, double val);
NyxQueryBuilder* nyx_query_join(NyxQueryBuilder* qb, const char* join_type, const char* table, const char* on_clause);
NyxQueryBuilder* nyx_query_order_by(NyxQueryBuilder* qb, const char* column, const char* dir);
NyxQueryBuilder* nyx_query_limit(NyxQueryBuilder* qb, int32_t limit);
NyxQueryBuilder* nyx_query_offset(NyxQueryBuilder* qb, int32_t offset);
void nyx_query_to_sql(const NyxQueryBuilder* qb, char* out_sql, size_t buf_len);

/* Schema Auto-Migrator API */
typedef struct {
    char name[64];
    char type[32];
    bool is_primary;
    bool is_nullable;
} NyxColumnDef;

typedef struct {
    char table_name[64];
    NyxColumnDef columns[16];
    size_t col_count;
} NyxTableSchema;

NyxTableSchema nyx_schema_create_table(const char* name);
void nyx_schema_add_column(NyxTableSchema* schema, const char* col_name, const char* type, bool is_primary, bool is_nullable);
void nyx_schema_generate_ddl(const NyxTableSchema* schema, SqlDialect dialect, char* out_ddl, size_t buf_len);

#ifdef __cplusplus
}
#endif

#endif /* RT_ORM_H */
