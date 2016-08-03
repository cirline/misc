#include <stdlib.h>

#include <ccutils/db.h>
#include <ccutils/log.h>

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
inline int db_open(const char *filename, sqlite3 **ppdb)
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
inline int db_exec(sqlite3 *hdl, const char *sql, int (*cb)(void*,int,char**,char**), void *arg, char **errmsg)
{
	return sqlite3_exec(hdl, sql, cb, arg, errmsg);
}

static int db_get_acell_callback(void *p, int nc, char **cv, char **cn)
{
	int i;

	pr_here();

	for(i = 0; i < nc; i++) {
		printf("\t%s - %s,", cn[i], cv[i]);
	}
	printf("\n");

	return 0;
}

static char * db_get_acell(sqlite3 *pdb, const char *sql)
{
	char *msg;
	int rc;

	if(!pdb)
		return NULL;

	rc = db_exec(pdb, sql, db_get_acell_callback, NULL, &msg);
	if(rc != SQLITE_OK) {
		pr_err("exec: %s\n", msg);
		free(msg);
	}

	return 0;
}

char * db_get_acell_int(sqlite3 *pdb, const char *tbl, const char *key, int keyval, const char *column)
{
	char *sql;
	char *result;

	asprintf(sql, "select %s from '%s' where %s = %d;", column, tbl, key, keyval);
	result = db_get_acell(pdb, sql);
	free(sql);

	return result;
}

char * db_get_acell_str(sqlite3 *pdb, const char *tbl, const char *key, const char *keyval, const char *column)
{
	char *sql;
	char *result;

	asprintf(sql, "select %s from '%s' where %s = '%s';", column, tbl, key, keyval);
	result = db_get_acell(pdb, sql);
	free(sql);

	return result;
}

int db_table_check(sqlite3 *pdb, const char *tbl, char *cols[], const char *subsql)
{
	char *sql;

	db_get_acell_str(pdb, "sqlite_master", "name", tbl, "*");

	return 0;
}

