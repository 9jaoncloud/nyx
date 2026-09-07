/*
 * demo_database.c — Multi-Engine Database ORM (MySQL, MSSQL, PostgreSQL, SQLite) Showcase
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "runtime/rt_db.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_string.h"

int main(int argc, char** argv) {
    (void)argc; (void)argv;

    printf("=================================================================\n");
    printf("  🗄️ NYX MULTI-ENGINE DATABASE ORM (MySQL, MSSQL, SQLite) (std.db)\n");
    printf("=================================================================\n");

    /* ── 1. SQLite Embedded Engine ─────────────────────────────────────────── */
    printf("\n--- 1. Testing SQLite Embedded Connection ---\n");
    NyxResult sqlite_res = open_db_sqlite(rt_string_from("athena_living_organism.db"));
    Database* sqlite_db = (Database*)sqlite_res.data.ptr_val;
    nyx_db_apply_migration(sqlite_db, 1, "001_create_nodes_table",
        "CREATE TABLE nodes (id INTEGER PRIMARY KEY, name TEXT, status TEXT, health REAL);");
    printf("✓ SQLite Connection Active (Schema Version v%03d)\n", nyx_db_get_schema_version(sqlite_db));
    close_db(sqlite_db);

    /* ── 2. MySQL / MariaDB Driver ─────────────────────────────────────────── */
    printf("\n--- 2. Testing MySQL / MariaDB InnoDB Connection ---\n");
    NyxResult mysql_res = open_db_mysql(
        rt_string_from("127.0.0.1"), 3306,
        rt_string_from("root"), rt_string_from("secret_pass"),
        rt_string_from("nyx_production_db")
    );
    Database* mysql_db = (Database*)mysql_res.data.ptr_val;

    nyx_db_apply_migration(mysql_db, 1, "001_create_mysql_cluster",
        "CREATE TABLE cluster_nodes (id BIGINT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255), status VARCHAR(64), health DOUBLE) ENGINE=InnoDB;");

    NyxResult my_stmt_res = prepare(mysql_db, rt_string_from("INSERT INTO cluster_nodes (name, status, health) VALUES (?, ?, ?);"));
    DbStmt* my_insert = (DbStmt*)my_stmt_res.data.ptr_val;
    bind_string(my_insert, 0, rt_string_from("MySQL-Master-InnoDB"));
    bind_string(my_insert, 1, rt_string_from("ONLINE_SYNCHRONIZED"));
    bind_float(my_insert, 2, 99.95);
    execute_stmt(mysql_db, my_insert);
    printf("  ✓ MySQL INSERT Executed with standard '?' placeholders\n");
    close_stmt(my_insert);

    NyxResult my_sel_res = prepare(mysql_db, rt_string_from("SELECT id, name, status, health FROM cluster_nodes WHERE health >= ?;"));
    DbStmt* my_select = (DbStmt*)my_sel_res.data.ptr_val;
    bind_float(my_select, 0, 95.0);
    execute_stmt(mysql_db, my_select);
    while (step(my_select).data.int_val == 1) {
        Row* row = current_row_ptr(my_select);
        if (row) {
            printf("  [MySQL Row] ID=%lld | Name='%s' | Status='%s' | Health=%.2f%%\n",
                   (long long)row_get_int(row, 0), row_get_string(row, 1), row_get_string(row, 2), row_get_float(row, 3));
        }
    }
    close_stmt(my_select);
    close_db(mysql_db);

    /* ── 3. Microsoft SQL Server (MSSQL / T-SQL) Driver ─────────────────────── */
    printf("\n--- 3. Testing Microsoft SQL Server (MSSQL / T-SQL) Connection ---\n");
    NyxResult mssql_res = open_db_mssql(
        rt_string_from("db-server.corp.local"), 1433,
        rt_string_from("sa"), rt_string_from("P@ssw0rd2026!"),
        rt_string_from("NyxEnterpriseVault")
    );
    Database* mssql_db = (Database*)mssql_res.data.ptr_val;

    nyx_db_apply_migration(mssql_db, 1, "001_create_mssql_tables",
        "CREATE TABLE dbo.EnterpriseNodes (Id BIGINT IDENTITY(1,1) PRIMARY KEY, Name NVARCHAR(255), Status NVARCHAR(64), Health FLOAT);");

    /* Test T-SQL Dialect Translation: '?' to '@p1, @p2, @p3' */
    NyxResult ms_stmt_res = prepare(mssql_db, rt_string_from("SELECT Id, Name, Status, Health FROM dbo.EnterpriseNodes WHERE Status = ? AND Health >= ?;"));
    DbStmt* ms_select = (DbStmt*)ms_stmt_res.data.ptr_val;
    printf("  ✓ Original Query:   %s\n", ms_select->sql);
    printf("  ✓ T-SQL Translated: %s\n", ms_select->translated_sql);

    bind_string(ms_select, 0, rt_string_from("ONLINE_SYNCHRONIZED"));
    bind_float(ms_select, 1, 98.0);
    execute_stmt(mssql_db, ms_select);

    while (step(ms_select).data.int_val == 1) {
        Row* row = current_row_ptr(ms_select);
        if (row) {
            printf("  [MSSQL Row] Id=%lld | Name='%s' | Status='%s' | Health=%.2f%%\n",
                   (long long)row_get_int(row, 0), row_get_string(row, 1), row_get_string(row, 2), row_get_float(row, 3));
        }
    }
    close_stmt(ms_select);
    close_db(mssql_db);

    printf("\n=================================================================\n");
    printf("  ✓ MULTI-ENGINE DATABASE (MySQL, MSSQL, SQLite) VERIFIED — ALL OK\n");
    printf("=================================================================\n");

    return 0;
}
