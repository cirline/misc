
#define pr_fmt(fmt) "test " fmt

#include "log.h"

int main(void)
{

	//pr_init("test.tmp.log");

	pr_log("hello world.\n");

	pr_debug("debug message.\n");
	pr_info("%s message\n", "info");
	pr_err("error %d message\n", 99);

	pr_s2hex("hello %s\n", "s2hex");
	return 0;
}
