#include <stdio.h>
#include <stdarg.h>
#include <string.h>

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

	fd_check();
	va_start(args, s);
	vfprintf(logfp, s, args);
	va_end(args);
	return 0;
}

int pr_s2hex(const char *s, ...)
{
	va_list args;
	char buf[4096];
	int len;
	int i, j;

	fd_check();
	va_start(args, s);
	vsprintf(buf, s, args);
	va_end(args);

	len = strlen(buf);
	for(i = 0; i < len;) {
		for(j = 0; j < 16 && i < len; j++) {
			pr_log("%x ", buf[i++]);
		}
		pr_log("\n");
	}

	return 0;
}

