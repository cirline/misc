#include <stdio.h>
#include <stdarg.h>

#define fd_check()	do if (logfp <= 0) logfp = stdout; while(0)

static FILE *logfp;

FILE *pr_init(const char *path)
{
	if(path) {
		logfp = fopen(path, "a+");
	} else
		logfp = stdout;

	return logfp;
}

int pr_log(const char *s, ...)
{
	va_list args;
	int rc;

	fd_check();
	va_start(args, s);
	vfprintf(logfp, s, args);
	va_end(args);
	return 0;
}



