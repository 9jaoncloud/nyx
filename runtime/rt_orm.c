/*
 * rt_orm.c — Nyx Runtime Type-Safe ORM, Query Builder & Auto-Migrations Implementation
 */

#include "rt_orm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NyxQueryBuilder nyx_query_from(const char* table, SqlDialect dialect) {
    NyxQueryBuilder qb;
    memset(&qb, 0, sizeof(qb));
    if (table) strncpy(qb.table_name, table, sizeof(qb.table_name) - 1);
    strncpy(qb.select_fields, "*", sizeof(qb.select_fields) - 1);
    qb.limit_val = -1;
    qb.offset_val = -1;
    qb.dialect = dialect;
    return qb;
}

NyxQueryBuilder* nyx_query_select(NyxQueryBuilder* qb, const char* fields) {
    if (qb && fields) strncpy(qb->select_fields, fields, sizeof(qb->select_fields) - 1);
    return qb;
}

NyxQueryBuilder* nyx_query_where(NyxQueryBuilder* qb, const char* column, const char* op, const char* val) {
    if (!qb || !column || !op || !val) return qb;
    char clause[128];
    if (strlen(qb->where_clauses) > 0) {
        snprintf(clause, sizeof(clause), " AND %s %s %s", column, op, val);
    } else {
        snprintf(clause, sizeof(clause), " WHERE %s %s %s", column, op, val);
    }
    strncat(qb->where_clauses, clause, sizeof(qb->where_clauses) - strlen(qb->where_clauses) - 1);
    return qb;
}

NyxQueryBuilder* nyx_query_where_num(NyxQueryBuilder* qb, const char* column, const char* op, double val) {
    char val_str[32];
    snprintf(val_str, sizeof(val_str), "%.2f", val);
    return nyx_query_where(qb, column, op, val_str);
}

NyxQueryBuilder* nyx_query_join(NyxQueryBuilder* qb, const char* join_type, const char* table, const char* on_clause) {
    if (!qb || !table || !on_clause) return qb;
    char j_str[128];
    snprintf(j_str, sizeof(j_str), " %s JOIN %s ON %s", (join_type ? join_type : "INNER"), table, on_clause);
    strncat(qb->join_clause, j_str, sizeof(qb->join_clause) - strlen(qb->join_clause) - 1);
    return qb;
}

NyxQueryBuilder* nyx_query_order_by(NyxQueryBuilder* qb, const char* column, const char* dir) {
    if (!qb || !column) return qb;
    snprintf(qb->order_by_clause, sizeof(qb->order_by_clause), " ORDER BY %s %s", column, (dir ? dir : "ASC"));
    return qb;
}

NyxQueryBuilder* nyx_query_limit(NyxQueryBuilder* qb, int32_t limit) {
    if (qb) qb->limit_val = limit;
    return qb;
}

NyxQueryBuilder* nyx_query_offset(NyxQueryBuilder* qb, int32_t offset) {
    if (qb) qb->offset_val = offset;
    return qb;
}

void nyx_query_to_sql(const NyxQueryBuilder* qb, char* out_sql, size_t buf_len) {
    if (!qb || !out_sql || buf_len == 0) return;

    snprintf(out_sql, buf_len, "SELECT %s FROM %s%s%s%s",
             qb->select_fields, qb->table_name, qb->join_clause, qb->where_clauses, qb->order_by_clause);

    if (qb->limit_val >= 0) {
        char lim[64];
        if (qb->offset_val >= 0) {
            snprintf(lim, sizeof(lim), " LIMIT %d OFFSET %d;", qb->limit_val, qb->offset_val);
        } else {
            snprintf(lim, sizeof(lim), " LIMIT %d;", qb->limit_val);
        }
        strncat(out_sql, lim, buf_len - strlen(out_sql) - 1);
    } else {
        strncat(out_sql, ";", buf_len - strlen(out_sql) - 1);
    }
}

/* ── Schema Migrator Implementation ──────────────────────────────────────── */
NyxTableSchema nyx_schema_create_table(const char* name) {
    NyxTableSchema s;
    memset(&s, 0, sizeof(s));
    if (name) strncpy(s.table_name, name, sizeof(s.table_name) - 1);
    return s;
}

void nyx_schema_add_column(NyxTableSchema* schema, const char* col_name, const char* type, bool is_primary, bool is_nullable) {
    if (!schema || schema->col_count >= 16 || !col_name || !type) return;
    NyxColumnDef* c = &schema->columns[schema->col_count++];
    strncpy(c->name, col_name, sizeof(c->name) - 1);
    strncpy(c->type, type, sizeof(c->type) - 1);
    c->is_primary = is_primary;
    c->is_nullable = is_nullable;
}

void nyx_schema_generate_ddl(const NyxTableSchema* schema, SqlDialect dialect, char* out_ddl, size_t buf_len) {
    (void)dialect;
    if (!schema || !out_ddl || buf_len == 0) return;

    snprintf(out_ddl, buf_len, "CREATE TABLE IF NOT EXISTS %s (\n", schema->table_name);
    for (size_t i = 0; i < schema->col_count; i++) {
        const NyxColumnDef* c = &schema->columns[i];
        char line[128];
        snprintf(line, sizeof(line), "  %s %s%s%s%s\n",
                 c->name, c->type,
                 (c->is_primary ? " PRIMARY KEY" : ""),
                 (!c->is_nullable && !c->is_primary ? " NOT NULL" : ""),
                 (i + 1 < schema->col_count ? "," : ""));
        strncat(out_ddl, line, buf_len - strlen(out_ddl) - 1);
    }
    strncat(out_ddl, ");", buf_len - strlen(out_ddl) - 1);
}
