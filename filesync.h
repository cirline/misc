#ifndef __FILESYNC_H__
#define __FILESYNC_H__

#include <dirent.h>

#if 0
#define FILESYNC_PATH	"/home/chenqiwei/ddat/e3_share/media/music"
#else
#define FILESYNC_PATH	"/home/chqw/chwork/testfiles/test-music"
#endif

#define NET_PORT	9877
#define NET_QUEUE	2

extern int fsloop(void);

#endif
