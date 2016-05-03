
#define pr_fmt(fmt) "test " fmt

#include "log.h"
#include "string_utf8.h"

int main(void)
{
	int i;
	char ext[16];
	char filename[] = "我的朋友.mp3";

	//pr_init("test.tmp.log");

	pr_log("hello world.\n");

	pr_debug("debug message.\n");

	utf8to16("我的朋友.mp3");
	i = utf8chr_revert(filename, '.');

	pr_info(". index = %d\n", i);

	pr_info("get ext: %s\n", utf8_get_file_extension(ext, filename));
	
	return 0;
}
