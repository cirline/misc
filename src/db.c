#include <ccutils/db.h>

//int check_table_and_create()
//

/**
 * db_open - sqlite3_open
 * @filename: database filename (UTF-8)
 * @ppdb: SQLite db handle
 *
 * the return value is a integer equal SQLITE_OK at opened successfully,
 * otherwise an [error code] is returned,
 * use sqlite3_errmsg(ppdb) obtain an error description
 */
int db_open(const char *filename, sqlite3 **ppdb)
{
	return sqlite3_open(filename, ppdb);
}

/**
 * db_exec - sqlite3_exec
 * @hdl: an opened database
 * @sql: SQL to be evaluated
 * @cb : callback function
 * @arg: 1st argument to callback
 * @errmsg: error msg written here, invoke sqlite3_free() to free the malloc memory
 *
 * int (*cb)(void *param, int ncolumn, char **column_val, char **column_name)
 * if cb is not NULL, then it is invoked for each result row
 *
 * the return value is integer equal SQLite_ success/failure codes
 * also write an error message into errmsg
 * if callback returns non-zero, db_exec will break and return SQLITE_ABORT
 */
int db_exec(sqlite3 *hdl, const char *sql, int (*cb)(void*,int,char**,char**), void *arg, char **errmsg)
{
	return sqlite3_exec(hdl, sql, cb, arg, errmsg);
}

