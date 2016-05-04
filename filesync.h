#ifndef __FILESYNC_H__
#define __FILESYNC_H__

#include <dirent.h>

#if 0
#define FILESYNC_PATH	"/home/chenqiwei/ddat/e3_share/media/music"
#else
#define FILESYNC_PATH	"/home/chqw/chwork/testfiles/test-music"
#endif

struct dir_node {
	struct dir_node *prev;

	DIR *d;
	char *path;
};

struct file_desc {
	char * filename;
	char * rel_dir;
	int hash;
};

extern int fsloop(void);

#endif
