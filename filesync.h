#ifndef __FILESYNC_H__
#define __FILESYNC_H__

#include <dirent.h>

/**
 * this custom config
 **
 * #define FILESYNC_PATH
 **
 */
#include "config.h"

#define NET_PORT	9877
#define NET_QUEUE	2

extern int fsloop(void);

#endif
