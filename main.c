#define pr_fmt(fmt)	"filesync: " fmt

#include <stdio.h>
#include "filesync.h"

#include "../utils/log.h"

int main(void)
{
	pr_info("main start\n");

	fsloop();

	return 0;
}
