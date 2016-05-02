
#define pr_fmt(fmt) "test " fmt

#include "log.h"

int main(void)
{

	//pr_init("test.tmp.log");

	pr_log("hello world.\n");

	pr_debug("debug message.\n");
	return 0;
}
