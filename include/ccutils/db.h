#ifndef __CC_UTILS_H__
#define __CC_UTILS_H__

#include <ccutils/sqlite3.h>

extern int db_open(const char *filename, sqlite3 **ppdb);
extern int db_exec(sqlite3 *hdl, const char *sql, int (*cb)(void*,int,char**,char**), void *arg, char **errmsg);

#endif
